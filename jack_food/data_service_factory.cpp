#include "data_service_factory.h"
#include "game_data_provider\hlddz\hlddz_game_data_service.h"

GDPS_NAMESPACE_BEGIN

data_service_object_ptr g_hlddz_game_data_service = nullptr;

data_service_object_ptr data_service_factory::get_data_service_object(data_service_type type)
{
    if (type == data_service_type_hlddz)
    {
        if (!g_hlddz_game_data_service)
        {
            g_hlddz_game_data_service = std::make_shared<hlddz_game_data_service>();
        }
        return g_hlddz_game_data_service;

    }
    return nullptr;
}

GDPS_NAMESPACE_END
