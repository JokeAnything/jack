#pragma once
#include "string"
using string_t = std::string;
class logger
{
private:
    logger();
    ~logger();

public:
    static logger& getinstance();
    bool post_message_log(const string_t& msg);

private:

    bool m_ready = false;
};
