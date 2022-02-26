#include "logger.h"
#include "logger_client.h"

logger::logger()
{
    m_ready = logger_client::getinstance().initialize();
}

logger::~logger()
{
    logger_client::getinstance().deinitialize();
}

logger& logger::getinstance()
{
    static logger m;
    return m;
}

bool logger::post_message_log(const string_t& msg)
{
    if (!m_ready)
    {
        return false;
    }
    return logger_client::getinstance().post_message_log(msg);
}
