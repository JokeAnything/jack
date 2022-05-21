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
    return 0;

#if 0
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
#endif
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

bool game_process_utils::set_inline_hook(unsigned char* lphooked_addr,
    unsigned char* target_proc,
    void** jmp_back_proc,
    uint32_t num,
    unsigned char** code_backup)
{
    if (!lphooked_addr || !target_proc || !jmp_back_proc ||!code_backup)
    {
        return false;
    }

    if (num < 5)
    {
        return false;
    }

    //改变修改地址为可写属性
    DWORD OldProtect = 0;
    DWORD bret = ::VirtualProtect((LPBYTE)lphooked_addr, num, PAGE_EXECUTE_READWRITE, &OldProtect);
    if (bret == 0)
    {
        return false;
    }

    auto backup_ptr = (unsigned char*)malloc(num * sizeof(unsigned char));
    memcpy(backup_ptr, lphooked_addr, num);

    auto jmp_addr = (uint32_t)(target_proc - lphooked_addr - 5);
    *lphooked_addr = 0xE9;
    *(uint32_t*)(lphooked_addr + 1) = jmp_addr;
    *jmp_back_proc = (void*)(lphooked_addr + num);
    *code_backup = backup_ptr;

    ::VirtualProtect((LPBYTE)lphooked_addr, num, OldProtect, &OldProtect);
    return true;
}

bool game_process_utils::restore_inline_hook(unsigned char* lphooked_addr,
    unsigned char* code_backup,
    uint32_t backup_num)
{
    if (!lphooked_addr || !code_backup || !backup_num)
    {
        return false;
    }

    //改变修改地址为可写属性
    DWORD OldProtect = 0;
    DWORD bret = ::VirtualProtect((LPBYTE)lphooked_addr, backup_num, PAGE_EXECUTE_READWRITE, &OldProtect);
    if (bret == 0)
    {
        return false;
    }
    memcpy(lphooked_addr, code_backup, backup_num);
    ::VirtualProtect((LPBYTE)lphooked_addr, backup_num, OldProtect, &OldProtect);
    free(code_backup);
    return true;
}

process_wnd game_process_utils::get_game_window(const char* wnd_class)
{
    if (wnd_class == nullptr)
    {
        return NULL;
    }
    // classname : HLDDZ
    // windowname:欢乐斗地主
    return ::FindWindow(wnd_class, nullptr);
}

void game_process_utils::get_wnd_width_height(process_wnd wnd,
    uint32_t& width,
    uint32_t& height)
{
    if (wnd == NULL)
    {
        return;
    }
    width = 0;
    height = 0;
    RECT rect = { 0 };
    ::GetClientRect(wnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
    return;
}

void game_process_utils::post_left_button_down_message(process_wnd wnd, uint32_t x, uint32_t y)
{
    if (wnd == NULL)
    {
        return;
    }

    LPARAM pos = (LPARAM)y;
    pos = (pos << 16) | x;

    ::PostMessage(wnd, WM_LBUTTONDOWN, (WPARAM)MK_LBUTTON, (LPARAM)pos);
    ::PostMessage(wnd, WM_LBUTTONUP, (WPARAM)MK_LBUTTON, (LPARAM)pos);
}
