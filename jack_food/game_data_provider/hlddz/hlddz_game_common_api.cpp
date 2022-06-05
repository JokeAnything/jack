#include "hlddz_game_common_api.h"
#include "../../utils/game_process_utils.h"
#include "hlddz_data_bin_offset_defs.h"
#include "logger/logger.h"
#include <windows.h>

GDPS_NAMESPACE_BEGIN

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
            mov ecx, data_mgr_base
            call proc_address
            mov view_player_object, eax
            popfd
            popad
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
    return view_player_object;
}

gdps_uint8_t get_view_chair_pos(void* view_player_object)
{
    if (view_player_object == nullptr)
    {
        return role_position_invalid;
    }
    auto view_player_this = (unsigned char*)view_player_object;
    return *(view_player_this + 21);
}

GDPS_NAMESPACE_END
