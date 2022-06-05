#ifndef _DATA_IPC_FACTORY_H
#define _DATA_IPC_FACTORY_H

#include "data_ipc_service_api.h"
#include <thread>
#include <atomic>

enum data_ipc_type
{
    data_ipc_type_hlddz = 0,
};

class data_ipc_factory
{

private:

    data_ipc_factory() = default;
    ~data_ipc_factory() = default;

public:

    static data_ipc_service_api_ptr create_data_ipc_object(data_ipc_type type);
    static void destroy_data_ipc_object(data_ipc_service_api_ptr& ipc_api_ptr);
};

#endif //_DATA_IPC_FACTORY_H

