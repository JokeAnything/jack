#ifndef _GAME_DATA_PROVIDER_SERVICE_DEFS_H
#define _GAME_DATA_PROVIDER_SERVICE_DEFS_H

#include <string>
#include <memory>
#include <vector>

#define GDPS_NAMESPACE_BEGIN namespace GDPS {
#define GDPS_NAMESPACE_END }

GDPS_NAMESPACE_BEGIN

    // data structs define
    using gdps_uint32_t = unsigned int;
    using gdps_int32_t = int;

    using gdps_uint16_t = unsigned short;
    using gdps_int16_t = short;

    using gdps_uint8_t = unsigned char;
    using gdps_int8_t = char;

    using gdps_uint32_ptr_t = void*;

    using gdps_void = void;

    using gdps_bool = bool;

    using gdps_string = std::string;

    template< class T >
    using gdps_shared_ptr = std::shared_ptr<T>;

    using card_number = gdps_uint32_t;

GDPS_NAMESPACE_END

#endif //_GAME_DATA_PROVIDER_SERVICE_DEFS_H
