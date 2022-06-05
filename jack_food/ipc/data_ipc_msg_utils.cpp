#include "data_ipc_msg_utils.h"
#include "jsoncpp.h"

data_message_string data_ipc_msg_utils::make_request_msg(ssid sid, function_name function, parameter_list parameters)
{
    json_t json_object;
    if (!sid.empty())
    {
        json_object["ssid"] = sid;
    }
    if (!function.empty())
    {
        json_object["function"] = function;
    }
    if (!parameters.empty())
    {
        json_object["parameters"] = parameters;
    }
    if (json_object.is_object())
    {
        return json_object.dump();
    }
    else
    {
        return "";
    }
}

bool data_ipc_msg_utils::parse_respond_msg(const data_message_string& msg,
    ssid& sid,
    function_name& function,
    return_list& ret_list)
{
    if (msg.empty())
    {
        return false;
    }
    auto json_object = json_t::parse(msg);
    if (!json_object.is_object())
    {
        return false;
    }
    ret_list.clear();
    sid = json_object["ssid"];
    function = json_object["function"];
    if (json_object["return_list"].is_array())
    {
        for (auto&& item : json_object["return_list"])
        {
            if (item.is_string())
            {
                ret_list.push_back(item);
            }
        }
    }
    return true;
}
