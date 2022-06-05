#include "data_ipc_service_impl.h"
#include "logger/logger.h"

#define DATA_IPC_SERVICE_TEXT(X) ("[DATA_IPC_SERVICE]"##X)

data_ipc_service_impl::data_ipc_service_impl(const connect_config& con_cfg)
{
    m_connect_cfg = con_cfg;
}

bool data_ipc_service_impl::initialize()
{
    if (m_connect_cfg.empty())
    {
        return false;
    }
    m_context = zmq_init(1);
    m_socket = zmq_socket(m_context, ZMQ_REQ);
    auto res = zmq_connect(m_socket, m_connect_cfg.c_str());
    if (res == 0)
    {
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(DATA_IPC_SERVICE_TEXT("connect server successfully.")));
    }
    else
    {
        DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(DATA_IPC_SERVICE_TEXT("connect server failed.")));
        return false;
    }
    if (!m_recv_thread_ptr)
    {
        m_recv_thread_ptr = std::make_shared<std::thread>(&data_ipc_service_impl::recv_msg_proc, this);
    }
    return true;
}

void data_ipc_service_impl::uninitilize()
{
    {
        std::unique_lock<std::mutex> lk(m_condition_variable_mutex);
        m_condition_variable.notify_all();
    }

    m_exit = true;
    if (m_recv_thread_ptr)
    {
        if (m_recv_thread_ptr->joinable())
        {
            m_recv_thread_ptr->join();
        }
        m_recv_thread_ptr = nullptr;
    }
    zmq_close(m_socket);
    zmq_term(m_context);
}

void data_ipc_service_impl::register_data_recv_notify(const ipc_data_notify_callback& callback)
{
    m_notify_callback = callback;
}

void data_ipc_service_impl::unregister_data_recv_notify()
{
    m_notify_callback = nullptr;
}

bool data_ipc_service_impl::send_data_message(const data_message_string& msg)
{
    if (msg.empty())
    {
        return false;
    }
    auto msg_len = (msg.size() + 1) * sizeof(data_message_string::value_type);
    zmq_msg_t send_msg;
    zmq_msg_init_size(&send_msg, msg_len);
    memcpy(zmq_msg_data(&send_msg), msg.c_str(), msg_len);
    if (msg_len != zmq_msg_send(&send_msg, m_socket, 0))
    {
        DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(DATA_IPC_SERVICE_TEXT("send data failed.")));
    }
    else
    {
        std::unique_lock<std::mutex> lk(m_condition_variable_mutex);
        m_condition_variable.notify_one();
    }
    zmq_msg_close(&send_msg);

    return true;
}

void data_ipc_service_impl::recv_msg_proc()
{
    while (1)
    {
        std::unique_lock<std::mutex> lk(m_condition_variable_mutex);
        m_condition_variable.wait(lk);
        if (m_exit)
        {
            break;
        }

        zmq_msg_t recv_msg;
        zmq_msg_init(&recv_msg);
        auto res = zmq_msg_recv(&recv_msg, m_socket, 0);
        if (res > 0)
        {
            auto msg_content = (data_message_string::value_type*)zmq_msg_data(&recv_msg);
            if ((msg_content != nullptr) && m_notify_callback)
            {
                m_notify_callback(msg_content);
            }
        }
        else
        {
            auto err_reason = zmq_errno();
            auto err_string = zmq_strerror(err_reason);
            if (err_string)
            {
                DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(DATA_IPC_SERVICE_TEXT("recv error:%s.")), err_string);
            }
        }
        zmq_msg_close(&recv_msg);
    }
}
