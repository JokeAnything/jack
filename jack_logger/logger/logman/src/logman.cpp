#include <stdio.h>
#include <iostream>
#include "logger_server.h"

string_t process_log_message(const string_t& recved_msg)
{
    std::cout << recved_msg << std::endl;
    return "ok";
}

int main()
{
    std::cout << "logman is starting..." << std::endl;
    auto& logger_svc = logger_server::getinstance();
    if (logger_svc.initialize())
    {
        logger_svc.register_message_respond_proc(process_log_message);
        logger_svc.start_ipc_svc();
        logger_svc.unregister_message_respond_proc();
        logger_svc.stop_ipc_svc();
        logger_svc.deinitialize();
    }
    std::cout << "logman was stopped!" << std::endl;
    return 0;
}
