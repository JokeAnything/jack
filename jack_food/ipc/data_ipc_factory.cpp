#include "data_ipc_factory.h"
#include "data_ipc_service_impl.h"

data_ipc_service_api_ptr data_ipc_factory::create_data_ipc_object(data_ipc_type type)
{
    if (type == data_ipc_type_hlddz)
    {
         return std::make_shared<data_ipc_service_impl>("tcp://localhost:5555");
    }
    else
    {
        return nullptr;
    }

}
void data_ipc_factory::destroy_data_ipc_object(data_ipc_service_api_ptr& ipc_api_ptr)
{
    ipc_api_ptr = nullptr;
}
