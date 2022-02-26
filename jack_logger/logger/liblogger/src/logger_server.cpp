#include "logger_server.h"

logger_server& logger_server::getinstance()
{
    static logger_server m;
    return m;
}

bool logger_server::initialize()
{
    m_context = zmq_init(1);    //指定zmq 处理I/0事件的thread pool 为1
    m_socket = zmq_socket(m_context, ZMQ_REP);
    zmq_bind(m_socket, "udp://*:9999");    // accept connections on a socket
    return true;
}

void logger_server::deinitialize()
{
    zmq_close(m_socket);
    zmq_term(m_context);
}

bool logger_server::start_ipc_svc()
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

void logger_server::stop_ipc_svc()
{
    m_bexit = true;
}

void logger_server::process_messages(const string_t& recved_msg, string_t& respond_msg)
{
    if (m_bexit)
    {
        return;
    }
    if (m_message_respond_proc)
    {
        respond_msg = m_message_respond_proc(recved_msg);
    }
}

bool logger_server::register_message_respond_proc(const message_respond_proc proc)
{
    m_message_respond_proc = proc;
    return true;
}
void logger_server::unregister_message_respond_proc()
{
    m_message_respond_proc = nullptr;
}
