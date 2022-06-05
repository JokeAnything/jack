#ifndef _DATA_IPC_SERVICE_IMPL_H
#define _DATA_IPC_SERVICE_IMPL_H

#include "data_ipc_defs.h"
#include "data_ipc_service_api.h"
#include <string>
#include <functional>
#include "zmq.h"
#include <thread>
#include <atomic>
#include <condition_variable>

using ipc_data_notify_callback = std::function<void(const data_message_string& recv_data)>;

class data_ipc_service_impl : public data_ipc_service_api
{
public:
    explicit data_ipc_service_impl(const connect_config& con_cfg);
    ~data_ipc_service_impl() = default;

private:

    bool initialize() override;
    void uninitilize() override;

    void register_data_recv_notify(const ipc_data_notify_callback& callback) override;
    void unregister_data_recv_notify() override;

    bool send_data_message(const data_message_string& msg) override;

private:

    void recv_msg_proc();

private:

    ipc_data_notify_callback m_notify_callback = nullptr;
    void* m_context = nullptr;
    void* m_socket = nullptr;
    std::shared_ptr<std::thread> m_recv_thread_ptr = nullptr;
    std::atomic_bool m_exit = false;
    connect_config m_connect_cfg;

    std::condition_variable m_condition_variable;
    std::mutex m_condition_variable_mutex;
};

#endif //_DATA_IPC_SERVICE_IMPL_H

