#pragma once
#include "zmq.h"
#include "string"
using string_t = std::string;

using message_respond_proc = string_t(*)(const string_t& recved_msg);

class logger_server
{
private:
    logger_server() = default;
    ~logger_server() = default;

public:
    static logger_server& getinstance();
    bool initialize();
    void deinitialize();
    bool start_ipc_svc();
    void stop_ipc_svc();
    bool register_message_respond_proc(const message_respond_proc proc);
    void unregister_message_respond_proc();

private:
    void process_messages(const string_t& recved_msg, string_t& respond_msg);

private:
    void* m_context = nullptr;
    void* m_socket = nullptr;
    bool m_bexit = false;
    message_respond_proc m_message_respond_proc = nullptr;
};
