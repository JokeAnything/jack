#ifndef _HLDDZ_GAME_COMMON_API_H
#define _HLDDZ_GAME_COMMON_API_H
#include "../game_data_provider_service_defs.h"
#include "data_service_game_api.h"
#include "../../utils/game_process_utils.h"

GDPS_NAMESPACE_BEGIN

void* get_round_data_mgr_base();
void* get_view_player_object(gdps_uint8_t pos);
gdps_uint8_t get_view_chair_pos(void* view_player_object);

GDPS_NAMESPACE_END

#endif //_HLDDZ_GAME_COMMON_API_H
