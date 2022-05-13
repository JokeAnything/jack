#ifndef _HLDDZ_GAME_ACTION_IMPL_H
#define _HLDDZ_GAME_ACTION_IMPL_H
#include "../game_data_provider_service_defs.h"
#include "../../data_service_game_api.h"
#include "../../utils/game_process_utils.h"

#include <map>
#include <mutex>

GDPS_NAMESPACE_BEGIN

using object_this = void*;
using object_name = std::string;

using player_action_notify_callback = std::function<void>(gdps_uint8_t view_chair_pos, uint32_t time_left);

struct hlddz_button_info
{
    hlddz_button_info()
    {
    }
    hlddz_button_info(player_action_type action_type, object_this object, gdps_bool status)
    {
        m_action_type = action_type;
        m_button_object_this = object;
        m_button_click_available = status;
    };
    player_action_type m_action_type = player_action_type_invalid;
    object_this m_button_object_this = nullptr;
    gdps_bool m_button_click_available = false;
};

using button_object_node = hlddz_button_info;

class hlddz_game_action_impl
{
public:

    hlddz_game_action_impl() = default;
    ~hlddz_game_action_impl() = default;

    gdps_bool initialize();
    gdps_void uninitialize();

public:

    bool click_button(player_action_type button_type);
    void save_button_object(const object_name::value_type* button_name, object_this object_ptr);
    void show_button_object(object_this button_object_this, uint32_t show_status);
    bool select_cards(const card_list& selected_list, uint32_t total_cards);
    void update_player_timer_info(gdps_uint8_t view_chair_pos, uint32_t time_left);

private:

    bool get_button_object(const object_name& btn_name, object_this object_ptr);
    void update_game_status(player_action_type current_action_type);
    void change_game_status(player_action_type current_action_type);

public:

    static hlddz_game_action_impl* s_myself_this;
    static void* s_original_get_button_proc;
    static void* s_original_show_button_status_proc;

    static void* s_timer_update_proc_original;
    static gdps_uint8_t* s_timer_update_proc_original_code_ptr;

private:

    std::map<object_name, button_object_node> m_button_object_list;
    process_wnd m_game_window = nullptr;
    std::mutex data_locker;

    player_action_notify_callback m_notify_callback;
    game_status_type m_current_game_status = game_status_type_invalid;
    role_position m_myself_role = role_position_invalid;
    role_position m_current_turning_role = role_position_invalid;
};

GDPS_NAMESPACE_END

#endif //_HLDDZ_GAME_ACTION_IMPL_H
