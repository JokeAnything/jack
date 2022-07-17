#ifndef _HLDDZ_GAME_ROUND_IMPL_H
#define _HLDDZ_GAME_ROUND_IMPL_H
#include "../game_data_provider_service_defs.h"
#include "data_service_game_api.h"

#include <map>
#include <mutex>
#include <array>

GDPS_NAMESPACE_BEGIN

enum round_data_notify_type
{
    round_data_notify_type_invalid = 0,
    round_data_notify_type_receive_myself_cards,
    round_data_notify_type_receive_bottom_cards,
    round_data_notify_type_receive_given_cards,
    round_data_notify_type_receive_myself_cards_reset,
    round_data_notify_type_game_over,
};

using round_data_notify_callback = std::function<void(round_data_notify_type notify_type, void* extra_info)>;

class hlddz_game_round_impl
{
public:

    hlddz_game_round_impl() = default;
    ~hlddz_game_round_impl() = default;

    gdps_bool initialize();
    gdps_void uninitialize();

    bool register_round_data_notify(const round_data_notify_callback& notify_callback);
    void unregister_round_data_notify();

public:

    void reset_round_data();
    void get_myself_handcards(card_list& myself_card_list);
    role_position get_landlord_position();
    void get_bottom_cards(card_list& bottom_card_list);
    void get_myself_view_hand_cards(card_list& myself_view_card_list);
    role_position get_last_given_role_pos();
    void get_last_given_cards(card_list& given_card_list, role_position& who_did_role_pos, uint32_t& left_count);
    gdps_string get_card_list_string(const card_list& list);
    gdps_uint8_t convert_ui_value_to_card_value(const card_ui_value& ui_value);
    card_ui_value convert_card_to_ui_card(const card_item& item);
    void get_selected_card_list(card_list& list);

public:

    void parse_myself_handcards(void* myself_handcard_base);
    void parse_current_given_cards();
    void parse_bottom_cards(void* card_set_base);
    void notify_user_select_card_change(void* notify_this);

private:

    void parse_selected_cards(card_list& list);
    bool get_card_item(void* pcard, card_item& item);
    bool get_card_set_list(void* pcard_set, card_list& list);

public:

    static hlddz_game_round_impl* s_myself_this;

private:

    // hook data
    static void* s_notify_give_proc_original;
    static gdps_uint8_t* s_notify_give_proc_original_code_ptr;

    static void* s_game_started_proc_original;
    static gdps_uint8_t* s_game_started_proc_original_code_ptr;

    static void* s_game_bottom_cards_notify_proc_original;
    static gdps_uint8_t* s_game_bottom_cards_notify_proc_original_code_ptr;

    static void* hlddz_game_round_impl::s_user_select_card_change_notify_proc_original;
    static gdps_uint8_t* hlddz_game_round_impl::s_user_select_card_change_notify_proc_original_code_ptr;

    // round data
    role_position m_current_turning_role = role_position_invalid;
    card_list m_myself_handcards;
    card_list m_myself_ui_view_handcards;
    card_list m_bottom_cards;
    card_list m_last_given_cards;
    role_position m_last_given_pos = role_position_invalid;
    role_position m_landlord = role_position_invalid;
    std::array<uint32_t, 4> m_player_hand_card_count = { FARMER_PLAYER_MAX_CARD_COUNT, FARMER_PLAYER_MAX_CARD_COUNT, FARMER_PLAYER_MAX_CARD_COUNT, 0 };
    given_history_list m_given_history;

    round_data_notify_callback m_round_data_notify_proc;
};

GDPS_NAMESPACE_END

#endif //_HLDDZ_GAME_ROUND_IMPL_H
