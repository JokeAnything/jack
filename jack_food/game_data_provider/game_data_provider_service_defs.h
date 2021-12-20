#ifndef _GAME_DATA_PROVIDER_SERVICE_DEFS_H
#define _GAME_DATA_PROVIDER_SERVICE_DEFS_H

#include <string>
#include <memory>
#include <vector>

#define GDPS_NAMESPACE_BEGIN namespace GDPS {
#define GDPS_NAMESPACE_END }

GDPS_NAMESPACE_BEGIN

    // 数据类型定义
    using gdps_uint32_t = unsigned int;
    using gdps_int32_t = int;

    using gdps_uint16_t = unsigned short;
    using gdps_int16_t = short;

    using gdps_uint8_t = unsigned char;
    using gdps_int8_t = char;

    using gdps_uint32_ptr_t = ULONG_PTR;

    using gdps_void = void;

    using gdps_bool = bool;

    using gdps_string = std::string;

    template< class T >
    using gdps_shared_ptr = std::shared_ptr<T>;

    enum role_position
    {
        role_position_landlord_up = 0,
        role_position_landlord = 1,
        role_position_landlord_down = 2,
    };

    using card_number = gdps_uint32_t;

    struct card_item
    {
        gdps_uint8_t m_card_value = 0;
        gdps_uint8_t m_card_shape = 0;
        gdps_uint8_t m_card_level = 0;
    };

    using card_list = std::vector<card_item>;


GDPS_NAMESPACE_END

#endif //_GAME_DATA_PROVIDER_SERVICE_DEFS_H
