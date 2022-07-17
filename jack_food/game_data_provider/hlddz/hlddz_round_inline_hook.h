void __stdcall get_player_given_cards_info()
{
    if (hlddz_game_round_impl::s_myself_this)
    {
        hlddz_game_round_impl::s_myself_this->parse_current_given_cards();
    }
}

void __stdcall game_started_notify(void* self_card_base_ptr)
{
    if (!self_card_base_ptr)
    {
        return;
    }
    if (hlddz_game_round_impl::s_myself_this)
    {
        hlddz_game_round_impl::s_myself_this->parse_myself_handcards(self_card_base_ptr);
    }
}

void __stdcall game_bottom_cards_notify(void* card_set_base)
{
    if (!card_set_base)
    {
        return;
    }

    if (hlddz_game_round_impl::s_myself_this)
    {
        hlddz_game_round_impl::s_myself_this->parse_bottom_cards(card_set_base);
    }
}

void __stdcall user_select_card_change_notify(void* notify_this)
{
    if (!notify_this)
    {
        return;
    }

    if (hlddz_game_round_impl::s_myself_this)
    {
        hlddz_game_round_impl::s_myself_this->notify_user_select_card_change(notify_this);
    }
    return;

}

void _declspec(naked) notify_give_proc_stub()
{
    _asm
    {
        pushad    //�����Ĵ���
        pushfd   //������־�Ĵ���
    }

    // call business proc.
    _asm
    {
        call get_player_given_cards_info
    }

    _asm
    {
        popfd
        popad
    }

    _asm
    {
        mov     edi, [eax + 0x3C]
        movsx   eax, byte ptr[esi + 0x124]
        push    eax

    }

    _asm
    {
        jmp hlddz_game_round_impl::s_notify_give_proc_original;
    }
}

void _declspec(naked) notify_game_started_proc_stub()
{
    _asm
    {
        mov     edx, [eax + 0x3C]
        mov     eax, [edx + 0x78]
        mov     eax, [eax]

    }
    _asm
    {
        pushad    //�����Ĵ���
        pushfd   //������־�Ĵ���
    }

    // call business proc.
    _asm
    {
        push eax //  self cards base ptr.
        call game_started_notify
    }

    _asm
    {
        popfd
        popad
    }

    _asm
    {
        jmp hlddz_game_round_impl::s_game_started_proc_original;
    }
}

void _declspec(naked) notify_game_bottom_cards_proc_stub()
{
    _asm
    {
        mov     edx, [edi + 0x0C]
        mov     eax, edx
        mov     ecx, [edi + 0x8]
        sub     eax, ecx
        sar     eax, 2

    }
    _asm
    {
        pushad    //�����Ĵ���
        pushfd   //������־�Ĵ���
    }

    // call business proc.
    _asm
    {
        push  edi
        call game_bottom_cards_notify
    }

    _asm
    {
        popfd
        popad
    }

    _asm
    {
        jmp hlddz_game_round_impl::s_game_bottom_cards_notify_proc_original;
    }
}


void _declspec(naked) notify_user_select_card_change_notify_proc_stub()
{
    _asm
    {
        mov [ebp - 0xB8], ecx
        mov eax, [ebp + 0xC]
        mov [ebp - 0xC0], eax
    }

    _asm
    {
        pushad    //�����Ĵ���
        pushfd   //������־�Ĵ���
    }

    //_asm
    //{
    //    push eax
    //    call user_select_card_change_notify
    //}

    _asm
    {
        popfd
        popad
    }

    _asm
    {
        jmp hlddz_game_round_impl::s_user_select_card_change_notify_proc_original;
    }
}
