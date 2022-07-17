#include "hlddz_game_action_impl.h"
#include "../../utils/game_process_utils.h"
#include "hlddz_data_bin_offset_defs.h"
#include "hlddz_game_common_api.h"
#include "logger/logger.h"
#include <windows.h>

GDPS_NAMESPACE_BEGIN

#define HLDDZ_GAME_ACTION_TEXT(X) ("[GAME_ACTION_DATA]"##X)

hlddz_game_action_impl* hlddz_game_action_impl::s_myself_this = nullptr;
void* hlddz_game_action_impl::s_original_get_button_proc = nullptr;
void* hlddz_game_action_impl::s_original_show_button_status_proc = nullptr;
void* hlddz_game_action_impl::s_timer_update_proc_original = nullptr;
gdps_uint8_t* hlddz_game_action_impl::s_timer_update_proc_original_code_ptr = nullptr;

void* hlddz_game_action_impl::s_order_notify_proc_original = nullptr;
gdps_uint8_t* hlddz_game_action_impl::s_order_notify_proc_original_code_ptr = nullptr;

#include "hlddz_action_inline_hook.h"

DWORD WINAPI btn_click_thread_proc(LPVOID lpParam)
{
    if (lpParam == nullptr)
    {
        return 0;
    }
    __try
    {
        void* btn_object = lpParam;
        uint32_t base = (uint32_t)game_process_utils::get_process_image_base();
        auto proc_address = (uint32_t)(BO_BUTTON_OBJECT_CLICK_PROC_ADDRESS(base));
        __asm
        {
            pushad
            pushfd
            mov ecx, btn_object
            push 2 // clicked event type.
            call proc_address
            popfd
            popad
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
    return 1;
}

gdps_bool hlddz_game_action_impl::initialize()
{
    std::lock_guard<std::mutex> guard(data_locker);

    m_button_object_list[BO_BUTTON_NAME_BTN_STARTGAME] = hlddz_button_info(player_action_type_start_game, nullptr,false);
    m_button_object_list[BO_BUTTON_NAME_BTN_BUJIAO] = hlddz_button_info(player_action_type_not_being_landlord, nullptr, false);
    m_button_object_list[BO_BUTTON_NAME_BTN_JIAODIZHU] = hlddz_button_info(player_action_type_being_landlord, nullptr, false);
    m_button_object_list[BO_BUTTON_NAME_BTN_BUQIANG] = hlddz_button_info(player_action_type_do_not_rob_landlord, nullptr, false);
    m_button_object_list[BO_BUTTON_NAME_BTN_QIANGDIZHU] = hlddz_button_info(player_action_type_rob_landlord, nullptr, false);
    m_button_object_list[BO_BUTTON_NAME_BTN_JIABEI] = hlddz_button_info(player_action_type_do_double, nullptr, false);
    m_button_object_list[BO_BUTTON_NAME_BTN_BUJIABEI] = hlddz_button_info(player_action_type_do_not_double, nullptr, false);
    m_button_object_list[BO_BUTTON_NAME_BTN_BUCHU] = hlddz_button_info(player_action_type_do_not_give, nullptr, false);
    m_button_object_list[BO_BUTTON_NAME_BTN_CHUPAI] = hlddz_button_info(player_action_type_do_give, nullptr, false);

    m_button_object_list[BO_BUTTON_NAME_BTN_MINGPAI] = hlddz_button_info(player_action_type_do_mingpai, nullptr, false);
    m_button_object_list[BO_BUTTON_NAME_BTN_CHANGE_OPPONENTS] = hlddz_button_info(player_action_type_change_oponents, nullptr, false);
    m_button_object_list[BO_BUTTON_NAME_BTN_SUPER_MULTI] = hlddz_button_info(player_action_type_do_super_double, nullptr, false);
    m_button_object_list[BO_BUTTON_NAME_BTN_SUGGESTION] = hlddz_button_info(player_action_type_suggestion, nullptr, false);

    s_myself_this = this;

    uint32_t base = (uint32_t)game_process_utils::get_process_image_base();
    game_process_utils::hook_function_table_proc((void*)(BO_BUTTON_OBJECT_COLLECT_PROC_ADDRESS(base)),
        (const void*)get_button_object_this_stub,
        (void**)(&hlddz_game_action_impl::s_original_get_button_proc));

    //game_process_utils::hook_function_table_proc((void*)(BO_BUTTON_OBJECT_SHOW_PROC_ADDRESS(base)),
    //    (const void*)get_button_object_show_status_stub,
    //    (void**)(&hlddz_game_action_impl::s_original_show_button_status_proc));

    auto timer_update_proc_address = (uint32_t)(BO_DATA_ROUND_TIMER_UPDATE_PROC_ADDRESS(base));
    game_process_utils::set_inline_hook((unsigned char*)timer_update_proc_address,
        (unsigned char*)notify_timer_update_proc_stub,
        &s_timer_update_proc_original,
        5,
        &s_timer_update_proc_original_code_ptr);

    auto order_notify_proc_address = (uint32_t)(BO_DATA_ACTION_GAME_ORDER_NOTIFY_PROC_ADDRESS(base));
    game_process_utils::set_inline_hook((unsigned char*)order_notify_proc_address,
        (unsigned char*)order_notify_stub,
        &s_order_notify_proc_original,
        10,
        &s_order_notify_proc_original_code_ptr);

    m_game_window = game_process_utils::get_game_window(BO_DATA_UI_WINDOW_CLASS_NAME);
    return true;
}

gdps_void hlddz_game_action_impl::uninitialize()
{
    std::lock_guard<std::mutex> guard(data_locker);

    uint32_t base = (uint32_t)game_process_utils::get_process_image_base();

    auto order_notify_proc_address = (uint32_t)(BO_DATA_ACTION_GAME_ORDER_NOTIFY_PROC_ADDRESS(base));
    game_process_utils::restore_inline_hook((unsigned char*)order_notify_proc_address,
        s_order_notify_proc_original_code_ptr,
        10);

    auto timer_update_proc_address = (uint32_t)(BO_DATA_ROUND_TIMER_UPDATE_PROC_ADDRESS(base));
    game_process_utils::restore_inline_hook((unsigned char*)timer_update_proc_address,
        s_timer_update_proc_original_code_ptr,
        5);

    void* old_proc_address = nullptr;

    //game_process_utils::hook_function_table_proc((void*)(BO_BUTTON_OBJECT_SHOW_PROC_ADDRESS(base)),
    //    (const void*)hlddz_game_action_impl::s_original_show_button_status_proc,
    //    (void**)(&old_proc_address));

    game_process_utils::hook_function_table_proc((void*)(BO_BUTTON_OBJECT_COLLECT_PROC_ADDRESS(base)),
        (const void*)hlddz_game_action_impl::s_original_get_button_proc,
        (void**)(&old_proc_address));
    s_myself_this = nullptr;
    m_button_object_list.clear();
    m_game_window = nullptr;

}

bool hlddz_game_action_impl::register_player_action_notify(const player_action_notify_callback& notify_callback)
{
    m_notify_callback = notify_callback;
    return true;
}

void hlddz_game_action_impl::unregister_player_action_notify()
{
    m_notify_callback = nullptr;
}

void hlddz_game_action_impl::save_button_object(const object_name::value_type* button_name, object_this object_ptr)
{
    if (button_name == nullptr || object_ptr == nullptr)
    {
        return;
    }

    std::string btn_name = button_name;

    std::lock_guard<std::mutex> guard(data_locker);
    auto found = m_button_object_list.find(btn_name);
    if (found != m_button_object_list.end())
    {
        found->second.m_button_object_this = object_ptr;
    }
}

void hlddz_game_action_impl::update_player_timer_info(gdps_uint8_t view_chair_pos, uint32_t time_left)
{
    m_current_turning_role = (role_position)view_chair_pos;
    if (m_notify_callback)
    {
        m_notify_callback(view_chair_pos, time_left);
    }
}

void hlddz_game_action_impl::order_notify_proc_logic(void* order_object)
{
    if (!order_object)
    {
        return;
    }
    gdps_uint8_t cur_svc_pos = *((unsigned char*)order_object + 8);
    auto cur_object = get_view_player_object(cur_svc_pos);
    gdps_uint8_t cur_chair_view_pos = get_view_chair_pos(cur_object);

    gdps_uint8_t next_svc_pos = *((unsigned char*)order_object + 9);
    auto next_object = get_view_player_object(next_svc_pos);
    gdps_uint8_t next_chair_view_pos = get_view_chair_pos(next_object);

    gdps_uint8_t status = *((unsigned char*)order_object + 10);
    if (status == 0xFF)
    {
        m_order_status = game_status_order_type_ordering;
    }
    else
    {
        m_order_status = game_status_order_type_robbing;
    }

    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ACTION_TEXT("order current view chair:%d, next view chair:%d, current order status:%d.")),
        cur_chair_view_pos,
        next_chair_view_pos,
        status);
}

bool hlddz_game_action_impl::click_button(player_action_type button_type)
{
    std::lock_guard<std::mutex> guard(data_locker);
    object_this btn_object = nullptr;
    object_name btn_name;
    for (auto&& item : m_button_object_list)
    {
        if (item.second.m_action_type == button_type)
        {
            btn_name = item.first;
            btn_object = item.second.m_button_object_this;
            break;
        }
    }

    if (!btn_object)
    {
        return false;
    }

    DWORD dwTid = 0;
    auto handle = ::CreateThread(NULL, 0, btn_click_thread_proc, (LPVOID)btn_object, 0, &dwTid);
    ::CloseHandle(handle);

    // It's necessary to check the button whether or not it was clicked successfully.

    return true;
}

#define CARDS_X_STARTING_POSITION(wnd_width, select_area, card_width, card_count) ((wnd_width / 2)-((card_width + ((card_count - 1) * select_area)) / 2))
#define CARD_X_CLICK_POSITION(starting_pos, select_area, card_index) (starting_pos + card_index * select_area + select_area / 2)

bool hlddz_game_action_impl::select_cards(const card_list& selected_list, uint32_t total_cards)
{
    if (selected_list.empty())
    {
        return true;
    }
    if (m_game_window == nullptr)
    {
        return false;
    }
    if (total_cards == 0)
    {
        return false;
    }

    // reset card selected state.
    reset_card_select_status(total_cards);

    for (int32_t cur_index = (int32_t)(total_cards - 1); cur_index >= 0; cur_index--)
    {
        bool selected = false;
        for (const auto& item : selected_list)
        {
            if (item.m_card_view_pos_index == cur_index)
            {
                selected = true;
                break;
            }
        }

        //if (!selected)
        //{
        //    continue;
        //}

        // reset all cards from [0,cur_index).
        for (int32_t reset_card_index = 0; reset_card_index < cur_index; reset_card_index++)
        {
            auto cur_selected_state = get_current_card_select_status(reset_card_index, total_cards);
            auto unselected_state = get_card_unselected_status(reset_card_index);
            if (cur_selected_state != unselected_state)
            {
                click_card(reset_card_index, total_cards);
            }
        }

        // select the current card.
        {
            auto cur_selected_state = get_current_card_select_status(cur_index, total_cards);
            auto unselected_state = get_card_unselected_status(cur_index);

            if ((unselected_state != cur_selected_state && !selected) ||
                (unselected_state == cur_selected_state && selected))
            {
                click_card(cur_index, total_cards);
            }
        }

        //    auto current_selected_state = game_process_utils::get_point_pixel(m_game_window, x_pos, y_selected_stated_pos);
        //    auto unselected_state = get_card_selected_status(cur_index);


        //    DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ACTION_TEXT("[card_selected_info]card index:%d before_state:%u,now_state:%u.")),
        //        cur_index,
        //        unselected_state,
        //        current_selected_state);

        //    if (unselected_state == 0)
        //    {
        //        DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ACTION_TEXT("[card_selected]card index:%d selected state 0.")),
        //            cur_index);
        //    }
        //    else if ((unselected_state != current_selected_state) && selected)
        //    {
        //        // was meant to select but selected->do nothing.
        //    }
        //    else if ((unselected_state != current_selected_state && !selected) ||
        //        (unselected_state == current_selected_state && selected))
        //    {
        //    }
        //}
    }
    return true;
}

void hlddz_game_action_impl::click_card(uint32_t card_index,uint32_t total_cards)
{
    uint32_t wnd_width = 0;
    uint32_t wnd_height = 0;
    game_process_utils::get_wnd_width_height(m_game_window, wnd_width, wnd_height);
    if ((wnd_width == 0) || (wnd_height == 0))
    {
        return;
    }

    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ACTION_TEXT("[card_selected]game window width:%d height:%d")),
        wnd_width,
        wnd_height);

    auto starting_pos = CARDS_X_STARTING_POSITION(wnd_width, BO_DATA_UI_PIXEL_SELECT_CARD_AREA, BO_DATA_UI_PIXEL_CARD_WIDTH, total_cards);

    auto x_pos = CARD_X_CLICK_POSITION(starting_pos, BO_DATA_UI_PIXEL_SELECT_CARD_AREA, card_index);
    //auto y_selected_stated_pos = wnd_height - BO_DATA_UI_PIXEL_CARD_SELECTED_JUDGE_DELTA;

    // was meant not to select but selected->unselected.
    // was meant to select but not selected->selected.
    auto y_pos = wnd_height - BO_DATA_UI_PIXEL_CARD_BOTTOM_DELTA;
    game_process_utils::post_left_button_down_message(m_game_window, x_pos, y_pos);
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ACTION_TEXT("[card_selected]click down select card index:%d click_x:%d click_y:%d.")),
        card_index,
        x_pos,
        y_pos);
    ::Sleep(200);

    {
        auto cur_selected_state =get_current_card_select_status(card_index, total_cards);
        auto before_selected_state = get_card_unselected_status(card_index);

        DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ACTION_TEXT("[card_selected_info]after click down card index:%d before_state:%u,now_state:%u.")),
            card_index,
            before_selected_state,
            cur_selected_state);
    }

    game_process_utils::post_left_button_up_message(m_game_window, x_pos, y_pos);
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ACTION_TEXT("[card_selected]click up select card index:%d click_x:%d click_y:%d.")),
        card_index,
        x_pos,
        y_pos);
    ::Sleep(200);

    {
        auto cur_selected_state = get_current_card_select_status(card_index, total_cards);
        auto before_selected_state = get_card_unselected_status(card_index);

        DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ACTION_TEXT("[card_selected_info]after click up card index:%d before_state:%u,now_state:%u.")),
            card_index,
            before_selected_state,
            cur_selected_state);
    }
}

uint32_t hlddz_game_action_impl::get_current_card_select_status(uint32_t card_index, uint32_t total_cards)
{
    uint32_t wnd_width = 0;
    uint32_t wnd_height = 0;
    game_process_utils::get_wnd_width_height(m_game_window, wnd_width, wnd_height);
    if ((wnd_width == 0) || (wnd_height == 0))
    {
        return 0;
    }
    auto starting_pos = CARDS_X_STARTING_POSITION(wnd_width, BO_DATA_UI_PIXEL_SELECT_CARD_AREA, BO_DATA_UI_PIXEL_CARD_WIDTH, total_cards);
    auto x_pos = CARD_X_CLICK_POSITION(starting_pos, BO_DATA_UI_PIXEL_SELECT_CARD_AREA, card_index);
    auto y_selected_stated_pos = wnd_height - BO_DATA_UI_PIXEL_CARD_SELECTED_JUDGE_DELTA;
    return game_process_utils::get_point_pixel(m_game_window, x_pos, y_selected_stated_pos);
}

void hlddz_game_action_impl::reset_card_select_status(uint32_t total_cards)
{
    for (size_t i = 0; i < m_unselected_card_state_list.size(); i++)
    {
        m_unselected_card_state_list[i] = 0;
    }

    uint32_t wnd_width = 0;
    uint32_t wnd_height = 0;
    game_process_utils::get_wnd_width_height(m_game_window, wnd_width, wnd_height);
    if ((wnd_width == 0) || (wnd_height == 0))
    {
        DEBUG_MSG(logger_level_warning, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ACTION_TEXT("[card_selected]game windows metric got failed.")));
        return;
    }


    auto starting_pos = CARDS_X_STARTING_POSITION(wnd_width, BO_DATA_UI_PIXEL_SELECT_CARD_AREA, BO_DATA_UI_PIXEL_CARD_WIDTH, total_cards);
    for (size_t i = 0; i < total_cards; i++)
    {
        auto x_pos = CARD_X_CLICK_POSITION(starting_pos, BO_DATA_UI_PIXEL_SELECT_CARD_AREA, i);
        auto y_pos = wnd_height - BO_DATA_UI_PIXEL_CARD_SELECTED_JUDGE_DELTA;
        auto state = game_process_utils::get_point_pixel(m_game_window, x_pos, y_pos);
        m_unselected_card_state_list[i] = state;
        /*DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ACTION_TEXT("[card_selected_info]card index:%d reset_state:%u.")),
            i,
            state);*/
    }
}

uint32_t hlddz_game_action_impl::get_card_unselected_status(uint32_t index)
{
    if (index >= m_unselected_card_state_list.size())
    {
        return 0;
    }
    return m_unselected_card_state_list[index];
}


game_status_order_type hlddz_game_action_impl::get_current_order_status()
{
    return m_order_status;
}

GDPS_NAMESPACE_END
