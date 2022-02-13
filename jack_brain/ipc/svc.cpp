#include "zmq.h"
#include "string"
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
private:
    void process_messages(const string_t& recved_msg, string_t& respond_msg);

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
    zmq_bind(m_socket, "tcp://*:5555");    // accept connections on a socket
    return true;
}

void zmq_ipc::deinitialize()
{
    zmq_close(m_socket);
    zmq_term(m_context);
}

bool zmq_ipc::start_ipc_svc()
{
    while (!m_bexit)
    {
        zmq_msg_t recv_msg;
        zmq_msg_init(&recv_msg);
        zmq_msg_recv(&recv_msg, m_socket, 0);
        auto recved = (char*)zmq_msg_data(&recv_msg);
        string_t respond;
        process_messages(recved, respond);
        zmq_msg_close(&recv_msg);

        zmq_msg_t send_msg;
        zmq_msg_init_size(&send_msg, respond.size());
        auto sent_msg = (char*)zmq_msg_data(&send_msg);
        memcpy(sent_msg, respond.c_str(), respond.size());
        zmq_sendmsg(m_socket, &send_msg, 0);
        zmq_msg_close(&send_msg);
    }
    return true;
}

void zmq_ipc::stop_ipc_svc()
{
    m_bexit = true;
}

void zmq_ipc::process_messages(const string_t& recved_msg, string_t& respond_msg)
{
    if (m_bexit)
    {
        return;
    }
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


PYBIND11_MODULE(jack_ipc_svc, m)
{
    m.doc() = "here is a module for jack ipc svc and jack ipc msg.";
    m.def("initialize_ipc_svc", initialize_ipc_svc);
    m.def("deinitialize_ipc_svc", deinitialize_ipc_svc);
    m.def("start_ipc_svc", start_ipc_svc);
    m.def("stop_ipc_svc", stop_ipc_svc);
}


