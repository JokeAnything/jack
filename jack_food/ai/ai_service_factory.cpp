#include "ai_service_factory.h"
#include "ai_service_object.h"
#include "douzero_service_proxy.h"

ai_service_object_ptr ai_service_factory::create_ai_service_object(ai_service_type type)
{
    if (type == ai_service_type_hlddz)
    {
        return  std::make_shared<douzero_service_proxy>();
    }
    return nullptr;
}

void ai_service_factory::destroy_ai_service_object(ai_service_object_ptr& data_object_ptr)
{
    data_object_ptr = nullptr;
}
