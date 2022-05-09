#include <windows.h>
#include "data_service_factory.h"
#include "data_service_game_api.h"
#include <future>
#include <iostream>

HMODULE g_hModule = NULL;
void start_data_service()
{
    auto ds_ptr = GDPS::data_service_factory::get_data_service_object(GDPS::data_service_type_hlddz);
    if (ds_ptr != nullptr)
    {
        if (ds_ptr->initialize())
        {
            ds_ptr->start_data_service();
        }
    }
}

void stop_data_service()
{
    auto ds_ptr = GDPS::data_service_factory::get_data_service_object(GDPS::data_service_type_hlddz);
    if (ds_ptr != nullptr)
    {
        ds_ptr->stop_data_service();
        ds_ptr->uninitialize();
        ds_ptr = nullptr;
    }
}

HANDLE g_handle = NULL;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    ::AllocConsole();
    FILE *stream = nullptr;
    ::freopen_s(&stream, "CONOUT$", "w+t", stdout);
    ::freopen_s(&stream, "CONIN$", "r+t", stdin);
    while (1)
    {
        std::string cmd = "";
        getline(std::cin, cmd);
        if (cmd == "start")
        {
            start_data_service();
        }
        else if (cmd == "stop")
        {
            stop_data_service();
        }
        else if (cmd == "break")
        {
            if (::IsDebuggerPresent())
            {
                ::DebugBreak();
            }
        }
        else if (cmd == "exit")
        {
            break;
        }
        else if (strlen(cmd.c_str()) <= 2)
        {
            auto type = std::strtol(cmd.c_str(), nullptr, 10);
            auto ds_ptr = GDPS::data_service_factory::get_data_service_object(GDPS::data_service_type_hlddz);
            if (ds_ptr != nullptr)
            {
                auto api = ds_ptr->get_data_service_game_api();
                if (api)
                {
                    api->execute_current_player_action((GDPS::player_action_type)type);
                }
            }

        }
    }
    ::FreeConsole();
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        {
            DWORD dwTid = 0;
            auto handle = ::CreateThread(NULL, 0, ThreadProc, (LPVOID)1, 0, &dwTid);
            ::CloseHandle(handle);
            g_hModule = hModule;
        }
        break;
    case DLL_PROCESS_DETACH:
        {
        }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return (TRUE);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    ThreadProc(0);
    Sleep(5000);
    return 0;
}
