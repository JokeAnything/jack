#ifndef _HLDDZ_GAME_ROUND_IMPL_H
#define _HLDDZ_GAME_ROUND_IMPL_H
#include "../game_data_provider_service_defs.h"
#include "../../data_service_game_api.h"

#include <map>
#include <mutex>

GDPS_NAMESPACE_BEGIN

class hlddz_game_round_impl
{
public:

    hlddz_game_round_impl() = default;
    ~hlddz_game_round_impl() = default;

    gdps_bool initialize();
    gdps_void uninitialize();

public:

    bool get_current_turning(role_position& pos);
    //void update_current_turning_given(void* object);
    void update_player_timer_info(gdps_uint8_t view_chair_pos, uint32_t time_left);
    void parse_myself_handcards(void* myself_handcard_base);
    void parse_current_given_cards();
    void parse_bottom_cards(void* card_set_base);

private:

    bool get_card_item(void* pcard, card_item& item);
    bool get_card_set_list(void* pcard_set, card_list& list);
    gdps_string get_card_list_string(const card_list& list);

public:

    static hlddz_game_round_impl* s_myself_this;

private:

    static void* s_notify_give_proc_original;
    static gdps_uint8_t* s_notify_give_proc_original_code_ptr;

    static void* s_timer_update_proc_original;
    static gdps_uint8_t* s_timer_update_proc_original_code_ptr;

    static void* s_game_started_proc_original;
    static gdps_uint8_t* s_game_started_proc_original_code_ptr;

    static void* s_game_bottom_cards_notify_proc_original;
    static gdps_uint8_t* s_game_bottom_cards_notify_proc_original_code_ptr;

    game_status_type m_current_game_status = game_status_type_invalid;

    role_position m_myself_role = role_position_invalid;

    role_position m_current_turning_role = role_position_invalid;

    card_list m_myself_handchads;

    given_history_list m_given_history;
};

GDPS_NAMESPACE_END

#endif //_HLDDZ_GAME_ROUND_IMPL_H
