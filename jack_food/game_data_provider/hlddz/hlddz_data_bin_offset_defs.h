#ifndef _HLDDZ_DATA_BIN_OFFSET_DEFS_H
#define _HLDDZ_DATA_BIN_OFFSET_DEFS_H

#include "../game_data_provider_service_defs.h"

GDPS_NAMESPACE_BEGIN

#define BO_BUTTON_OBJECT_COLLECT_PROC_OFFSET            (0x010c6738)
#define BO_BUTTON_OBJECT_COLLECT_PROC_ADDRESS(base)     (base+BO_BUTTON_OBJECT_COLLECT_PROC_OFFSET)
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


GDPS_NAMESPACE_END

#endif //_HLDDZ_DATA_BIN_OFFSET_DEFS_H
