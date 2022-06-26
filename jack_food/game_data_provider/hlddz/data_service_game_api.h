#ifndef _DATA_SERVICE_GAME_API_H
#define _DATA_SERVICE_GAME_API_H

#include "../game_data_provider_service_defs.h"
#include <memory>
#include <functional>

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
    player_action_type_do_mingpai = 10,
    player_action_type_change_oponents = 11,
    player_action_type_do_super_double = 12,
    player_action_type_suggestion = 13,
};

enum game_status_type
{
    game_status_type_invalid = 0,
    game_status_type_starting = 1,
    game_status_type_started = 2,
    game_status_type_bidding = 3,
    game_status_type_ordered = 4,
    game_status_type_robbing = 5,
    game_status_type_bidden = 6,
    game_status_type_no_one_bidding = 7,
    game_status_type_multiuping = 8,
    game_status_type_multiuped = 9,
    game_status_type_giving = 10,
    game_status_type_given = 11,
    game_status_type_over = 12,
};

enum game_status_order_type
{
    game_status_order_type_invalid = 0,
    game_status_order_type_ordering = 1,
    game_status_order_type_robbing = 2,
};

enum role_position
{
    role_position_myself = 0,
    role_position_right = 1,
    role_position_left = 2,
    role_position_invalid
};

struct card_item
{
    gdps_uint8_t m_card_value = 0;
    gdps_uint8_t m_card_shape = 0;
    gdps_uint8_t m_card_level = 0;
    gdps_uint8_t m_card_view_pos_index = 0;
};

#define LANDLORD_PLAYER_MAX_CARD_COUNT      (20)
#define FARMER_PLAYER_MAX_CARD_COUNT        (17)
#define CARD_UI_VALUE_SPLIT_CHAR            (":")

using card_list = std::vector<card_item>;

using given_history_list = std::vector<card_list>;

using card_ui_value = std::string;

using request_action_notify_callback = std::function<void(game_status_type action_type, void* extra_info)>;

class data_service_game_api
{
public:

    virtual ~data_service_game_api() {}
    virtual bool register_action_notify(const request_action_notify_callback& notify_callback) = 0;
    virtual void unregister_action_notify() = 0;
    virtual bool get_current_turning_role(role_position& pos) = 0;
    virtual role_position get_landlord_position() = 0;
    virtual bool get_bottom_cards(card_list& list) = 0;
    virtual bool get_role_card_number(role_position role_id, card_number& number) = 0;
    virtual bool get_role_hand_cards(role_position role_id, card_list& list) = 0;
    virtual role_position get_last_given_role_position() = 0;
    virtual bool get_last_given_cards(card_list& list) = 0;
    virtual card_ui_value convert_card_to_ui_card(const card_item& item) = 0;
    virtual bool select_hand_cards(const card_ui_value& card_selected) = 0;
    virtual bool execute_current_player_action(player_action_type type) = 0;
};

using data_service_game_api_ptr = std::shared_ptr<data_service_game_api>;

GDPS_NAMESPACE_END

#endif //_DATA_SERVICE_OBJECT_H

