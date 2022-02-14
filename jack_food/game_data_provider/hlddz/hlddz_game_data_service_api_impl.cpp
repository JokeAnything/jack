#include "hlddz_game_data_service_api_impl.h"
#include <Tlhelp32.h>

GDPS_NAMESPACE_BEGIN
gdps_bool hlddz_game_data_service_impl::initialize()
{
    return false;
}
gdps_void hlddz_game_data_service_impl::uninitialize()
{

}
auto  hlddz_game_data_service_impl::get_current_turning_role()
{

}
auto  hlddz_game_data_service_impl::get_role_card_number(role_position role_id)
{

}
auto hlddz_game_data_service_impl::get_role_hand_cards(role_position role_id)
{

}
auto hlddz_game_data_service_impl::get_role_given_cards(role_position role_id)
{

}

GDPS_NAMESPACE_END
HMODULE GethlddzProcessBase()
{
    HANDLE hSnapShot;
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, get_process_id(MAIN_PE_NAME));
    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }
    MODULEENTRY32 ModuleEntry32;
    ModuleEntry32.dwSize = sizeof(ModuleEntry32);
    if (Module32First(hSnapShot, &ModuleEntry32))
    {
        do
        {
            if (strstr(ModuleEntry32.szExePath, MAIN_PE_NAME))
            {
                CloseHandle(hSnapShot);
                return ModuleEntry32.hModule;
            }
        } while (Module32Next(hSnapShot, &ModuleEntry32));
    }
    CloseHandle(hSnapShot);
    return NULL;
}

DWORD get_process_id(const wchar_t *processName)
{
    if (processName == nullptr)
    {
        return 0;
    }

    PROCESSENTRY32 pe32;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return 0;
    }


    BOOL bMore = ::Process32First(hProcessSnap, &pe32);
    while (bMore)
    {
        if (lstrcmp(pe32.szExeFile, processName) == 0)
        {
            break;
        }
        bMore = ::Process32Next(hProcessSnap, &pe32);
    }
    ::CloseHandle(hProcessSnap);

    return pe32.th32ProcessID;
}
