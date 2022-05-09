#ifndef _HLDDZ_GAME_ACTION_IMPL_H
#define _HLDDZ_GAME_ACTION_IMPL_H
#include "../game_data_provider_service_defs.h"
#include "../../data_service_game_api.h"

#include <map>
#include <mutex>

GDPS_NAMESPACE_BEGIN

using object_this = void*;
using object_name = std::string;
using button_object_node = std::pair<player_action_type, object_this>;

class hlddz_game_action_impl
{
public:

    hlddz_game_action_impl() = default;
    ~hlddz_game_action_impl() = default;

    gdps_bool initialize();
    gdps_void uninitialize();

private:

    bool get_button_object(const object_name& btn_name, object_this object_ptr);

public:

    bool click_button(player_action_type button_type);
    void save_button_object(const object_name::value_type* button_name, object_this object_ptr);

public:

    static hlddz_game_action_impl* s_myself_this;
    static void* s_original_get_button_proc;

private:

    std::map<object_name, button_object_node> m_button_object_list;


    std::mutex data_locker;
};

GDPS_NAMESPACE_END

#endif //_HLDDZ_GAME_ACTION_IMPL_H
