#include <windows.h>
#include "utils/game_process_utils.h"
#include "ai\ai_service_factory.h"
#include "game_data_provider\hlddz\hlddz_data_bin_offset_defs.h"
#include "game_data_provider/hlddz/data_service_game_api.h"

#include <future>
#include <iostream>

HMODULE g_hModule = NULL;
ai_service_object_ptr g_ai_service_object_ptr = nullptr;

void start_data_service()
{
    g_ai_service_object_ptr = ai_service_factory::create_ai_service_object(ai_service_type_hlddz);
    if (g_ai_service_object_ptr->initialize())
    {
        g_ai_service_object_ptr->start_ai_service();
    }
}

void stop_data_service()
{
    if (g_ai_service_object_ptr)
    {
        g_ai_service_object_ptr->stop_ai_service();
        g_ai_service_object_ptr->uninitialize();
        ai_service_factory::destroy_ai_service_object(g_ai_service_object_ptr);
    }
}

void reset_game()
{
    if (g_ai_service_object_ptr)
    {
        g_ai_service_object_ptr->reset_game();
    }
}

HANDLE g_handle = NULL;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    ::AllocConsole();
    FILE *stream = nullptr;
    ::freopen_s(&stream, "CONOUT$", "w+t", stdout);
    ::freopen_s(&stream, "CONIN$", "r+t", stdin);
    while (1)
    {
        std::string cmd = "";
        getline(std::cin, cmd);
        if (cmd == "start")
        {
            start_data_service();
        }
        else if (cmd == "stop")
        {
            stop_data_service();
        }
        else if (cmd == "reset_game")
        {
            reset_game();
        }
        else if (cmd == "break")
        {
            if (::IsDebuggerPresent())
            {
                ::DebugBreak();
            }
        }
        else if (cmd == "exit")
        {
            break;
        }
        else if (cmd == "select")
        {
            std::string cards = "";
            getline(std::cin, cards);
            auto ds_ptr = g_ai_service_object_ptr->get_data_service_game_api();
            //auto ds_ptr = GDPS::data_service_factory::get_data_service_object(GDPS::data_service_type_hlddz);
            if (ds_ptr != nullptr)
            {
                ds_ptr->select_hand_cards(cards);
            }
        }
        else if (cmd == "selected")
        {
            auto ds_ptr = g_ai_service_object_ptr->get_data_service_game_api();
            //auto ds_ptr = GDPS::data_service_factory::get_data_service_object(GDPS::data_service_type_hlddz);
            if (ds_ptr != nullptr)
            {
                GDPS::card_list list;
                ds_ptr->get_selected_cards(list);
            }
        }
        else if (strlen(cmd.c_str()) <= 2)
        {
            //auto type = std::strtol(cmd.c_str(), nullptr, 10);
            //auto ds_ptr = GDPS::data_service_factory::get_data_service_object(GDPS::data_service_type_hlddz);
            //if (ds_ptr != nullptr)
            //{
            //    auto api = ds_ptr->get_data_service_game_api();
            //    if (api)
            //    {
            //        api->execute_current_player_action((GDPS::player_action_type)type);
            //    }
            //}
        }
    }
    ::FreeConsole();
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        {
            DWORD dwTid = 0;
            auto handle = ::CreateThread(NULL, 0, ThreadProc, (LPVOID)1, 0, &dwTid);
            ::CloseHandle(handle);
            g_hModule = hModule;
        }
        break;
    case DLL_PROCESS_DETACH:
        {
        }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return (TRUE);
}

//#define CARDS_X_STARTING_POSITION(wnd_width, select_area, card_width, card_count) ((wnd_width / 2)-((card_width + ((card_count - 1) * select_area)) / 2))
//#define CARD_X_CLICK_POSITION(starting_pos, select_area, card_index) (starting_pos + card_index * select_area + select_area / 2)

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
//    auto wnd = game_process_utils::get_game_window(BO_DATA_UI_WINDOW_CLASS_NAME);
//    uint32_t wnd_width = 1920;
//    uint32_t wnd_height = 880;
//    game_process_utils::get_wnd_width_height(wnd, wnd_width, wnd_height);
//
//    //::Sleep(5000);
//    //for (int i = 0; i < 1440; i++)
//    //{
//    //    for (int j = 0; j < 840; j++)
//        //{
//        //    game_process_utils::post_left_button_down_message(wnd, 607, 883);
//        //}
////    }
//
//    auto total_cards = 18/2;
//    std::vector<uint32_t> pos_list;
//    for (auto index = 0; index <= total_cards - 1; index++)
//    {
//        if (index % 2 == 0)
//        {
//            continue;
//        }
//        //int index = 0;
//        auto starting_pos = CARDS_X_STARTING_POSITION(wnd_width, BO_DATA_UI_PIXEL_SELECT_CARD_AREA, BO_DATA_UI_PIXEL_CARD_WIDTH, total_cards);
//        auto x_pos = CARD_X_CLICK_POSITION(starting_pos, BO_DATA_UI_PIXEL_SELECT_CARD_AREA, index);
//        pos_list.push_back(x_pos);
//        auto y_pos = wnd_height - BO_DATA_UI_PIXEL_CARD_BOTTOM_DELTA;
//        game_process_utils::post_left_button_down_message(wnd, x_pos, y_pos);
//        //Sleep(2000);
//        //game_process_utils::post_left_button_down_message(wnd, x_pos, y_pos);
//    }

    //for (auto&& item : selected_list)
    //{
    //}
    //ThreadProc(0);
    //Sleep(5000);

    ThreadProc(nullptr);
    return 0;
}
