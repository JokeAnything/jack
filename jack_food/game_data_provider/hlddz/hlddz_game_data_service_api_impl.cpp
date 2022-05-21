#include "hlddz_game_data_service_api_impl.h"
#include "../../utils/game_string_utils.h"
#include "logger\logger.h"

GDPS_NAMESPACE_BEGIN

#define HLDDZ_GAME_DATA_SVC_API_TEXT(X) ("[GAME_DATA_SVC_API]"##X)

gdps_bool hlddz_game_data_service_api_impl::initialize()
{
    if (!m_action_object.initialize())
    {
        return false;
    }
    if (!m_game_round_object.initialize())
    {
        return false;
    }
    m_action_object.register_player_action_notify(std::bind(&hlddz_game_data_service_api_impl::notify_player_action,
        this,
        std::placeholders::_1,
        std::placeholders::_2
        ));
    m_game_round_object.register_round_data_notify(std::bind(&hlddz_game_data_service_api_impl::notify_round_data,
        this,
        std::placeholders::_1,
        std::placeholders::_2));

    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("hlddz game data service initialized."));
    return true;
}

gdps_void hlddz_game_data_service_api_impl::uninitialize()
{
    m_game_round_object.unregister_round_data_notify();
    m_action_object.unregister_player_action_notify();
    m_game_round_object.uninitialize();
    m_action_object.uninitialize();
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("hlddz game data service deinitialized."));
}

bool hlddz_game_data_service_api_impl::register_action_notify(const request_action_notify_callback& notify_callback)
{
    m_request_action_notify_callback = notify_callback;
    return true;
}

void hlddz_game_data_service_api_impl::unregister_action_notify()
{
    m_request_action_notify_callback = nullptr;
}

bool hlddz_game_data_service_api_impl::get_current_turning_role(role_position& pos)
{
    //m_game_round_object.get_current_turning(pos);
    //pos = role_position_invalid;
    //DEBUG_MSG(logger_level_info, DEBUG_TEXT_FORMAT("current turn pos:%d."), pos);
    return false;
}

bool hlddz_game_data_service_api_impl::get_role_card_number(role_position role_id, card_number& number)
{
    DEBUG_MSG(logger_level_info, DEBUG_TEXT_FORMAT("pos:%d.car number:%d"), role_id, number);
    return false;
}

bool hlddz_game_data_service_api_impl::get_role_hand_cards(role_position role_id, card_list& list)
{
    return false;
}

bool hlddz_game_data_service_api_impl::get_role_given_cards(role_position role_id, card_list& list)
{
    return false;
}

bool hlddz_game_data_service_api_impl::execute_current_player_action(player_action_type type)
{
    DEBUG_MSG(logger_level_info, DEBUG_TEXT_FORMAT("execute player action:%d"), type);
    if (type == player_action_type_start_game)
    {
        m_game_round_object.reset_round_data();
    }
    return m_action_object.click_button(type);
}

bool hlddz_game_data_service_api_impl::select_hand_cards(const card_ui_value& card_selected)
{
    if (card_selected.empty())
    {
        return false;
    }

    card_list myself_view_card_list;
    m_game_round_object.get_myself_view_hand_cards(myself_view_card_list);
    if (myself_view_card_list.empty())
    {
        return false;
    }

    auto current_view_count = myself_view_card_list.size();

    auto list = game_string_utils::split(card_selected, CARD_UI_VALUE_SPLIT_CHAR);
    if (list.empty())
    {
        return false;
    }

    card_list ready_to_give;
    for (auto&& item : list)
    {
        auto value = m_game_round_object.convert_ui_value_to_card_value(item);
        auto found = std::find_if(myself_view_card_list.begin(), myself_view_card_list.end(), [value](auto&& current_item) {
            return ((current_item.m_card_value == value) ? true : false);
        });

        if (found != std::end(myself_view_card_list))
        {
            ready_to_give.insert(ready_to_give.end(), *found);
            myself_view_card_list.erase(found);
        }
        else
        {
            DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT("no found the card:%s from view hand cards"), item.c_str());
        }
    }
    if (!ready_to_give.empty())
    {
        return m_action_object.select_cards(ready_to_give, current_view_count);
    }
    else
    {
        return false;
    }
}

void hlddz_game_data_service_api_impl::notify_player_action(gdps_uint8_t view_chair_pos, uint32_t time_left)
{
    if (view_chair_pos == role_position_myself)
    {
        // myself turn.
        auto current_game_status = get_current_game_status();
        if (current_game_status == game_status_type_started)
        {
            set_current_game_status(game_status_type_bidding);
        }
        if (current_game_status == game_status_type_bidden)
        {
            set_current_game_status(game_status_type_multiuping);
        }
        if ((current_game_status == game_status_type_multiuped) || (current_game_status == game_status_type_given))
        {
            set_current_game_status(game_status_type_giving);
        }
    }
}

void hlddz_game_data_service_api_impl::notify_round_data(round_data_notify_type notify_type, void* extra_info)
{
    switch (notify_type)
    {
    case round_data_notify_type_receive_myself_cards:
        set_current_game_status(game_status_type_started);
        break;
    case round_data_notify_type_receive_bottom_cards:
        set_current_game_status(game_status_type_bidden);
        break;
    case round_data_notify_type_receive_given_cards:
        if (m_game_round_object.get_last_given_role_pos() == role_position_myself)
        {
            set_current_game_status(game_status_type_given);
        }
        break;
    case round_data_notify_type_game_over:
        set_current_game_status(game_status_type_over);

        // reset the previous round data.
        // m_game_round_object.reset_round_data();
        break;
    }
}

void hlddz_game_data_service_api_impl::set_current_game_status(game_status_type current_game_status_type)
{
    m_current_game_status = current_game_status_type;
    if ((m_current_game_status == game_status_type_bidding) ||
        (m_current_game_status == game_status_type_multiuping) ||
        (m_current_game_status == game_status_type_giving))
    {
        if (m_request_action_notify_callback)
        {
            DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_DATA_SVC_API_TEXT("requesting action:%d.")), m_current_game_status);
            m_request_action_notify_callback(m_current_game_status, nullptr);
        }
    }
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_DATA_SVC_API_TEXT("change game status to:%d.")), m_current_game_status);
}

void hlddz_game_data_service_api_impl::change_game_status(player_action_type current_action_type)
{
    switch (current_action_type)
    {
    case player_action_type_invalid:
        set_current_game_status_by_button_clicked(game_status_type_invalid);
        break;
    case player_action_type_start_game:
        set_current_game_status_by_button_clicked(game_status_type_starting);
        break;
    case player_action_type_do_not_rob_landlord:
    case player_action_type_rob_landlord:
    case player_action_type_not_being_landlord:
    case player_action_type_being_landlord:
        set_current_game_status_by_button_clicked(game_status_type_bidding);
        break;
    case player_action_type_do_double:
    case player_action_type_do_not_double:
    case player_action_type_do_super_double:
        set_current_game_status_by_button_clicked(game_status_type_multiuped);
        break;
    case player_action_type_do_not_give:
    case player_action_type_do_give:
        set_current_game_status_by_button_clicked(game_status_type_giving);
        break;
    }
}

void hlddz_game_data_service_api_impl::set_current_game_status_by_button_clicked(game_status_type current_game_status_type)
{
    m_current_game_status = current_game_status_type;
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(HLDDZ_GAME_DATA_SVC_API_TEXT("button clicked changes game status to:%d.")), m_current_game_status);
}

game_status_type hlddz_game_data_service_api_impl::get_current_game_status()
{
    return m_current_game_status;
}

GDPS_NAMESPACE_END
