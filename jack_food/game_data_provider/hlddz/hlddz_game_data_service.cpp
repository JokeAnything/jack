#include "hlddz_game_data_service.h"
#include "logger\logger.h"

GDPS_NAMESPACE_BEGIN

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
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("game data service deinitialized."));
}

gdps_bool hlddz_game_data_service::start_data_service()
{
    if (!m_main_thread_ptr)
    {
        m_main_thread_ptr = std::make_shared<std::thread>(&hlddz_game_data_service::main_work_proc, this);
    }
    m_is_exit = false;
    m_is_started = true;
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("game data service starting..."));
    return true;
}

gdps_void hlddz_game_data_service::stop_data_service()
{
    m_is_exit = true;
    m_is_started = false;

    if (m_main_thread_ptr)
    {
        if (m_main_thread_ptr->joinable())
        {
            m_main_thread_ptr->join();
        }
        m_main_thread_ptr = nullptr;
    }
    DEBUG_MSG(logger_level_debug, DEBUG_TEXT_FORMAT("game data service stopped."));
}

data_service_game_api_ptr hlddz_game_data_service::get_data_service_game_api()
{
    return m_data_service_impl;
}

gdps_void hlddz_game_data_service::main_work_proc()
{
    m_data_service_impl = std::make_shared<hlddz_game_data_service_impl>();
    if (!m_data_service_impl->initialize())
    {
        return;
    }

    while (1)
    {
        if (m_is_exit)
        {
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        auto action_type = player_action_type_invalid;
        auto api = get_data_service_game_api();
        api->execute_current_player_action(action_type);
        action_type = player_action_type_invalid;
    }

    m_data_service_impl->uninitialize();
    m_data_service_impl = nullptr;
}

GDPS_NAMESPACE_END
