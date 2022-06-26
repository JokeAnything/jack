#ifndef _DOUZERO_SERVICE_PROXY_H
#define _DOUZERO_SERVICE_PROXY_H
#include "../game_data_provider/game_data_provider_service_defs.h"
#include "ai_service_object.h"
#include "../game_data_provider/hlddz/data_service_object.h"
#include "../ipc/data_ipc_service_api.h"
#include "../ipc/data_ipc_msg_utils.h"

#include <thread>
#include <atomic>

#define RPC_FUNCTION_NAME(X)    ("rpc_function_"##X)
#define IPC_FUNCTION_CREATE_GAME_SESSION RPC_FUNCTION_NAME("create_game_session")

#define IPC_FUNCTION_GAME_STATUS_TYPE_STARTING RPC_FUNCTION_NAME("game_status_type_starting")
#define IPC_FUNCTION_GAME_STATUS_TYPE_STARTED RPC_FUNCTION_NAME("game_status_type_started")

#define IPC_FUNCTION_GAME_STATUS_TYPE_BIDDING RPC_FUNCTION_NAME("game_status_type_bidding")
#define IPC_FUNCTION_GAME_STATUS_TYPE_ROBBING RPC_FUNCTION_NAME("game_status_type_robbing")

#define IPC_FUNCTION_GAME_STATUS_TYPE_BIDDEN RPC_FUNCTION_NAME("game_status_type_bidden")
#define IPC_FUNCTION_GAME_STATUS_TYPE_NO_ONE_BIDDING RPC_FUNCTION_NAME("game_status_type_no_one_bidding")

#define IPC_FUNCTION_GAME_STATUS_TYPE_MULTIUPING RPC_FUNCTION_NAME("game_status_type_multiuping")
#define IPC_FUNCTION_GAME_STATUS_TYPE_MULTIUPED RPC_FUNCTION_NAME("game_status_type_multiuped")

#define IPC_FUNCTION_GAME_STATUS_TYPE_GIVING RPC_FUNCTION_NAME("game_status_type_giving")
#define IPC_FUNCTION_GAME_STATUS_TYPE_GIVEN RPC_FUNCTION_NAME("game_status_type_given")
#define IPC_FUNCTION_GAME_STATUS_TYPE_OVER RPC_FUNCTION_NAME("game_status_type_over")

using namespace GDPS;

class douzero_service_proxy : public ai_service_object
{
public:

    douzero_service_proxy();
    ~douzero_service_proxy();

private:

    bool initialize() override;
    void uninitialize() override;
    bool start_ai_service() override;
    void stop_ai_service() override;
    void reset_game() override;
    data_service_game_api_ptr get_data_service_game_api() override;
    void notify_recv_ipc_data(const data_message_string& recv_data);
    void notify_game_action(game_status_type action_type, void* extra_info);

    void request_start_game_ssid();

private:

    void respond_game_session_created(const ssid& game_ssid, const return_list& ret_list);
    void respond_game_bidding(const ssid& game_ssid, const return_list& ret_list);
    void respond_game_multiuping(const ssid& game_ssid, const return_list& ret_list);
    void respond_game_robbing(const ssid& game_ssid, const return_list& ret_list);
    void respond_game_giving(const ssid& game_ssid, const return_list& ret_list);

    void request_game_started();
    void request_game_bidding();
    void request_game_robbing();
    void request_game_bidden();
    void request_game_no_one_bidding();
    void request_game_multiuping();
    void request_game_giving();
    void request_game_given();
    void request_game_over();
    parameter_list format_card_list_parameter(const card_list& list);

private:

    data_ipc_service_api_ptr m_data_ipc_service_api_ptr;
    data_service_object_ptr m_game_data_svc_object = nullptr;
    ssid m_cur_ssid;
};

#endif //_DOUZERO_SERVICE_PROXY_H
