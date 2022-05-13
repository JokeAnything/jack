#ifndef _HLDDZ_DATA_BIN_OFFSET_DEFS_H
#define _HLDDZ_DATA_BIN_OFFSET_DEFS_H

#include "../game_data_provider_service_defs.h"

GDPS_NAMESPACE_BEGIN

#define BO_BUTTON_OBJECT_COLLECT_PROC_OFFSET            (0x010c6738)
#define BO_BUTTON_OBJECT_COLLECT_PROC_ADDRESS(base)     (base+BO_BUTTON_OBJECT_COLLECT_PROC_OFFSET)

#define BO_BUTTON_OBJECT_SHOW_PROC_OFFSET            (0x00a48c40)
#define BO_BUTTON_OBJECT_SHOW_PROC_ADDRESS(base)     (base+BO_BUTTON_OBJECT_SHOW_PROC_OFFSET)

#define BO_BUTTON_OBJECT_CLICK_PROC_OFFSET               (0x852750)
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

// sub_46BF80()
#define BO_DATA_ROUND_MGR_BASE_PTR_PROC_OFFSET              (0x6BF80)
#define BO_DATA_ROUND_MGR_BASE_PTR_PROC_ADDRESS(base)       (base+BO_DATA_ROUND_MGR_BASE_PTR_PROC_OFFSET)

// sub_46CDA0
#define BO_DATA_ROUND_VIEW_PLAYER_OBJECT_PTR_PROC_OFFSET              (0x6CDA0)
#define BO_DATA_ROUND_VIEW_PLAYER_OBJECT_PTR_PROC_ADDRESS(base)       (base+BO_DATA_ROUND_VIEW_PLAYER_OBJECT_PTR_PROC_OFFSET)

// 
#define BO_DATA_ROUND_GIVE_NOTIFY_PROC_OFFSET               (0x16E263)
#define BO_DATA_ROUND_GIVE_NOTIFY_PROC_ADDRESS(base)        (base+BO_DATA_ROUND_GIVE_NOTIFY_PROC_OFFSET)

// sub_CF7C10
#define BO_DATA_ROUND_TIMER_UPDATE_PROC_OFFSET               (0x8F7C10)
#define BO_DATA_ROUND_TIMER_UPDATE_PROC_ADDRESS(base)        (base+BO_DATA_ROUND_TIMER_UPDATE_PROC_OFFSET)

// sub_751230(round started)(middle)(8bytes)
#define BO_DATA_ROUND_GAME_START_PROC_OFFSET               (0x3513D7)
#define BO_DATA_ROUND_GAME_START_PROC_ADDRESS(base)        (base+BO_DATA_ROUND_GAME_START_PROC_OFFSET)

// sub_7530CE(game bottom cards notify)(middle)(13bytes)
#define BO_DATA_ROUND_GAME_BOTTOM_CARDS_NOTIFY_PROC_OFFSET               (0x3530EB)
#define BO_DATA_ROUND_GAME_BOTTOM_CARDS_NOTIFY_PROC_ADDRESS(base)        (base+BO_DATA_ROUND_GAME_BOTTOM_CARDS_NOTIFY_PROC_OFFSET)

#define BO_DATA_ROUND_ON_GIVE_CURRENT_OFFSET               (0x1)
#define BO_DATA_ROUND_ON_GIVE_NEXT_OFFSET                  (0x2)
#define BO_DATA_ROUND_VIEW_PLAYER_OBJECT_VIEW_CHAIR_OFFSET                  (0x15)

////////////////////UI position define///////////////////
#define BO_DATA_UI_WINDOW_CLASS_NAME                    ("HLDDZ")
#define BO_DATA_UI_PIXEL_CARD_WIDTH                     (150)
#define BO_DATA_UI_PIXEL_SELECT_CARD_AREA               (46)
#define BO_DATA_UI_PIXEL_CARD_BOTTOM_DELTA              (150)

GDPS_NAMESPACE_END

#endif //_HLDDZ_DATA_BIN_OFFSET_DEFS_H
