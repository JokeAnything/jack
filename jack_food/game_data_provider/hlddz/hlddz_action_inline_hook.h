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

void __stdcall show_button_object(void* button_object, uint32_t show_status)
{
    if (hlddz_game_action_impl::s_myself_this != nullptr)
    {
        hlddz_game_action_impl::s_myself_this->show_button_object(button_object, show_status);
    }
}
void __stdcall update_game_timer_info(void* view_chair_pos, uint32_t time_left)
{
    if (!view_chair_pos)
    {
        return;
    }

    // 获取view char pos.
    unsigned char* pos = (unsigned char*)view_chair_pos + 116;
    gdps_uint8_t view_chair = *pos;
    if (hlddz_game_action_impl::s_myself_this)
    {
        hlddz_game_action_impl::s_myself_this->update_player_timer_info(view_chair, time_left);
    }
}

void _declspec(naked) notify_timer_update_proc_stub()
{
    _asm
    {
        pushad    //保留寄存器
        pushfd   //保留标志寄存器
    }

    // call business proc.
    _asm
    {
        push [esp + 4 + 32 + 4] // time left.
        push ecx
        call update_game_timer_info
    }

    _asm
    {
        popfd
        popad
    }

    _asm
    {
        push        ebp
        mov         ebp, esp
        push        0FFFFFFFFh
    }

    _asm
    {
        jmp hlddz_game_action_impl::s_timer_update_proc_original;
    }
}

__declspec(naked) void get_button_object_this_stub()
{
    __asm
    {
        push[esp + 4]
        call hlddz_game_action_impl::s_original_get_button_proc
        pushad
        pushfd
        push eax
        push[esp + 32 + 4 + 4 + 4]
        call set_button_object
        popfd
        popad
        retn 4
    }
}

__declspec(naked) void get_button_object_show_status_stub()
{
    __asm
    {
        pushad
        pushfd
        push[esp + 32 + 4 + 4]
        push  ecx
        call show_button_object
        popfd
        popad
        jmp hlddz_game_action_impl::s_original_show_button_status_proc
    }
}
