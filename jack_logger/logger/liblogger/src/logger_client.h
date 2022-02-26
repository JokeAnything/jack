#pragma once
#include "zmq.h"
#include "string"
using string_t = std::string;

class logger_client
{
private:
    logger_client() = default;
    ~logger_client() = default;

public:
    static logger_client& getinstance();
    bool initialize();
    void deinitialize();
    bool post_message_log(const string_t& msg);

private:
    void* m_context = nullptr;
    void* m_socket = nullptr;
};
