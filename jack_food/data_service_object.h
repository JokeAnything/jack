#ifndef _DATA_SERVICE_OBJECT_H
#define _DATA_SERVICE_OBJECT_H

#include "game_data_provider\game_data_provider_service_defs.h"
#include "data_service_game_api.h"
#include <memory>

GDPS_NAMESPACE_BEGIN

class data_service_object
{
public:

    virtual ~data_service_object() {}
    virtual gdps_bool initialize() = 0;
    virtual gdps_void uninitialize() = 0;
    virtual gdps_bool start_data_service() = 0;
    virtual gdps_void stop_data_service() = 0;
    virtual data_service_game_api_ptr get_data_service_game_api() = 0;
};

using data_service_object_ptr = std::shared_ptr<data_service_object>;

GDPS_NAMESPACE_END

#endif //_DATA_SERVICE_OBJECT_H

