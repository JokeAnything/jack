#include "hlddz_game_action_impl.h"
#include "../../utils/game_process_utils.h"
#include "hlddz_data_bin_offset_defs.h"
#include <windows.h>

GDPS_NAMESPACE_BEGIN

hlddz_game_action_impl* hlddz_game_action_impl::s_myself_this = nullptr;
void* hlddz_game_action_impl::s_original_get_button_proc = nullptr;

void* __stdcall set_button_object(object_name::value_type* button_name, object_this object_ptr)
{
    object_name::value_type buffer[100] = { 0 };
    __try
    {
        strcpy_s(buffer, button_name);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return nullptr;
    }

    if (hlddz_game_action_impl::s_myself_this != nullptr)
    {
        hlddz_game_action_impl::s_myself_this->save_button_object(buffer, object_ptr);
    }
    return object_ptr;
}

__declspec(naked) void get_button_object_this_stub()
{
    __asm
    {
        push [esp + 4]
        call hlddz_game_action_impl::s_original_get_button_proc
        pushad
        pushfd
        push eax
        push [esp + 32 + 4 + 4 + 4]
        call set_button_object
        popfd
        popad
        retn 4
    }
}

DWORD WINAPI btn_click_thread_proc(LPVOID lpParam)
{
    if (lpParam == nullptr)
    {
        return 0;
    }
    __try
    {
        void* btn_object = lpParam;
        uint32_t base = (uint32_t)game_process_utils::get_process_image_base();
        auto proc_address = (uint32_t)(BO_BUTTON_OBJECT_CLICK_PROC_ADDRESS(base));
        __asm
        {
            pushad
            pushfd
            mov ecx, btn_object
            push 2 // clicked event type.
            call proc_address
            popfd
            popad
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
    return 1;
}

gdps_bool hlddz_game_action_impl::initialize()
{
    std::lock_guard<std::mutex> guard(data_locker);

    m_button_object_list[BO_BUTTON_NAME_BTN_STARTGAME] = std::make_pair(player_action_type_start_game, nullptr);
    m_button_object_list[BO_BUTTON_NAME_BTN_BUJIAO] = std::make_pair(player_action_type_not_being_landlord, nullptr);
    m_button_object_list[BO_BUTTON_NAME_BTN_JIAODIZHU] = std::make_pair(player_action_type_being_landlord, nullptr);
    m_button_object_list[BO_BUTTON_NAME_BTN_BUQIANG] = std::make_pair(player_action_type_do_not_rob_landlord, nullptr);
    m_button_object_list[BO_BUTTON_NAME_BTN_QIANGDIZHU] = std::make_pair(player_action_type_rob_landlord, nullptr);
    m_button_object_list[BO_BUTTON_NAME_BTN_JIABEI] = std::make_pair(player_action_type_do_double, nullptr);
    m_button_object_list[BO_BUTTON_NAME_BTN_BUJIABEI] = std::make_pair(player_action_type_do_not_double, nullptr);
    m_button_object_list[BO_BUTTON_NAME_BTN_BUCHU] = std::make_pair(player_action_type_do_not_give, nullptr);
    m_button_object_list[BO_BUTTON_NAME_BTN_CHUPAI] = std::make_pair(player_action_type_do_give, nullptr);

    s_myself_this = this;

    uint32_t base = (uint32_t)game_process_utils::get_process_image_base();
    auto res = game_process_utils::hook_function_table_proc((void*)(BO_BUTTON_OBJECT_COLLECT_PROC_ADDRESS(base)),
        (const void*)get_button_object_this_stub,
        (void**)(&hlddz_game_action_impl::s_original_get_button_proc));
    return res;
}

gdps_void hlddz_game_action_impl::uninitialize()
{
    std::lock_guard<std::mutex> guard(data_locker);

    uint32_t base = (uint32_t)game_process_utils::get_process_image_base();
    void* old_proc_address = nullptr;
    game_process_utils::hook_function_table_proc((void*)(BO_BUTTON_OBJECT_COLLECT_PROC_ADDRESS(base)),
        (const void*)hlddz_game_action_impl::s_original_get_button_proc,
        (void**)(&old_proc_address));
    s_myself_this = nullptr;
    m_button_object_list.clear();    
}

void hlddz_game_action_impl::save_button_object(const object_name::value_type* button_name, object_this object_ptr)
{
    if (button_name == nullptr || object_ptr == nullptr)
    {
        return;
    }

    std::string btn_name = button_name;
    std::lock_guard<std::mutex> guard(data_locker);
    auto found = m_button_object_list.find(btn_name);
    if (found != m_button_object_list.end())
    {
        found->second.second = object_ptr;
    }
}

bool hlddz_game_action_impl::click_button(player_action_type button_type)
{
    std::lock_guard<std::mutex> guard(data_locker);
    object_this btn_object = nullptr;
    for (auto&& item : m_button_object_list)
    {
        if (item.second.first == button_type)
        {
            btn_object = item.second.second;
            break;
        }
    }
    if (!btn_object)
    {
        return false;
    }

    DWORD dwTid = 0;
    auto handle = ::CreateThread(NULL, 0, btn_click_thread_proc, (LPVOID)btn_object, 0, &dwTid);
    ::CloseHandle(handle);
    return true;
}


GDPS_NAMESPACE_END
