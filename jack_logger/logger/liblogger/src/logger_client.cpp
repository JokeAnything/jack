#include "zmq.h"
#include <iostream>
#include "logger_client.h"

logger_client& logger_client::getinstance()
{
    static logger_client m;
    return m;
}

bool logger_client::initialize()
{
    m_context = zmq_init(1);    //ָ��zmq ����I/0�¼���thread pool Ϊ1
    m_socket = zmq_socket(m_context, ZMQ_REP);
    zmq_bind(m_socket, "udp://*:9999");    // accept connections on a socket
    return true;
}

void logger_client::deinitialize()
{
    zmq_close(m_socket);
    zmq_term(m_context);
}

bool logger_client::post_message_log(const string_t& msg)
{
    zmq_msg_t send_msg;
    zmq_msg_init_size(&send_msg, msg.size());
    memcpy(zmq_msg_data(&send_msg), msg.c_str(), msg.size());
    zmq_msg_send(&send_msg, m_socket, 0);
    zmq_msg_close(&send_msg);

    //���շ�����Ӧ��
    //zmq_msg_t recv_msg;
    //zmq_msg_init(&recv_msg);
    //zmq_msg_recv(&recv_msg, z_socket, ZMQ_DONTWAIT);                    //0��ʾ������
    //printf("�յ�Server�˻ش�:\t");
    //std::cout << (char*)zmq_msg_data(&recv_msg) << std::endl;
    //zmq_msg_close(&recv_msg);

    return true;
}
