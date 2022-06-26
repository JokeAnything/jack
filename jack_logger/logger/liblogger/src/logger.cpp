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

#define CONSOLE_TEXT_COLOR_RED      (0x4|FOREGROUND_INTENSITY)
#define CONSOLE_TEXT_COLOR_WHITE    (0x7|FOREGROUND_INTENSITY)
#define CONSOLE_TEXT_COLOR_YELLOW   (0x6|FOREGROUND_INTENSITY)
#define CONSOLE_TEXT_COLOR_GREEN    (0x2|FOREGROUND_INTENSITY)

void printf_log(logger_level level, const char* log_content)
{
    auto set_color = [](unsigned short fore_color, unsigned short background_color) {
        auto output = ::GetStdHandle(STD_OUTPUT_HANDLE);
        ::SetConsoleTextAttribute(output, fore_color | background_color);
    };

    if (log_content)
    {
        if (level == logger_level_error || level == logger_level_fatal)
        {
            set_color(CONSOLE_TEXT_COLOR_RED, 0);
        }
        else if (level == logger_level_warning)
        {
            set_color(CONSOLE_TEXT_COLOR_YELLOW, 0);
        }
        else if (level == logger_level_success)
        {
            set_color(CONSOLE_TEXT_COLOR_GREEN, 0);
        }
        else
        {
            set_color(CONSOLE_TEXT_COLOR_WHITE, 0);
        }
        std::cout << log_content << std::endl;
        set_color(CONSOLE_TEXT_COLOR_WHITE, 0);
    }
}

bool loggerImpl::log_message(logger_level level, const string_t& msg)
{
    printf_log(level, msg.c_str());
    return true;
}

LOGGER_NAMESPACE_END
