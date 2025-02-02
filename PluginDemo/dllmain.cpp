﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "PluginSystemDate.h"
#include "PluginSystemTime.h"
#include "PluginDemo.h"
#include "DataManager.h"
#include "OptionsDlg.h"

//BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//                     )
//{
//    switch (ul_reason_for_call)
//    {
//    case DLL_PROCESS_ATTACH:
//    case DLL_THREAD_ATTACH:
//    case DLL_THREAD_DETACH:
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//    return TRUE;
//}

IPluginItem* TMPluginCreateInstance(int index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    switch (index)
    {
    case 0:
        return new CPluginSystemDate();
    case 1:
        return new CPluginSystemTime();
    default:
        break;
    }
    return nullptr;
}

void TMPluginInfoRequired()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //获取时间和日期
    SYSTEMTIME system_time;
    GetSystemTime(&system_time);
    wchar_t buff[128];
    swprintf_s(buff, L"%d/%.2d/%.2d", system_time.wYear, system_time.wMonth, system_time.wDay);
    CDataManager::Instance().m_cur_date = buff;

    if (CDataManager::Instance().m_setting_data.show_second)
        swprintf_s(buff, L"%.2d:%.2d:%.2d", system_time.wHour, system_time.wMinute, system_time.wSecond);
    else
        swprintf_s(buff, L"%.2d:%.2d", system_time.wHour, system_time.wMinute);
    CDataManager::Instance().m_cur_time = buff;
}

void TMPluginOptions(HWND hParent)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    COptionsDlg dlg(CWnd::FromHandle(hParent));
    dlg.m_data = CDataManager::Instance().m_setting_data;
    if (dlg.DoModal() == IDOK)
    {
        CDataManager::Instance().m_setting_data = dlg.m_data;
    }
}

const wchar_t* TMPluginGetInfo(int index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    static CString str;
    switch (index)
    {
    case TMI_NAME:
        str.LoadString(IDS_PLUGIN_NAME);
        return str.GetString();
    case TMI_DESCRIPTION:
        str.LoadString(IDS_PLUGIN_DESCRIPTION);
        return str.GetString();
    case TMI_AUTHOR:
        return L"zhongyang219";
    case TMI_COPYRIGHT:
        return L"Copyright (C) by Zhong Yang 2021";
    default:
        break;
    }
    return L"";
}
