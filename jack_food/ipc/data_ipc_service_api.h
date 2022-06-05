#ifndef _DATA_IPC_SERVICE_API_H
#define _DATA_IPC_SERVICE_API_H

#include "data_ipc_defs.h"
#include <string>
#include <functional>
#include <memory>

using ipc_data_notify_callback = std::function<void(const data_message_string& recv_data)>;

class data_ipc_service_api
{

public:

    virtual ~data_ipc_service_api() = default;

    virtual bool initialize() = 0;
    virtual void uninitilize() = 0;

    virtual void register_data_recv_notify(const ipc_data_notify_callback& callback) = 0;
    virtual void unregister_data_recv_notify() = 0;

    virtual bool send_data_message(const data_message_string& msg) = 0;
};

using data_ipc_service_api_ptr = std::shared_ptr<data_ipc_service_api>;

#endif //_DATA_IPC_SERVICE_API_H
