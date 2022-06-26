#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
#include <memory>
#include <time.h>
#include <windows.h>

#define LOGGER   LOGGER

#define LOGGER_NAMESPACE_BEGIN namespace LOGGER {
#define LOGGER_NAMESPACE_END }

LOGGER_NAMESPACE_BEGIN

using string_t = std::string;

enum logger_level
{
    logger_level_info,
    logger_level_debug,
    logger_level_warning,
    logger_level_success,
    logger_level_error,
    logger_level_fatal,
};

class logger_api
{
public:
    virtual ~logger_api() {};
public:
    virtual bool log_message(logger_level level,const string_t& msg) = 0;
};

using logger_api_ptr = std::shared_ptr<logger_api>;

class logger
{

public:
    static logger_api_ptr getinstance();
};

class logger_utils
{
public:

    static void append_log_time(string_t& log_text)
    {
        time_t now_time = time(NULL);
        tm st_tm = { 0 };
        localtime_s(&st_tm, &now_time);
        string_t::value_type time_buffer[512] = { 0 };
        auto tickcount = ::GetTickCount() % 1000;
        sprintf_s(time_buffer, "[%02d-%02d-%02d %02d:%02d:%02d:%03d]",
            st_tm.tm_year + 1900,
            st_tm.tm_mon + 1,
            st_tm.tm_mday,
            st_tm.tm_hour,
            st_tm.tm_min,
            st_tm.tm_sec,
            tickcount);
        std::string current_time_text = time_buffer;
        log_text += current_time_text;
    }

    static void append_project_name(string_t& log_text)
    {
        string_t project_name;

#ifdef  LOGGER_PROJECT_NAME_SIGNATURE
        project_name = LOGGER_PROJECT_NAME_SIGNATURE;
#endif
        log_text += project_name;
    }

    template <typename... Args>
    static void append_log_info(string_t& log_text, const string_t::value_type* format, Args&&... args)
    {
        log_info(log_text, format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void append_from_info(string_t& log_text, const string_t::value_type* format, Args&&... args)
    {
        log_info(log_text, format, std::forward<Args>(args)...);
    }

private:

    template <typename... Args>
    static void log_info(string_t& log_text, const string_t::value_type* format, Args&&... args)
    {
        string_t::value_type buffer[512] = { 0 };
        sprintf_s(buffer, format, std::forward<Args>(args)...);
        log_text += buffer;
    }

};

//LOGGER::logger_utils::append_from_info(log_content, __FILE__, __FUNCTION__, __LINE__);

LOGGER_NAMESPACE_END

#define DEBUG_TEXT_FORMAT(x)    x

#define DEBUG_MSG( level, ... )   \
{ \
    LOGGER::logger_api_ptr api_ptr = LOGGER::logger::getinstance(); \
    if ( api_ptr ) \
    { \
        LOGGER::string_t log_content; \
        LOGGER::logger_utils::append_log_time(log_content); \
        LOGGER::logger_utils::append_project_name(log_content); \
        LOGGER::logger_utils::append_log_info(log_content, __VA_ARGS__); \
        api_ptr->log_message(LOGGER::level,log_content); \
    } \
} \

#endif

