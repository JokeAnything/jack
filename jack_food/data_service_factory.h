#ifndef _DATA_SERVICE_FACTORY_H
#define _DATA_SERVICE_FACTORY_H

#include "game_data_provider\game_data_provider_service_defs.h"
#include "data_service_object.h"
#include <thread>
#include <atomic>

GDPS_NAMESPACE_BEGIN

enum data_service_type
{
    data_service_type_hlddz = 0,
};

class data_service_factory
{

private:

    data_service_factory() = default;
    ~data_service_factory() = default;

public:

    static data_service_object_ptr get_data_service_object(data_service_type type);

};

GDPS_NAMESPACE_END

#endif //_DATA_SERVICE_FACTORY_H

