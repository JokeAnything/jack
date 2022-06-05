#ifndef _ai_SERVICE_FACTORY_H
#define _ai_SERVICE_FACTORY_H

#include "ai_service_object.h"

enum ai_service_type
{
    ai_service_type_hlddz = 0,
};

class ai_service_factory
{

private:

    ai_service_factory() = default;
    ~ai_service_factory() = default;

public:

    static ai_service_object_ptr create_ai_service_object(ai_service_type type);
    static void destroy_ai_service_object(ai_service_object_ptr& data_object_ptr);

};

#endif //_ai_SERVICE_FACTORY_H

