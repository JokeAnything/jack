#ifndef _GAME_PROCESS_UTILS_H
#define _GAME_PROCESS_UTILS_H
#include <memory>

using process_image_base_ptr = void*;
using process_id = uint32_t;

class game_process_utils
{
public:
    game_process_utils() = default;
    ~game_process_utils() = default;

    static process_image_base_ptr get_process_image_base();
    static process_id get_process_id(const wchar_t *processName);
    static bool hook_function_table_proc(const void* target_fun_table_addr, const void* target_fun_ptr, void** original_fun_ptr);

private:

};

#endif //_GAME_PROCESS_UTILS_H


