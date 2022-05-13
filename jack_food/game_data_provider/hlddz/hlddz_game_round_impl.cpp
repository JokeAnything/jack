#include "hlddz_game_round_impl.h"
#include "../../utils/game_process_utils.h"
#include "hlddz_data_bin_offset_defs.h"
#include "logger\logger.h"
#include <windows.h>

GDPS_NAMESPACE_BEGIN

hlddz_game_round_impl* hlddz_game_round_impl::s_myself_this = nullptr;
void* hlddz_game_round_impl::s_notify_give_proc_original = nullptr;
gdps_uint8_t* hlddz_game_round_impl::s_notify_give_proc_original_code_ptr = nullptr;

void* hlddz_game_round_impl::s_game_started_proc_original = nullptr;
gdps_uint8_t* hlddz_game_round_impl::s_game_started_proc_original_code_ptr = nullptr;

void* hlddz_game_round_impl::s_game_bottom_cards_notify_proc_original = nullptr;
gdps_uint8_t* hlddz_game_round_impl::s_game_bottom_cards_notify_proc_original_code_ptr = nullptr;

card_ui_value g_card_ui_table[] = { "","A","2","3","4","5","6","7","8","9","10","J","Q","K","JOKER_BIG","JOKER_SMALL" };
#define IS_CARD_VALUE_VALID(x) (x > 0 && x < _countof(g_card_ui_table))
#define CARD_VALUE_TO_UI_STRING(x) (g_card_ui_table[x])

void __stdcall get_player_given_cards_info()
{
    if (hlddz_game_round_impl::s_myself_this)
    {
        hlddz_game_round_impl::s_myself_this->parse_current_given_cards();
    }
}

void __stdcall game_started_notify(void* self_card_base_ptr)
{
    if (!self_card_base_ptr)
    {
        return;
    }
    if (hlddz_game_round_impl::s_myself_this)
    {
        hlddz_game_round_impl::s_myself_this->parse_myself_handcards(self_card_base_ptr);
    }
}

void __stdcall game_bottom_cards_notify(void* card_set_base)
{
    if (!card_set_base)
    {
        return;
    }

    if (hlddz_game_round_impl::s_myself_this)
    {
        hlddz_game_round_impl::s_myself_this->parse_bottom_cards(card_set_base);
    }
}

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

    s_myself_this = nullptr;
    return;
}

void* get_round_data_mgr_base()
{
    uint32_t base = (uint32_t)game_process_utils::get_process_image_base();
    auto proc_address = (uint32_t)(BO_DATA_ROUND_MGR_BASE_PTR_PROC_ADDRESS(base));
    void* data_mgr_base = nullptr;

    __try
    {
        __asm
        {
            pushad
            pushfd
            call proc_address
            push eax
            pop data_mgr_base
            popfd
            popad
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
    return data_mgr_base;
}

void* get_view_player_object(gdps_uint8_t pos)
{
    uint32_t base = (uint32_t)game_process_utils::get_process_image_base();
    auto proc_address = (uint32_t)(BO_DATA_ROUND_VIEW_PLAYER_OBJECT_PTR_PROC_ADDRESS(base));
    void* data_mgr_base = get_round_data_mgr_base();
    if (data_mgr_base == nullptr)
    {
        return nullptr;
    }
    void* view_player_object = nullptr;
    uint32_t position = (uint32_t)pos;
    __try
    {
        __asm
        {
            pushad
            pushfd
            push position
            mov ecx,data_mgr_base
            call proc_address
            mov view_player_object,eax
            popfd
            popad
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
    return view_player_object;
}

bool hlddz_game_round_impl::get_current_turning(role_position& pos)
{
    return true;
    gdps_uint8_t view_char = 0;
    auto data_mgr_base = get_round_data_mgr_base();
    if (!data_mgr_base)
    {
        return false;
    }
    //view_char = *((gdps_uint8_t*)data_mgr_base + BO_DATA_ROUND_ON_TURN_VIEW_CHAR_OFFSET);
    //DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("current turning view:%d."), view_char);
    return true;
}

void hlddz_game_round_impl::parse_current_given_cards()
{
    auto data_mgr_base_ptr = (unsigned char*)get_round_data_mgr_base();
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
        m_given_history.push_back(give_list);
        auto card_string = get_card_list_string(give_list);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("current given cards :%s"), card_string.c_str());
    }
}

void hlddz_game_round_impl::parse_bottom_cards(void* card_set_base)
{
    if (!card_set_base)
    {
        return;
    }

    card_list list;
    if (get_card_set_list(card_set_base, list))
    {
        auto card_string = get_card_list_string(list);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("current bottom cards :%s"), card_string.c_str());
    }
    else
    {
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("parsed bottom cards failed."));
    }

    // get landlord view_chair.
    auto data_mgr_base_ptr = (unsigned char*)get_round_data_mgr_base();
    void* landlord_index_ptr = (void*)(*((gdps_uint32_t*)((unsigned char*)data_mgr_base_ptr + 0x10)));
    gdps_uint8_t landlord_index = *(((unsigned char*)landlord_index_ptr + 0x124));
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("landlord_view_index:%d."), landlord_index);
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

    if (get_card_set_list(myself_handcard_base, m_myself_handchads))
    {
        auto card_string = get_card_list_string(m_myself_handchads);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("myself hand cards :%s"), card_string.c_str());
    }
    else
    {
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("parsed myself hand cards failed."));
    }
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
                result = result + ":";
                result = result + CARD_VALUE_TO_UI_STRING(value);
            }
        }
    }
    return result;
}

GDPS_NAMESPACE_END
