#ifndef _GAME_PROCESS_UTILS_H
#define _GAME_PROCESS_UTILS_H
#include <memory>
#include <windows.h>

using process_image_base_ptr = void*;
using process_id = uint32_t;
using process_wnd = HWND;

class game_process_utils
{
public:
    game_process_utils() = default;
    ~game_process_utils() = default;

    static process_image_base_ptr get_process_image_base();
    static process_id get_process_id(const wchar_t *processName);
    static bool hook_function_table_proc(const void* target_fun_table_addr, const void* target_fun_ptr, void** original_fun_ptr);

    static bool set_inline_hook(unsigned char* lphooked_addr,
        unsigned char* target_proc,
        void** jmp_back_proc,
        uint32_t num,
        unsigned char** code_backup);

    static bool restore_inline_hook(unsigned char* lphooked_addr,
        unsigned char* code_backup,
        uint32_t backup_num);

    static process_wnd get_game_window(const char* wnd_class);

    static void get_wnd_width_height(process_wnd wnd,
        uint32_t& width,
        uint32_t& height);

    static void post_left_button_down_message(process_wnd wnd, uint32_t x, uint32_t y);

private:

};

#endif //_GAME_PROCESS_UTILS_H


