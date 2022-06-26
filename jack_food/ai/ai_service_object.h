#ifndef _AI_SERVICE_OBJECT_H
#define _AI_SERVICE_OBJECT_H

#include "hlddz/data_service_game_api.h"
#include <memory>

class ai_service_object
{
public:

    virtual ~ai_service_object() {}
    virtual bool initialize() = 0;
    virtual void uninitialize() = 0;
    virtual bool start_ai_service() = 0;
    virtual void stop_ai_service() = 0;
    virtual void reset_game() = 0;
    virtual GDPS::data_service_game_api_ptr get_data_service_game_api() = 0;
};

using ai_service_object_ptr = std::shared_ptr<ai_service_object>;

#endif //_AI_SERVICE_OBJECT_H

