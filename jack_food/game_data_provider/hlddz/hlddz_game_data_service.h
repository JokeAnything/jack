#ifndef _HLDDZ_GAME_DATA_SERVICE_H
#define _HLDDZ_GAME_DATA_SERVICE_H
#include "../game_data_provider_service_defs.h"
#include <thread>
#include <atomic>

GDPS_NAMESPACE_BEGIN

class hlddz_game_data_service_api
{
public:

    virtual ~hlddz_game_data_service_api() {}
    virtual gdps_bool initialize() = 0;
    virtual gdps_void uninitialize() = 0;
    virtual gdps_bool start_data_service() = 0;
    virtual gdps_void stop_data_service() = 0;
};

class hlddz_game_data_service : public hlddz_game_data_service_api
{
public:

    hlddz_game_data_service() = default;
    ~hlddz_game_data_service() = default;

    gdps_bool initialize() override;
    gdps_void uninitialize() override;
    gdps_bool start_data_service() override;
    gdps_void stop_data_service() override;

private:

    gdps_void main_work_proc();

private:

    gdps_shared_ptr<std::thread> m_main_thread_ptr = nullptr;
    std::atomic_bool m_is_exit = false;
};

GDPS_NAMESPACE_END

#endif //_HLDDZ_GAME_DATA_SERVICE_H
