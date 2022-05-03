#include "game_process_utils.h"
#include <windows.h>
#include <Tlhelp32.h>


process_image_base_ptr game_process_utils::get_process_image_base()
{
    return (process_image_base_ptr)::GetModuleHandle(NULL);

#if 0
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
#endif
}


process_id game_process_utils::get_process_id(const wchar_t *processName)
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

bool game_process_utils::hook_function_table_proc(const void* target_fun_table_addr, const void* target_fun_ptr, void** original_fun_ptr)
{
    if ((target_fun_table_addr == nullptr) ||
        (target_fun_ptr == nullptr) ||
        (original_fun_ptr == nullptr))
    {
        return false;
    }
    DWORD dwOld = PAGE_EXECUTE_READWRITE;
    auto res = ::VirtualProtect((LPVOID)target_fun_table_addr, sizeof(void*), PAGE_EXECUTE_READWRITE, &dwOld);
    if (!res)
    {
        return false;
    }
    *original_fun_ptr = *(void**)target_fun_table_addr;
    *(void**)target_fun_table_addr = (void*)target_fun_ptr;

    ::VirtualProtect((LPVOID)target_fun_table_addr, sizeof(void*), dwOld, &dwOld);
    return true;
}
