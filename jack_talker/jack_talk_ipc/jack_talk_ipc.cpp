#include "zmq.h"
#include "string"
#include "logger/logger.h"

#define JACK_TALK_IPC_TEXT(X) ("[JACK_TALK_IPC]"##X)

using string_t = std::string;

class zmq_ipc
{
private:
    zmq_ipc() = default;
    ~zmq_ipc() = default;

public:
    static zmq_ipc& getinstance();
    bool initialize();
    void deinitialize();
    bool start_ipc_svc();
    void stop_ipc_svc();
    string_t recv_messages();
    bool send_messages(const string_t& sent_content);

private:
    void* m_context = nullptr;
    void* m_socket = nullptr;
    bool m_bexit = false;
};

zmq_ipc& zmq_ipc::getinstance()
{
    static zmq_ipc m;
    return m;
}

bool zmq_ipc::initialize()
{
    m_context = zmq_init(1);    //指定zmq 处理I/0事件的thread pool 为1
    m_socket = zmq_socket(m_context, ZMQ_REP);
    auto ret = zmq_bind(m_socket, "tcp://*:5555");    // accept connections on a socket
    if (ret != 0)
    {
        DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(JACK_TALK_IPC_TEXT("jack talk ipc bind failed.")));
        return false;
    }
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(JACK_TALK_IPC_TEXT("jack talk ipc initialized.")));
    return true;
}

void zmq_ipc::deinitialize()
{
    zmq_close(m_socket);
    zmq_term(m_context);
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(JACK_TALK_IPC_TEXT("jack talk ipc uninitialized.")));
}

bool zmq_ipc::start_ipc_svc()
{
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(JACK_TALK_IPC_TEXT("jack talk ipc starting...")));
    return true;
}

void zmq_ipc::stop_ipc_svc()
{
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(JACK_TALK_IPC_TEXT("jack talk ipc stopped.")));
    return;
}

string_t zmq_ipc::recv_messages()
{
    string_t recv_content;
    if (m_bexit)
    {
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(JACK_TALK_IPC_TEXT("jack talk ipc exited.")));
        return recv_content;
    }
    zmq_msg_t recv_msg;
    zmq_msg_init(&recv_msg);
    auto result = zmq_msg_recv(&recv_msg, m_socket, 0);
    if (result > 0)
    {
        auto recved = (string_t::value_type*)zmq_msg_data(&recv_msg);
        if (recved)
        {
            recv_content = recved;
            DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(JACK_TALK_IPC_TEXT("recv message:%s.")), recv_content.c_str());
        }
    }
    else
    {
        auto err_reason = zmq_errno();
        auto err_string = zmq_strerror(err_reason);
        if (err_string)
        {
            DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(JACK_TALK_IPC_TEXT("recv message failed:%s.")), err_string);
        }
    }
    zmq_msg_close(&recv_msg);
    return recv_content;
}

bool zmq_ipc::send_messages(const string_t& sent_content)
{
    if (sent_content.empty())
    {
        return false;
    }

    auto msg_len = (sent_content.size() + 1) * sizeof(string_t::value_type);

    zmq_msg_t send_msg;
    zmq_msg_init_size(&send_msg, msg_len);
    auto buffer_msg = (string_t::value_type*)zmq_msg_data(&send_msg);
    memcpy(buffer_msg, sent_content.c_str(), msg_len);
    if (msg_len != zmq_msg_send(&send_msg, m_socket, 0))
    {
        DEBUG_MSG(logger_level_error, DEBUG_TEXT_FORMAT(JACK_TALK_IPC_TEXT("send data failed.")));
    }
    else
    {
        DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT(JACK_TALK_IPC_TEXT("send message:%s.")), sent_content.c_str());
    }
    zmq_msg_close(&send_msg);
    return true;
}

#include <pybind11/pybind11.h>
namespace py = pybind11;

bool initialize_ipc_svc()
{
    return zmq_ipc::getinstance().initialize();
}

void deinitialize_ipc_svc()
{
    zmq_ipc::getinstance().deinitialize();
}

bool start_ipc_svc()
{
    return zmq_ipc::getinstance().start_ipc_svc();
}

void stop_ipc_svc()
{
    zmq_ipc::getinstance().stop_ipc_svc();
}

string_t recv_messages()
{
    return zmq_ipc::getinstance().recv_messages();
}

bool send_messages(const string_t& sent_content)
{
    return zmq_ipc::getinstance().send_messages(sent_content);
}

PYBIND11_MODULE(jack_talk_ipc, m)
{
    m.doc() = "here is a module for jack talk ipc svc.";
    m.def("initialize_jack_talk_ipc_svc", initialize_ipc_svc);
    m.def("deinitialize_jack_talk_ipc_svc", deinitialize_ipc_svc);
    m.def("start_jack_talk_ipc_svc", start_ipc_svc);
    m.def("stop_jack_talk_ipc_svc", stop_ipc_svc);
    m.def("recv_talk_message", recv_messages);
    m.def("send_talk_message", send_messages);
}
