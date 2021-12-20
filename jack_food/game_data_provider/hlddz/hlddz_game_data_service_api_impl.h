#ifndef _HLDDZ_GAME_DATA_SERVICE_API_IMPL_H
#define _HLDDZ_GAME_DATA_SERVICE_API_IMPL_H
#include "../game_data_provider_service_defs.h"
#include <windows.h>

GDPS_NAMESPACE_BEGIN
class hlddz_game_data_service_impl
{
public:

    hlddz_game_data_service_impl() = default;
    ~hlddz_game_data_service_impl() = default;
    gdps_bool initialize();
    gdps_void uninitialize();
    auto get_current_turning_role();
    auto get_role_card_number(role_position role_id);
    auto get_role_hand_cards(role_position role_id);
    auto get_role_given_cards(role_position role_id);

private:

    gdps_uint32_ptr_t m_main_module_base_addr = nullptr;
    HANDLE m_main_processs_handle = NULL;
};

GDPS_NAMESPACE_END

#endif //_HLDDZ_GAME_DATA_SERVICE_API_IMPL_H

