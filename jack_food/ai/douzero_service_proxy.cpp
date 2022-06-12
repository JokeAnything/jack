#include "douzero_service_proxy.h"
#include "../game_data_provider/hlddz/data_service_object.h"
#include "../ipc/data_ipc_factory.h"
#include "../game_data_provider/hlddz/data_service_factory.h"
#include "logger\logger.h"

#define DOUZERO_SERVICE_PROXY_TEXT(X) ("[DOUZERO_SERVICE_PROXY]"##X)

douzero_service_proxy::douzero_service_proxy()
{
    m_game_data_svc_object = data_service_factory::create_data_service_object(data_service_type_hlddz);
    m_data_ipc_service_api_ptr = data_ipc_factory::create_data_ipc_object(data_ipc_type_hlddz);
}

douzero_service_proxy::~douzero_service_proxy()
{
    uninitialize();
    data_ipc_factory::destroy_data_ipc_object(m_data_ipc_service_api_ptr);
}

gdps_bool douzero_service_proxy::initialize()
{
    if (!m_data_ipc_service_api_ptr->initialize())
    {
        return false;
    }

    if (m_game_data_svc_object)
    {
        if (!m_game_data_svc_object->initialize())
        {
            m_data_ipc_service_api_ptr->uninitilize();
            return false;
        }
    }
    m_data_ipc_service_api_ptr->register_data_recv_notify(std::bind(&douzero_service_proxy::notify_recv_ipc_data,
        this,
        std::placeholders::_1
    ));

    auto game_object_api = get_data_service_game_api();
    game_object_api->register_action_notify(std::bind(&douzero_service_proxy::notify_game_action,this,
        std::placeholders::_1,
        std::placeholders::_2));

    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("douzero_service_proxy initialized.")));
    return true;
}

gdps_void douzero_service_proxy::uninitialize()
{
    auto game_object_api = get_data_service_game_api();
    game_object_api->unregister_action_notify();
    if (m_game_data_svc_object != nullptr)
    {
        m_game_data_svc_object->uninitialize();
        m_game_data_svc_object = nullptr;
    }

    m_data_ipc_service_api_ptr->unregister_data_recv_notify();
    m_data_ipc_service_api_ptr->uninitilize();

}

gdps_bool douzero_service_proxy::start_ai_service()
{
    m_game_data_svc_object->start_data_service();
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("game ai service starting...")));
    return true;
}

gdps_void douzero_service_proxy::stop_ai_service()
{
    m_game_data_svc_object->stop_data_service();
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("game ai service stopped.")));
}

void douzero_service_proxy::reset_game()
{
    request_start_game_ssid();
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("game ai service reset game.")));
}

data_service_game_api_ptr douzero_service_proxy::get_data_service_game_api()
{
    if (m_game_data_svc_object != nullptr)
    {
        return m_game_data_svc_object->get_data_service_game_api();
    }
    else
    {
        return nullptr;
    }
}

void douzero_service_proxy::notify_recv_ipc_data(const data_message_string& recv_data)
{
    if (recv_data.empty())
    {
        return;
    }

    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("recv message:%s.")), recv_data.c_str());

    ssid cur_ssid;
    function_name function;
    return_list ret_list;
    auto res = data_ipc_msg_utils::parse_respond_msg(recv_data, cur_ssid, function, ret_list);
    if (!res)
    {
        return;
    }
    if (function == IPC_FUNCTION_CREATE_GAME_SESSION)
    {
        respond_game_session_created(cur_ssid, ret_list);
    }
    else if (function == IPC_FUNCTION_GAME_STATUS_TYPE_BIDDING)
    {
        respond_game_bidding(cur_ssid, ret_list);
    }
    else if (function == IPC_FUNCTION_GAME_STATUS_TYPE_MULTIUPING)
    {
        respond_game_multiuping(cur_ssid, ret_list);
    }
    else if (function == IPC_FUNCTION_GAME_STATUS_TYPE_GIVING)
    {
        respond_game_giving(cur_ssid, ret_list);
    }
}

void douzero_service_proxy::respond_game_session_created(const ssid& game_ssid, const return_list& ret_list)
{
    if (!game_ssid.empty())
    {
        m_cur_ssid = game_ssid;
    }
    if (!m_cur_ssid.empty())
    {
        auto game_object_api = get_data_service_game_api();
        if (game_object_api)
        {
            game_object_api->execute_current_player_action(player_action_type_start_game);
        }
    }
}

void douzero_service_proxy::respond_game_bidding(const ssid& game_ssid, const return_list& ret_list)
{
    if (game_ssid.empty())
    {
        return;
    }
    if (ret_list.empty())
    {
        return;
    }
    auto type = ret_list[0];
    auto value = std::strtoul(type.c_str(), nullptr, 10);
    player_action_type action_type = player_action_type_invalid;
    if (value)
    {
        action_type = player_action_type_being_landlord;
    }
    else
    {
        action_type = player_action_type_not_being_landlord;
    }
    auto game_object_api = get_data_service_game_api();
    if (game_object_api)
    {
        game_object_api->execute_current_player_action(action_type);
    }
}

void douzero_service_proxy::respond_game_multiuping(const ssid& game_ssid, const return_list& ret_list)
{
    if (game_ssid.empty())
    {
        return;
    }
    player_action_type action_type = player_action_type_invalid;

    if (ret_list.empty())
    {
        action_type = player_action_type_do_not_double;
    }
    else
    {
        auto type = ret_list[0];
        auto value = !!std::strtoul(type.c_str(), nullptr, 10);
        DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("respond double status:%d.")), value);
        if (value)
        {
            action_type = player_action_type_do_double;
        }
        else
        {
            action_type = player_action_type_do_not_double;
        }
    }

    auto game_object_api = get_data_service_game_api();
    if (game_object_api)
    {
        game_object_api->execute_current_player_action(action_type);
    }
}

void douzero_service_proxy::respond_game_giving(const ssid& game_ssid, const return_list& ret_list)
{
    if (game_ssid.empty())
    {
        return;
    }

    card_ui_value card_giving;
    for (const auto& item : ret_list)
    {
        if (card_giving.empty())
        {
            card_giving = item;
        }
        else
        {
            card_giving += CARD_UI_VALUE_SPLIT_CHAR;
            card_giving += item;
        }
    }

    DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("respond giving cards:%s.")), card_giving.c_str());

    auto game_object_api = get_data_service_game_api();
    if (game_object_api)
    {
        auto action_type = player_action_type_do_not_give;
        if (!card_giving.empty())
        {
            game_object_api->select_hand_cards(card_giving);
            action_type = player_action_type_do_give;
        }
        game_object_api->execute_current_player_action(action_type);
    }
}

void douzero_service_proxy::notify_game_action(game_status_type action_type, void* extra_info)
{
    switch (action_type)
    {
    case game_status_type_started:
        request_game_started();
        break;
    case game_status_type_bidding:
        request_game_bidding();
        break;
    case game_status_type_robbing:
        request_game_bidding();
        break;
    case game_status_type_bidden:
        request_game_bidden();
        break;
    case game_status_type_no_one_bidding:
        request_game_no_one_bidding();
        break;
    case game_status_type_multiuping:
        request_game_multiuping();
        break;
    case game_status_type_giving:
        request_game_giving();
        break;
    case game_status_type_given:
        request_game_given();
        break;
    case game_status_type_over:
        request_game_over();
        break;
    }
}

void douzero_service_proxy::request_start_game_ssid()
{
    auto req_msg = data_ipc_msg_utils::make_request_msg("", IPC_FUNCTION_CREATE_GAME_SESSION, parameter_list());
    if (m_data_ipc_service_api_ptr)
    {
        m_data_ipc_service_api_ptr->send_data_message(req_msg);
    }
}

void douzero_service_proxy::request_game_started()
{
    auto game_api = get_data_service_game_api();
    if (!game_api)
    {
        return;
    }

    card_list list;
    game_api->get_role_hand_cards(role_position_myself, list);
    if (list.empty())
    {
        DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("get player hand cards failed.")));
        return;
    }

    auto para_list = format_card_list_parameter(list);
    auto req_msg = data_ipc_msg_utils::make_request_msg(m_cur_ssid, IPC_FUNCTION_GAME_STATUS_TYPE_STARTED, para_list);
    if (m_data_ipc_service_api_ptr)
    {
        m_data_ipc_service_api_ptr->send_data_message(req_msg);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("player hand cards sent.")));
    }
}

void douzero_service_proxy::request_game_bidding()
{
    parameter_list para_list;
    auto req_msg = data_ipc_msg_utils::make_request_msg(m_cur_ssid, IPC_FUNCTION_GAME_STATUS_TYPE_BIDDING, para_list);
    if (m_data_ipc_service_api_ptr)
    {        
        m_data_ipc_service_api_ptr->send_data_message(req_msg);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("request ipc_function_game_status_type_bidding.")));
    }
}

void douzero_service_proxy::request_game_robbing()
{
    parameter_list para_list;
    auto req_msg = data_ipc_msg_utils::make_request_msg(m_cur_ssid, IPC_FUNCTION_GAME_STATUS_TYPE_ROBBING, para_list);
    if (m_data_ipc_service_api_ptr)
    {
        m_data_ipc_service_api_ptr->send_data_message(req_msg);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("request ipc_function_game_status_type_robbing.")));
    }
}

void douzero_service_proxy::request_game_bidden()
{
    auto game_api = get_data_service_game_api();
    if (!game_api)
    {
        return;
    }

    card_list bottom_list;
    if (!game_api->get_bottom_cards(bottom_list))
    {
        return;
    }
    auto landlord_pos = game_api->get_landlord_position();

    parameter_list para_list_pos;
    para_list_pos.push_back(std::to_string(landlord_pos));
    parameter_list para_list_bottom_cards = format_card_list_parameter(bottom_list);
    para_list_pos.insert(para_list_pos.end(), para_list_bottom_cards.begin(), para_list_bottom_cards.end());

    auto req_msg = data_ipc_msg_utils::make_request_msg(m_cur_ssid, IPC_FUNCTION_GAME_STATUS_TYPE_BIDDEN, para_list_pos);
    if (m_data_ipc_service_api_ptr)
    {        
        m_data_ipc_service_api_ptr->send_data_message(req_msg);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("request ipc_function_game_status_type_bidden.")));
    }
}

void douzero_service_proxy::request_game_no_one_bidding()
{
    parameter_list para_list;
    auto req_msg = data_ipc_msg_utils::make_request_msg(m_cur_ssid, IPC_FUNCTION_GAME_STATUS_TYPE_NO_ONE_BIDDING, para_list);
    if (m_data_ipc_service_api_ptr)
    {
        m_data_ipc_service_api_ptr->send_data_message(req_msg);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("request ipc_function_game_status_type_no_one_bidding.")));
    }
}

void douzero_service_proxy::request_game_multiuping()
{
    parameter_list para_list;
    auto req_msg = data_ipc_msg_utils::make_request_msg(m_cur_ssid, IPC_FUNCTION_GAME_STATUS_TYPE_MULTIUPING, para_list);
    if (m_data_ipc_service_api_ptr)
    {
        m_data_ipc_service_api_ptr->send_data_message(req_msg);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("request ipc_function_game_status_type_multiuping.")));
    }
}

void douzero_service_proxy::request_game_giving()
{
    parameter_list para_list;
    auto req_msg = data_ipc_msg_utils::make_request_msg(m_cur_ssid, IPC_FUNCTION_GAME_STATUS_TYPE_GIVING, para_list);
    if (m_data_ipc_service_api_ptr)
    {
        m_data_ipc_service_api_ptr->send_data_message(req_msg);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("request ipc_function_game_status_type_giving.")));
    }
}

void douzero_service_proxy::request_game_given()
{
    auto game_api = get_data_service_game_api();
    if (!game_api)
    {
        return;
    }

    auto last_given_pos = game_api->get_last_given_role_position();
    card_list given_card_list;
    game_api->get_last_given_cards(given_card_list);

    parameter_list para_list_pos;
    para_list_pos.push_back(std::to_string(last_given_pos));
    parameter_list para_list_given_cards = format_card_list_parameter(given_card_list);
    para_list_pos.insert(para_list_pos.end(), para_list_given_cards.begin(), para_list_given_cards.end());


    parameter_list para_list;    
    auto req_msg = data_ipc_msg_utils::make_request_msg(m_cur_ssid, IPC_FUNCTION_GAME_STATUS_TYPE_GIVEN, para_list);
    if (m_data_ipc_service_api_ptr)
    {
        m_data_ipc_service_api_ptr->send_data_message(req_msg);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("request ipc_function_game_status_type_given.")));
    }
}

void douzero_service_proxy::request_game_over()
{
    parameter_list para_list;
    auto req_msg = data_ipc_msg_utils::make_request_msg(m_cur_ssid, IPC_FUNCTION_GAME_STATUS_TYPE_OVER, para_list);
    if (m_data_ipc_service_api_ptr)
    {        
        m_data_ipc_service_api_ptr->send_data_message(req_msg);
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DOUZERO_SERVICE_PROXY_TEXT("request ipc_function_game_status_type_over.")));
    }
}

parameter_list douzero_service_proxy::format_card_list_parameter(const card_list& list)
{
    parameter_list para_list;
    if (list.empty())
    {
        return para_list;
    }

    auto game_api = get_data_service_game_api();
    if (!game_api)
    {
        return para_list;
    }

    para_list.emplace_back(std::to_string(list.size()));
    for (auto&& item : list)
    {
        auto ui_value = game_api->convert_card_to_ui_card(item);
        if (!ui_value.empty())
        {
            para_list.emplace_back(ui_value);
        }
    }
    return para_list;
}
