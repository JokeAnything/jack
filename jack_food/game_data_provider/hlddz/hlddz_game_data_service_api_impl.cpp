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
    //获取进程基址
    HANDLE hSnapShot;
    //通过CreateToolhelp32Snapshot和线程ID，获取进程快照
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
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
            TCHAR szExt[5];
            strcpy(szExt, ModuleEntry32.szExePath + strlen(ModuleEntry32.szExePath) - 4);
            for (int i = 0; i < 4; i++)
            {
                if ((szExt[i] >= 'a') && (szExt[i] <= 'z'))
                {
                    szExt[i] = szExt[i] - 0x20;
                }
            }
            if (!strcmp(szExt, ".EXE"))
            {
                CloseHandle(hSnapShot);
                return ModuleEntry32.hModule;
            }
        } while (Module32Next(hSnapShot, &ModuleEntry32));
    }
    CloseHandle(hSnapShot);
    return NULL;

}

DWORD GetProcessID(const char *ProcessName)
{
    PROCESSENTRY32 pe32;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    //获取进程快照
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot 调用失败.\n");
        return -1;
    }

    //遍历进程快照，轮流显示每个进程的信息
    BOOL bMore = ::Process32First(hProcessSnap, &pe32);
    while (bMore)
    {
        printf("进程名称：%ls\n", pe32.szExeFile); //这里得到的应该是宽字符，用%ls,不然无法正常打印
        printf("进程ID：%u\n\n", pe32.th32ProcessID);
        if (lstrcmp(pe32.szExeFile, ProcessName) == 0)
        {
            break;
        }

        bMore = ::Process32Next(hProcessSnap, &pe32);
    }

    //不要忘记清除掉snapshot对象
    ::CloseHandle(hProcessSnap);

    return pe32.th32ProcessID;
}
