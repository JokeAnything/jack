#include "hlddz_game_round_impl.h"
#include "../../utils/game_process_utils.h"
#include "hlddz_data_bin_offset_defs.h"
#include "hlddz_game_common_api.h"
#include "logger\logger.h"
#include <windows.h>

GDPS_NAMESPACE_BEGIN

#define HLDDZ_GAME_ROUND_TEXT(X) ("[GAME_ROUND_DATA]"##X)

hlddz_game_round_impl* hlddz_game_round_impl::s_myself_this = nullptr;
void* hlddz_game_round_impl::s_notify_give_proc_original = nullptr;
gdps_uint8_t* hlddz_game_round_impl::s_notify_give_proc_original_code_ptr = nullptr;

void* hlddz_game_round_impl::s_game_started_proc_original = nullptr;
gdps_uint8_t* hlddz_game_round_impl::s_game_started_proc_original_code_ptr = nullptr;

void* hlddz_game_round_impl::s_game_bottom_cards_notify_proc_original = nullptr;
gdps_uint8_t* hlddz_game_round_impl::s_game_bottom_cards_notify_proc_original_code_ptr = nullptr;

void* hlddz_game_round_impl::s_user_select_card_change_notify_proc_original = nullptr;
gdps_uint8_t* hlddz_game_round_impl::s_user_select_card_change_notify_proc_original_code_ptr = nullptr;


//card_ui_value g_card_ui_table[] = { "","A","2","3","4","5","6","7","8","9","10","J","Q","K","JOKER_SMALL","JOKER_BIG" };
std::array<card_ui_value, 16> g_card_ui_table = { "","A","2","3","4","5","6","7","8","9","10","J","Q","K","JOKER_SMALL","JOKER_BIG" };
#define IS_CARD_VALUE_VALID(x) (x > 0 && x < g_card_ui_table.size())
#define CARD_VALUE_TO_UI_STRING(x) (g_card_ui_table[x])

#include "hlddz_round_inline_hook.h"

gdps_bool hlddz_game_round_impl::initialize()
{
    s_myself_this = this;
    uint32_t base = (uint32_t)game_process_utils::get_process_image_base();
    auto give_notify_proc_address = (uint32_t)(BO_DATA_ROUND_GIVE_NOTIFY_PROC_ADDRESS(base));
    if (!game_process_utils::set_inline_hook((unsigned char*)give_notify_proc_address,
        (unsigned char*)notify_give_proc_stub,
        &s_notify_give_proc_original,
        11,
        &s_notify_give_proc_original_code_ptr))
    {
        DEBUG_MSG(logger_level_fatal, DEBUG_TEXT_FORMAT("inline give cards notification failed."));
        return false;
    }

    auto game_started_notify_proc_address = (uint32_t)(BO_DATA_ROUND_GAME_START_PROC_ADDRESS(base));
    if (!game_process_utils::set_inline_hook((unsigned char*)game_started_notify_proc_address,
        (unsigned char*)notify_game_started_proc_stub,
        &s_game_started_proc_original,
        8,
        &s_game_started_proc_original_code_ptr))
    {
        DEBUG_MSG(logger_level_fatal, DEBUG_TEXT_FORMAT("inline game started notification failed."));
        return false;
    }

    auto game_bottom_cards_notify_proc_address = (uint32_t)(BO_DATA_ROUND_GAME_BOTTOM_CARDS_NOTIFY_PROC_ADDRESS(base));
    if (!game_process_utils::set_inline_hook((unsigned char*)game_bottom_cards_notify_proc_address,
        (unsigned char*)notify_game_bottom_cards_proc_stub,
        &s_game_bottom_cards_notify_proc_original,
        0xd,
        &s_game_bottom_cards_notify_proc_original_code_ptr))
    {
        DEBUG_MSG(logger_level_fatal, DEBUG_TEXT_FORMAT("inline game started notification failed."));
        return false;
    }

    auto game_user_select_card_change_notify_proc_address = (uint32_t)(BO_DATA_ACTION_GAME_SELECT_CHANGE_NOTIFY_PROC_ADDRESS(base));
    if (!game_process_utils::set_inline_hook((unsigned char*)game_user_select_card_change_notify_proc_address,
        (unsigned char*)notify_user_select_card_change_notify_proc_stub,
        &s_user_select_card_change_notify_proc_original,
        0xf,
        &s_user_select_card_change_notify_proc_original_code_ptr))
    {
        DEBUG_MSG(logger_level_fatal, DEBUG_TEXT_FORMAT("inline game selected card notify failed."));
        return false;
    }

    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("inline hook data notification successfully."));
    return true;
}

gdps_void hlddz_game_round_impl::uninitialize()
{
    uint32_t base = (uint32_t)game_process_utils::get_process_image_base();

    auto give_notify_proc_address = (uint32_t)(BO_DATA_ROUND_GIVE_NOTIFY_PROC_ADDRESS(base));
    if (game_process_utils::restore_inline_hook((unsigned char*)give_notify_proc_address,
        s_notify_give_proc_original_code_ptr,
        11))
    {
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("restore inline give cards notification successfully."));
    }

    auto game_started_notify_proc_address = (uint32_t)(BO_DATA_ROUND_GAME_START_PROC_ADDRESS(base));
    if (game_process_utils::restore_inline_hook((unsigned char*)game_started_notify_proc_address,
        s_game_started_proc_original_code_ptr,
        8))
    {
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("restore inline game started notification successfully."));
    }

    auto game_bottom_cards_notify_proc_address = (uint32_t)(BO_DATA_ROUND_GAME_BOTTOM_CARDS_NOTIFY_PROC_ADDRESS(base));
    if (game_process_utils::restore_inline_hook((unsigned char*)game_bottom_cards_notify_proc_address,
        s_game_bottom_cards_notify_proc_original_code_ptr,
        0xd))
    {
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("restore inline game bottom cards notification successfully."));
    }

    auto game_user_select_card_change_notify_proc_address = (uint32_t)(BO_DATA_ACTION_GAME_SELECT_CHANGE_NOTIFY_PROC_ADDRESS(base));
    if (!game_process_utils::restore_inline_hook((unsigned char*)game_user_select_card_change_notify_proc_address,
        s_user_select_card_change_notify_proc_original_code_ptr,
        0xf))
    {
        DEBUG_MSG(logger_level_fatal, DEBUG_TEXT_FORMAT("restore inline user_select_card_change_notify successfully.."));
    }

    s_myself_this = nullptr;
    return;
}

bool hlddz_game_round_impl::register_round_data_notify(const round_data_notify_callback& notify_callback)
{
    m_round_data_notify_proc = notify_callback;
    return true;
}

void hlddz_game_round_impl::unregister_round_data_notify()
{
    m_round_data_notify_proc = nullptr;
}

void hlddz_game_round_impl::reset_round_data()
{
    m_current_turning_role = role_position_invalid;
    m_myself_handcards.clear();
    m_myself_ui_view_handcards.clear();
    m_bottom_cards.clear();
    m_last_given_cards.clear();
    m_landlord = role_position_invalid;
    m_player_hand_card_count = { FARMER_PLAYER_MAX_CARD_COUNT, FARMER_PLAYER_MAX_CARD_COUNT, FARMER_PLAYER_MAX_CARD_COUNT };
    m_given_history.clear();
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ROUND_TEXT("reset_round_data")));
}

void hlddz_game_round_impl::get_myself_handcards(card_list& myself_card_list)
{
    myself_card_list = m_myself_handcards;
}

role_position hlddz_game_round_impl::get_landlord_position()
{
    return m_landlord;
}

void hlddz_game_round_impl::get_bottom_cards(card_list& bottom_card_list)
{
    bottom_card_list = m_bottom_cards;
}

void hlddz_game_round_impl::get_selected_card_list(card_list& list)
{
    parse_selected_cards(list);
}

void hlddz_game_round_impl::get_myself_view_hand_cards(card_list& myself_view_card_list)
{
    myself_view_card_list = m_myself_ui_view_handcards;

    // reset card index.starting from 0.
    auto idx = 0;
    for (auto& item : myself_view_card_list)
    {
        item.m_card_view_pos_index = idx;
        idx++;
    }
}

role_position hlddz_game_round_impl::get_last_given_role_pos()
{
    return m_last_given_pos;
}

void hlddz_game_round_impl::get_last_given_cards(card_list& given_card_list, role_position& who_did_role_pos, uint32_t& left_count)
{
    given_card_list = m_last_given_cards;
    who_did_role_pos = m_last_given_pos;
    left_count = m_player_hand_card_count[m_last_given_pos];
}

void hlddz_game_round_impl::parse_current_given_cards()
{
    auto data_mgr_base_ptr = (unsigned char*)get_round_data_mgr_base();

    // get given cards detail.
    auto data_mgr_base_delta_ptr = *(uint32_t*)(data_mgr_base_ptr + 0x3C);
    if (!data_mgr_base_delta_ptr)
    {
        return;
    }

    auto card_set_this_ptr = (unsigned char*)(data_mgr_base_delta_ptr + 0x64);
    if (!card_set_this_ptr)
    {
        return;
    }

    card_list give_list;
    if (get_card_set_list(card_set_this_ptr, give_list))
    {
        m_last_given_cards = give_list;
        m_player_hand_card_count[m_current_turning_role] = m_player_hand_card_count[m_current_turning_role] -
            m_last_given_cards.size();

        m_last_given_pos = m_current_turning_role;
        m_given_history.push_back(m_last_given_cards);
        auto card_string = get_card_list_string(m_last_given_cards);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ROUND_TEXT("current given cards :%s,view_chair:%d,left card count:%d")),
            card_string.c_str(),
            m_last_given_pos,
            m_player_hand_card_count[m_last_given_pos]);

        // Here it's to remove the cards that myself held.Those can not be given any more.
        if (m_current_turning_role == role_position_myself)
        {
            for (auto&& item : m_last_given_cards)
            {
                auto iter = std::find_if(m_myself_ui_view_handcards.begin(),
                    m_myself_ui_view_handcards.end(),
                    [item](auto&& view_item)->bool {
                    return ((view_item.m_card_value == item.m_card_value) && (view_item.m_card_shape == item.m_card_shape)) ? true : false;
                });
                if (iter != std::end(m_myself_ui_view_handcards))
                {
                    m_myself_ui_view_handcards.erase(iter);
                }
            }

            // Because the m_myself_ui_view_handcards list was sorted before, it does not need to sort again.
            // Just output it.
            auto card_info_string = get_card_list_string(m_myself_ui_view_handcards);
            DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ROUND_TEXT("current myself view hand cards:%s")), card_info_string.c_str());
        }

        // set next player.
        m_current_turning_role = (role_position)(((uint32_t)m_current_turning_role + 1) % 3);

        if (m_round_data_notify_proc)
        {
            m_round_data_notify_proc(round_data_notify_type_receive_given_cards, (void*)m_last_given_pos);
        }

        if (m_player_hand_card_count[m_last_given_pos] == 0)
        {
            m_round_data_notify_proc(round_data_notify_type_game_over, (void*)m_last_given_pos);
        }
    }
}

void hlddz_game_round_impl::parse_bottom_cards(void* card_set_base)
{
    if (!card_set_base)
    {
        return;
    }

    if (get_card_set_list(card_set_base, m_bottom_cards))
    {
        auto card_string = get_card_list_string(m_bottom_cards);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ROUND_TEXT("current bottom cards :%s")), card_string.c_str());
    }
    else
    {
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ROUND_TEXT("parsed bottom cards failed.")));
    }

    // get landlord view_chair.
    auto data_mgr_base_ptr = (unsigned char*)get_round_data_mgr_base();
    if (data_mgr_base_ptr == nullptr)
    {
        return;
    }
    void* landlord_index_ptr = (void*)(*((gdps_uint32_t*)((unsigned char*)data_mgr_base_ptr + 0x10)));
    if (landlord_index_ptr == nullptr)
    {
        return;
    }
    m_landlord =(role_position)(*(((unsigned char*)landlord_index_ptr + 0x124)));
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ROUND_TEXT("landlord_view_pos:%d.")), m_landlord);
    m_current_turning_role = m_landlord;
    m_player_hand_card_count[m_landlord] = LANDLORD_PLAYER_MAX_CARD_COUNT;

    if ((m_landlord == role_position_myself) && !m_bottom_cards.empty())
    {
        m_myself_ui_view_handcards.insert(m_myself_ui_view_handcards.begin(),
            m_bottom_cards.begin(),
            m_bottom_cards.end());
    }

    std::sort(m_myself_ui_view_handcards.begin(), m_myself_ui_view_handcards.end(),
        [](auto&& item_a,auto&& item_b){
        return item_a.m_card_level > item_b.m_card_level;
    });
    auto card_info_string = get_card_list_string(m_myself_ui_view_handcards);
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ROUND_TEXT("myself view hand cards:%s")), card_info_string.c_str());

    if (m_round_data_notify_proc)
    {
        m_round_data_notify_proc(round_data_notify_type_receive_bottom_cards, (void*)m_landlord);
    }
}

void hlddz_game_round_impl::parse_selected_cards(card_list& list)
{
    //auto data_mgr_base_ptr = (unsigned char*)get_round_data_mgr_base();
    //if (data_mgr_base_ptr == nullptr)
    //{
    //    return;
    //}

    //gdps_uint32_t selected_card = *((gdps_uint32_t*)((unsigned char*)data_mgr_base_ptr + 0x3C));
    //unsigned char* selected_card_ptr = (unsigned char*)(selected_card + 0x0A8);
    //get_card_set_list(selected_card_ptr, list);
}

bool hlddz_game_round_impl::get_card_item(void* pcard, card_item& item)
{
    if (pcard == nullptr)
    {
        return false;
    }

    auto pcard_this_ptr = (unsigned char*)(pcard);
    if (pcard_this_ptr)
    {
        item.m_card_value = *(pcard_this_ptr + 4);
        item.m_card_shape = *(pcard_this_ptr + 5);
        item.m_card_level = *(pcard_this_ptr + 6);
        return true;
    }
    return false;
}

bool hlddz_game_round_impl::get_card_set_list(void* pcard_set, card_list& list)
{
    if (!pcard_set)
    {
        return false;
    }

    auto pcard_set_base_ptr = (unsigned char*)pcard_set;

    uint32_t* item_card_end_ptr = (uint32_t*)(pcard_set_base_ptr + 12);
    uint32_t* item_card_end = (uint32_t*)(*item_card_end_ptr);

    uint32_t* item_card_begin_ptr = (uint32_t*)(pcard_set_base_ptr + 8);
    uint32_t* item_card_begin = (uint32_t*)(*item_card_begin_ptr);

    uint32_t item_card_count = item_card_end - item_card_begin;
    uint32_t index = 0;
    while (index < item_card_count)
    {
        card_item item;
        auto pcard_ptr = *(item_card_begin + index);
        if (!pcard_ptr)
        {
            continue;
        }
        if (get_card_item((void*)pcard_ptr, item))
        {
            list.push_back(item);
        }
        index++;
    }
    return true;
}


void hlddz_game_round_impl::parse_myself_handcards(void* myself_handcard_base)
{
    if (!myself_handcard_base)
    {
        return;
    }

    if (!m_myself_handcards.empty())
    {
        if (m_round_data_notify_proc)
        {
            m_round_data_notify_proc(round_data_notify_type_receive_myself_cards_reset, nullptr);
        }
    }

    if (get_card_set_list(myself_handcard_base, m_myself_handcards))
    {
        m_myself_ui_view_handcards = m_myself_handcards;
        m_player_hand_card_count[role_position_myself] = m_myself_ui_view_handcards.size();
        auto card_string = get_card_list_string(m_myself_handcards);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ROUND_TEXT("myself hand cards :%s")), card_string.c_str());

        if (m_round_data_notify_proc)
        {
            m_round_data_notify_proc(round_data_notify_type_receive_myself_cards, nullptr);
        }
    }
    else
    {
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_ROUND_TEXT("parsed myself hand cards failed.")));
    }
}

void hlddz_game_round_impl::notify_user_select_card_change(void* notify_this)
{
    // disable ui give cards mechanism.
    auto data_mgr_base_ptr = (unsigned char*)get_round_data_mgr_base();
    if (data_mgr_base_ptr == nullptr)
    {
        return;
    }
    auto ai_give = (unsigned char*)(data_mgr_base_ptr + 184);
    *ai_give = 1;
}

gdps_string hlddz_game_round_impl::get_card_list_string(const card_list& list)
{
    gdps_string result;
    for (auto&& item : list)
    {
        gdps_uint8_t value = item.m_card_value;
        if (IS_CARD_VALUE_VALID(value))
        {
            if (result.empty())
            {
                result = result + CARD_VALUE_TO_UI_STRING(value);
            }
            else
            {
                result = result + CARD_UI_VALUE_SPLIT_CHAR;
                result = result + CARD_VALUE_TO_UI_STRING(value);
            }
        }
    }
    return result;
}

gdps_uint8_t hlddz_game_round_impl::convert_ui_value_to_card_value(const card_ui_value& ui_value)
{
    gdps_uint8_t index = 0;
    for (auto&& item : g_card_ui_table)
    {
        if (_stricmp(item.c_str(), ui_value.c_str()) == 0)
        {
            return index;
        }
        index++;
    }
    return 0;
}

card_ui_value hlddz_game_round_impl::convert_card_to_ui_card(const card_item& item)
{
    card_ui_value result;
    if (!IS_CARD_VALUE_VALID(item.m_card_value))
    {
        return result;
    }
    result = g_card_ui_table[item.m_card_value];
    return result;
}

GDPS_NAMESPACE_END
