#include "hlddz_game_data_service_api_impl.h"
#include <Tlhelp32.h>

GDPS_NAMESPACE_BEGIN
gdps_bool hlddz_game_data_service_impl::initialize()
{
    return false;
}
gdps_void hlddz_game_data_service_impl::uninitialize()
{

}
auto  hlddz_game_data_service_impl::get_current_turning_role()
{

}
auto  hlddz_game_data_service_impl::get_role_card_number(role_position role_id)
{

}
auto hlddz_game_data_service_impl::get_role_hand_cards(role_position role_id)
{

}
auto hlddz_game_data_service_impl::get_role_given_cards(role_position role_id)
{

}

GDPS_NAMESPACE_END

HMODULE GethlddzProcessBase()
{
    //��ȡ���̻�ַ
    HANDLE hSnapShot;
    //ͨ��CreateToolhelp32Snapshot���߳�ID����ȡ���̿���
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }
    MODULEENTRY32 ModuleEntry32;
    ModuleEntry32.dwSize = sizeof(ModuleEntry32);
    if (Module32First(hSnapShot, &ModuleEntry32))
    {
        do
        {
            TCHAR szExt[5];
            strcpy(szExt, ModuleEntry32.szExePath + strlen(ModuleEntry32.szExePath) - 4);
            for (int i = 0; i < 4; i++)
            {
                if ((szExt[i] >= 'a') && (szExt[i] <= 'z'))
                {
                    szExt[i] = szExt[i] - 0x20;
                }
            }
            if (!strcmp(szExt, ".EXE"))
            {
                CloseHandle(hSnapShot);
                return ModuleEntry32.hModule;
            }
        } while (Module32Next(hSnapShot, &ModuleEntry32));
    }
    CloseHandle(hSnapShot);
    return NULL;

}

DWORD GetProcessID(const char *ProcessName)
{
    PROCESSENTRY32 pe32;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    //��ȡ���̿���
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot ����ʧ��.\n");
        return -1;
    }

    //�������̿��գ�������ʾÿ�����̵���Ϣ
    BOOL bMore = ::Process32First(hProcessSnap, &pe32);
    while (bMore)
    {
        printf("�������ƣ�%ls\n", pe32.szExeFile); //����õ���Ӧ���ǿ��ַ�����%ls,��Ȼ�޷�������ӡ
        printf("����ID��%u\n\n", pe32.th32ProcessID);
        if (lstrcmp(pe32.szExeFile, ProcessName) == 0)
        {
            break;
        }

        bMore = ::Process32Next(hProcessSnap, &pe32);
    }

    //��Ҫ���������snapshot����
    ::CloseHandle(hProcessSnap);

    return pe32.th32ProcessID;
}
