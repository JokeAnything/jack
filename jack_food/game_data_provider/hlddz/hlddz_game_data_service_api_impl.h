#ifndef _HLDDZ_GAME_DATA_SERVICE_API_IMPL_H
#define _HLDDZ_GAME_DATA_SERVICE_API_IMPL_H
#include "../game_data_provider_service_defs.h"
#include "data_service_game_api.h"
#include "hlddz_game_action_impl.h"
#include "hlddz_game_round_impl.h"

#include <windows.h>
#include <array>

GDPS_NAMESPACE_BEGIN

class hlddz_game_data_service_api_impl : public data_service_game_api
{
public:

    hlddz_game_data_service_api_impl() = default;
    ~hlddz_game_data_service_api_impl() = default;
    gdps_bool initialize();
    gdps_void uninitialize();

private:

    bool register_action_notify(const request_action_notify_callback& notify_callback) override;
    void unregister_action_notify() override;
    bool get_current_turning_role(role_position& pos) override;
    role_position get_landlord_position() override;
    bool get_bottom_cards(card_list& list) override;
    void get_selected_cards(card_list& list) override;
    bool get_role_card_number(role_position role_id, card_number& number) override;
    bool get_role_hand_cards(role_position role_id, card_list& list) override;
    role_position get_last_given_role_position() override;
    bool get_last_given_cards(card_list& list) override;
    card_ui_value convert_card_to_ui_card(const card_item& item) override;
    bool select_hand_cards(const card_ui_value& card_selected) override;
    bool execute_current_player_action(player_action_type type) override;

private:

    void change_game_status(player_action_type current_action_type);
    void notify_player_action(gdps_uint8_t view_chair_pos, uint32_t time_left);
    void notify_round_data(round_data_notify_type notify_type, void* extra_info);

    void set_current_game_status(game_status_type current_game_status_type);
    void set_current_game_status_by_button_clicked(game_status_type current_game_status_type);
    game_status_type get_current_game_status();
    void reset_card_select_status();

private:

    hlddz_game_action_impl m_action_object;
    hlddz_game_round_impl m_game_round_object;
    game_status_type m_current_game_status = game_status_type_invalid;
    request_action_notify_callback m_request_action_notify_callback;
};

using hlddz_game_data_service_impl_ptr = std::shared_ptr<hlddz_game_data_service_api_impl>;


GDPS_NAMESPACE_END

#endif //_HLDDZ_GAME_DATA_SERVICE_API_IMPL_H

