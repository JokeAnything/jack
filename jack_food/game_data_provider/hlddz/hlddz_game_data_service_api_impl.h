#ifndef _HLDDZ_GAME_DATA_SERVICE_API_IMPL_H
#define _HLDDZ_GAME_DATA_SERVICE_API_IMPL_H
#include "../game_data_provider_service_defs.h"
#include "../../data_service_game_api.h"
#include "hlddz_game_action_impl.h"
#include "hlddz_game_round_impl.h"

#include <windows.h>

GDPS_NAMESPACE_BEGIN

class hlddz_game_data_service_impl : public data_service_game_api
{
public:

    hlddz_game_data_service_impl() = default;
    ~hlddz_game_data_service_impl() = default;
    gdps_bool initialize();
    gdps_void uninitialize();

private:

    bool get_current_turning_role(role_position& pos) override;
    bool get_role_card_number(role_position role_id, card_number& number) override;
    bool get_role_hand_cards(role_position role_id, card_list& list) override;
    bool get_role_given_cards(role_position role_id, card_list& list) override;
    bool execute_current_player_action(player_action_type type) override;

private:

    hlddz_game_action_impl m_action_object;
    hlddz_game_round_impl m_game_round_object;
};

using hlddz_game_data_service_impl_ptr = std::shared_ptr<hlddz_game_data_service_impl>;


GDPS_NAMESPACE_END

#endif //_HLDDZ_GAME_DATA_SERVICE_API_IMPL_H

