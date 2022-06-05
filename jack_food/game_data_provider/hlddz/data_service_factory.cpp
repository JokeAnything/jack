#include "data_service_factory.h"
#include "hlddz_game_data_service.h"

GDPS_NAMESPACE_BEGIN

data_service_object_ptr data_service_factory::create_data_service_object(data_service_type type)
{
    if (type == data_service_type_hlddz)
    {
        return  std::make_shared<hlddz_game_data_service>();
    }
    return nullptr;
}

void data_service_factory::destroy_data_service_object(data_service_object_ptr& data_object_ptr)
{
    data_object_ptr = nullptr;
}

GDPS_NAMESPACE_END
