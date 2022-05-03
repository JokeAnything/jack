#ifndef _HLDDZ_GAME_DATA_SERVICE_H
#define _HLDDZ_GAME_DATA_SERVICE_H
#include "../game_data_provider_service_defs.h"
#include "../../data_service_object.h"
#include "hlddz_game_data_service_api_impl.h"

#include <thread>
#include <atomic>

GDPS_NAMESPACE_BEGIN

class hlddz_game_data_service : public data_service_object
{
public:

    hlddz_game_data_service() = default;
    ~hlddz_game_data_service();

    gdps_bool initialize() override;
    gdps_void uninitialize() override;
    gdps_bool start_data_service() override;
    gdps_void stop_data_service() override;
    data_service_game_api_ptr get_data_service_game_api() override;

private:

    gdps_void main_work_proc();

private:

    gdps_shared_ptr<std::thread> m_main_thread_ptr = nullptr;
    std::atomic_bool m_is_exit = false;
    std::atomic_bool m_is_started = false;
    hlddz_game_data_service_impl_ptr m_data_service_impl = nullptr;
};

GDPS_NAMESPACE_END

#endif //_HLDDZ_GAME_DATA_SERVICE_H
