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

void _declspec(naked) notify_give_proc_stub()
{
    _asm
    {
        pushad    //±£Áô¼Ä´æÆ÷
        pushfd   //±£Áô±êÖ¾¼Ä´æÆ÷
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
        pushad    //±£Áô¼Ä´æÆ÷
        pushfd   //±£Áô±êÖ¾¼Ä´æÆ÷
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
        pushad    //±£Áô¼Ä´æÆ÷
        pushfd   //±£Áô±êÖ¾¼Ä´æÆ÷
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
