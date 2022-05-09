#include "hlddz_game_data_service_api_impl.h"
#include <Tlhelp32.h>
#include "logger\logger.h"

GDPS_NAMESPACE_BEGIN

gdps_bool hlddz_game_data_service_impl::initialize()
{
    if (!m_action_object.initialize())
    {
        return false;
    }
    if (!m_game_round_object.initialize())
    {
        return false;
    }
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("hlddz game data service initialized."));
    return true;
}

gdps_void hlddz_game_data_service_impl::uninitialize()
{
    m_game_round_object.uninitialize();
    m_action_object.uninitialize();
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("hlddz game data service deinitialized."));
}

bool hlddz_game_data_service_impl::get_current_turning_role(role_position& pos)
{
    m_game_round_object.get_current_turning(pos);
    //pos = role_position_invalid;
    //DEBUG_MSG(logger_level_info, DEBUG_TEXT_FORMAT("current turn pos:%d."), pos);
    return false;
}

bool hlddz_game_data_service_impl::get_role_card_number(role_position role_id, card_number& number)
{
    DEBUG_MSG(logger_level_info, DEBUG_TEXT_FORMAT("pos:%d.car number:%d"), role_id, number);
    return false;
}

bool hlddz_game_data_service_impl::get_role_hand_cards(role_position role_id, card_list& list)
{
    return false;
}

bool hlddz_game_data_service_impl::get_role_given_cards(role_position role_id, card_list& list)
{
    return false;
}

bool hlddz_game_data_service_impl::execute_current_player_action(player_action_type type)
{
    DEBUG_MSG(logger_level_info, DEBUG_TEXT_FORMAT("execute player action:%d"), type);
    return m_action_object.click_button(type);
}

GDPS_NAMESPACE_END
