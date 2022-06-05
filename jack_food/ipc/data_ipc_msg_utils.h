#ifndef _DATA_IPC_MSG_UTILS_H
#define _DATA_IPC_MSG_UTILS_H

#include "data_ipc_defs.h"
#include <vector>

using ssid = std::string;
using function_name = std::string;
using parameter_list = std::vector<std::string>;
using return_list = std::vector<std::string>;
/**
request:
{
"ssid":"",
"function":"",
"parameters":[],
}

respond:
{
"ssid":"",
"function":"",
"return":[],
}
*/
class data_ipc_msg_utils
{
public:

    static data_message_string make_request_msg(ssid sid, function_name function, parameter_list parameters);
    static bool parse_respond_msg(const data_message_string& msg, 
        ssid& sid,
        function_name& function,
        return_list& parameters);
};

#endif //_DATA_IPC_MSG_UTILS_H

