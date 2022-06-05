#include "hlddz_game_data_service.h"
#include "logger\logger.h"

GDPS_NAMESPACE_BEGIN

hlddz_game_data_service::hlddz_game_data_service()
{
    m_data_service_impl = std::make_shared<hlddz_game_data_service_api_impl>();
}

hlddz_game_data_service::~hlddz_game_data_service()
{
    uninitialize();
}

gdps_bool hlddz_game_data_service::initialize()
{
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("game data service initialized."));
    return true;
}

gdps_void hlddz_game_data_service::uninitialize()
{
    if (m_is_started)
    {
        stop_data_service();
        m_is_started = false;
    }
}

gdps_bool hlddz_game_data_service::start_data_service()
{
    if (m_is_started)
    {
        return true;
    }

    if (!m_data_service_impl->initialize())
    {
        return false;
    }

    m_is_started = true;
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("game data service starting..."));
    return true;
}

gdps_void hlddz_game_data_service::stop_data_service()
{
    if (m_is_started)
    {
        if (m_data_service_impl)
        {
            m_data_service_impl->uninitialize();
        }
        m_is_started = false;
    }
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("game data service stopped."));
}

data_service_game_api_ptr hlddz_game_data_service::get_data_service_game_api()
{
    return m_data_service_impl;
}

GDPS_NAMESPACE_END
