#include "logger/logger.h"
#include <thread>
#include <mutex>
#include <iostream>

LOGGER_NAMESPACE_BEGIN

std::once_flag once_flag;
logger_api_ptr g_p_logger = nullptr;

class loggerImpl :public logger_api
{

public:
    loggerImpl();
    ~loggerImpl();
    bool initialize();
    void deinitialize();

private:

    bool log_message(logger_level level, const string_t& msg) override;

private:

    bool m_init = false;
};

logger_api_ptr logger::getinstance()
{
    std::call_once(once_flag, []() {
        auto log_impl = std::make_shared<loggerImpl>();
        if (log_impl->initialize())
        {
            g_p_logger = log_impl;
        }
    });
    return g_p_logger;
}


loggerImpl::loggerImpl()
{
}

loggerImpl::~loggerImpl()
{
    deinitialize();
}

bool loggerImpl::initialize()
{
    m_init = true;
    return true;
}

void loggerImpl::deinitialize()
{
    if (!m_init)
    {
        return;
    }
    return;
}

bool loggerImpl::log_message(logger_level level, const string_t& msg)
{
    std::cout << msg.c_str() << std::endl;
    return true;
}

LOGGER_NAMESPACE_END
