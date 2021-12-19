#include "hlddz_game_data_service.h"

GDPS_NAMESPACE_BEGIN
gdps_bool hlddz_game_data_service::initialize()
{
    return false;
}

gdps_void hlddz_game_data_service::uninitialize()
{

}
gdps_bool hlddz_game_data_service::start_data_service()
{
    if (!m_main_thread_ptr)
    {
        m_main_thread_ptr = std::make_shared<std::thread>(&main_work_proc, this);
    }
    return true;
}
gdps_void hlddz_game_data_service::stop_data_service()
{
    m_is_exit = true;
    if (m_main_thread_ptr)
    {
        if (m_main_thread_ptr->joinable())
        {
            m_main_thread_ptr->join();
        }
        m_main_thread_ptr = nullptr;
    }
}

gdps_void hlddz_game_data_service::main_work_proc()
{
    while (1)
    {
        if (m_is_exit)
        {
            break;
        }

        _sleep(100);
    }
}

GDPS_NAMESPACE_END
