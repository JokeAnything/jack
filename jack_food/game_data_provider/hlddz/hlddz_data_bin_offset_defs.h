#ifndef _HLDDZ_DATA_BIN_OFFSET_DEFS_H
#define _HLDDZ_DATA_BIN_OFFSET_DEFS_H

#include "../game_data_provider_service_defs.h"

GDPS_NAMESPACE_BEGIN


//.text : 00A8B96A                 push    offset aBtn_startgame; "Btn_startgame"
//.text : 00A8B96F                 mov     edx, [ebp + var_4]
//.text : 00A8B972                 mov     eax, [edx]
//.text : 00A8B974                 mov     ecx, [ebp + var_4]
//.text : 00A8B977                 mov     edx, [eax + 1C0h] // eax + 1c0h
//.text : 00A8B97D                 call    edx
#define BO_BUTTON_OBJECT_COLLECT_PROC_OFFSET            (0x0116e1c4)
#define BO_BUTTON_OBJECT_COLLECT_PROC_ADDRESS(base)     (base + BO_BUTTON_OBJECT_COLLECT_PROC_OFFSET)

//#define BO_BUTTON_OBJECT_SHOW_PROC_OFFSET            (0x00a48c40)
//#define BO_BUTTON_OBJECT_SHOW_PROC_ADDRESS(base)     (base+BO_BUTTON_OBJECT_SHOW_PROC_OFFSET)
//
//#define BO_BUTTON_OBJECT_ISSHOW_PROC_OFFSET            (0xA48D60)
//#define BO_BUTTON_OBJECT_ISSHOW_PROC_ADDRESS(base)     (base+BO_BUTTON_OBJECT_ISSHOW_PROC_OFFSET)

#define BO_BUTTON_OBJECT_CLICK_PROC_OFFSET               (0x00436760)
#define BO_BUTTON_OBJECT_CLICK_PROC_ADDRESS(base)        (base + BO_BUTTON_OBJECT_CLICK_PROC_OFFSET)


#define BO_BUTTON_NAME_BTN_STARTGAME            ("Btn_startgame")

#define BO_BUTTON_NAME_BTN_BUJIAO               ("Btn_bujiao")
#define BO_BUTTON_NAME_BTN_JIAODIZHU            ("Btn_jiaodizhu")
#define BO_BUTTON_NAME_BTN_BUQIANG              ("Btn_buqiang")
#define BO_BUTTON_NAME_BTN_QIANGDIZHU           ("Btn_qiangdizhu")

#define BO_BUTTON_NAME_BTN_BUJIABEI             ("Btn_bujiabei")
#define BO_BUTTON_NAME_BTN_JIABEI               ("Btn_jiabei")

#define BO_BUTTON_NAME_BTN_BUCHU                ("Btn_buchu")
#define BO_BUTTON_NAME_BTN_CHUPAI               ("Btn_chupai")

#define BO_BUTTON_NAME_BTN_MINGPAI              ("Btn_mingpai")
#define BO_BUTTON_NAME_BTN_CHANGE_OPPONENTS     ("Btn_huanduishou")
#define BO_BUTTON_NAME_BTN_SUPER_MULTI          ("Btn_chaojijiabei")
#define BO_BUTTON_NAME_BTN_SUGGESTION           ("Btn_tishi")

// sub_46efb0()
#define BO_DATA_ROUND_MGR_BASE_PTR_PROC_OFFSET              (0x6efb0)
#define BO_DATA_ROUND_MGR_BASE_PTR_PROC_ADDRESS(base)       (base+BO_DATA_ROUND_MGR_BASE_PTR_PROC_OFFSET)

// sub_46FE60
#define BO_DATA_ROUND_VIEW_PLAYER_OBJECT_PTR_PROC_OFFSET              (0x6fe60)
#define BO_DATA_ROUND_VIEW_PLAYER_OBJECT_PTR_PROC_ADDRESS(base)       (base+BO_DATA_ROUND_VIEW_PLAYER_OBJECT_PTR_PROC_OFFSET)

// given card log,search log keyword:"Current Cards:\n  Give%s chair=%d lord="
// caution:two points;need to check both.
// pos:
//.text:00790ACD                 mov     edi, [eax + 3Ch]
//.text:00790AD0                 movsx   eax, byte ptr[esi + 124h]
//.text:00790AD7                 push    eax
//.text:00790AD8                 movzx   eax, byte ptr[ebx + 1]

#define BO_DATA_ROUND_GIVE_NOTIFY_PROC_OFFSET               (0x17C29D)
#define BO_DATA_ROUND_GIVE_NOTIFY_PROC_ADDRESS(base)        (base+BO_DATA_ROUND_GIVE_NOTIFY_PROC_OFFSET)

// update action timer,search log keyword:"UpdateClock LeftTime = %d, cViewChair ="
// pos:
//.text:00D5DCD0                 push    ebp
//.text : 00D5DCD1                 mov     ebp, esp
//.text : 00D5DCD3                 push    0FFFFFFFFh
//.text : 00D5DCD5                 push    offset sub_E82D40
//.text : 00D5DCDA                 mov     eax, large fs : 0

#define BO_DATA_ROUND_TIMER_UPDATE_PROC_OFFSET               (0x95dcd0)
#define BO_DATA_ROUND_TIMER_UPDATE_PROC_ADDRESS(base)        (base+BO_DATA_ROUND_TIMER_UPDATE_PROC_OFFSET)

// recv myself hand cards,search log keyword:"selfHand17_%s"
//.text : 007513D7                 mov     edx, [eax + 3Ch]
//.text : 007513DA                 mov     eax, [edx + 78h]
//.text : 007513DD                 mov     eax, [eax]
//.text : 007513DF                 mov     ecx, [eax + 0Ch]
//.text : 007513E2                 sub     ecx, [eax + 8]
//.text : 007513E5                 mov     eax, [edx + 9Ch]
//.text : 007513EB                 sar     ecx, 2

#define BO_DATA_ROUND_GAME_START_PROC_OFFSET               (0x38A10B)
#define BO_DATA_ROUND_GAME_START_PROC_ADDRESS(base)        (base+BO_DATA_ROUND_GAME_START_PROC_OFFSET)


// recv bottom cards,search log keyword:"onGiveBeneathCardRsp beneath:count=%d"
//.text:0078BE8B                 mov     edx, [edi + 0Ch]
//.text:0078BE8E                 mov     eax, edx
//.text:0078BE90                 mov     ecx, [edi + 8]
//.text:0078BE93                 sub     eax, ecx
//.text:0078BE95                 sar     eax, 2

#define BO_DATA_ROUND_GAME_BOTTOM_CARDS_NOTIFY_PROC_OFFSET               (0x38BE8B)
#define BO_DATA_ROUND_GAME_BOTTOM_CARDS_NOTIFY_PROC_ADDRESS(base)        (base+BO_DATA_ROUND_GAME_BOTTOM_CARDS_NOTIFY_PROC_OFFSET)

// recv order notify,search log keyword:"onGiveBeneathCardRsp beneath:count=%d"
//.text : 0074DC7F                 mov     ecx, [eax + 10h]
//.text : 0074DC82                 movzx   eax, byte ptr[esi + 2]
//.text : 0074DC86                 mov[ecx + 6Ch], al
#define BO_DATA_ACTION_GAME_ORDER_NOTIFY_PROC_OFFSET               (0x38648F)
#define BO_DATA_ACTION_GAME_ORDER_NOTIFY_PROC_ADDRESS(base)        (base+BO_DATA_ACTION_GAME_ORDER_NOTIFY_PROC_OFFSET)


// recv select notify,search log keyword:"onUserSelectCardChange:"
//.text:00A838DB                 mov [ebp + var_B8], ecx
//.text : 00A838E1               mov eax, [ebp + arg_4]
//.text : 00A838E4               mov [ebp + var_C0], eax

#define BO_DATA_ACTION_GAME_SELECT_CHANGE_NOTIFY_PROC_OFFSET               (0x6838DB)
#define BO_DATA_ACTION_GAME_SELECT_CHANGE_NOTIFY_PROC_ADDRESS(base)        (base+BO_DATA_ACTION_GAME_SELECT_CHANGE_NOTIFY_PROC_OFFSET)


#define BO_DATA_ROUND_ON_GIVE_CURRENT_OFFSET               (0x1)
#define BO_DATA_ROUND_ON_GIVE_NEXT_OFFSET                  (0x2)
#define BO_DATA_ROUND_VIEW_PLAYER_OBJECT_VIEW_CHAIR_OFFSET                  (0x15)

////////////////////UI position define///////////////////
// here are some parameters for UI when the game main ui is in the center of screen(not in the full screen).
// there is no influence about windows pantograph ratio,such as 124% or 100%.

#define BO_DATA_UI_WINDOW_CLASS_NAME                    ("HLDDZ")
#define BO_DATA_UI_PIXEL_CARD_WIDTH                     (131)
#define BO_DATA_UI_PIXEL_SELECT_CARD_AREA               (40)
#define BO_DATA_UI_PIXEL_CARD_BOTTOM_DELTA              (150)
#define BO_DATA_UI_PIXEL_CARD_SELECTED_JUDGE_DELTA      (242)

GDPS_NAMESPACE_END

#endif //_HLDDZ_DATA_BIN_OFFSET_DEFS_H
