#ifndef _DATA_SERVICE_GAME_API_H
#define _DATA_SERVICE_GAME_API_H

#include "game_data_provider\game_data_provider_service_defs.h"
#include <memory>

GDPS_NAMESPACE_BEGIN

enum player_action_type
{
    player_action_type_invalid = 0,
    player_action_type_start_game = 1,
    player_action_type_do_not_rob_landlord = 2,
    player_action_type_rob_landlord = 3,
    player_action_type_not_being_landlord = 4,
    player_action_type_being_landlord = 5,
    player_action_type_do_double = 6,
    player_action_type_do_not_double = 7,
    player_action_type_do_not_give = 8,
    player_action_type_do_give = 9,
};

class data_service_game_api
{
public:

    virtual ~data_service_game_api() {}
    virtual bool get_current_turning_role(role_position& pos) = 0;
    virtual bool get_role_card_number(role_position role_id, card_number& number) = 0;
    virtual bool get_role_hand_cards(role_position role_id, card_list& list) = 0;
    virtual bool get_role_given_cards(role_position role_id, card_list& list) = 0;
    virtual bool execute_current_player_action(player_action_type type) = 0;
};

using data_service_game_api_ptr = std::shared_ptr<data_service_game_api>;

GDPS_NAMESPACE_END

#endif //_DATA_SERVICE_OBJECT_H

