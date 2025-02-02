﻿#include "stdafx.h"
#include "PluginManager.h"
#include "Common.h"

CPluginManager::CPluginManager()
{
}

static wstring WcharArrayToWString(const wchar_t* str)
{
    if (str == nullptr)
        return wstring();
    else
        return wstring(str);
}

void CPluginManager::LoadPlugins()
{
    //从plugins目录下加载插件
    wstring plugin_dir;
    plugin_dir = CCommon::GetModuleDir() + L"plugins";
    vector<wstring> plugin_files;
    CCommon::GetFiles((plugin_dir + L"\\*.dll").c_str(), plugin_files);		//获取Plugins目录下所有的dll文件的文件名
    for (const auto& file : plugin_files)
    {
        //插件信息
        m_modules.push_back(PluginInfo());
        PluginInfo& plugin_info{ m_modules.back() };
        //插件dll的路径
        plugin_info.file_path = plugin_dir + file;
        //载入dll
        plugin_info.plugin_module = LoadLibrary(plugin_info.file_path.c_str());
        if (plugin_info.plugin_module == NULL)
        {
            plugin_info.state = PluginState::PS_MUDULE_LOAD_FAILED;
            plugin_info.error_code = GetLastError();
            continue;
        }
        //获取函数的入口地址
        pfTMPluginCreateInstance TMPluginCreateInstance = (pfTMPluginCreateInstance)::GetProcAddress(plugin_info.plugin_module, "TMPluginCreateInstance");
        plugin_info.MPluginInfoRequired = (pfTMPluginInfoRequired)::GetProcAddress(plugin_info.plugin_module, "TMPluginInfoRequired");
        if (TMPluginCreateInstance == NULL || plugin_info.MPluginInfoRequired == NULL)
        {
            plugin_info.state = PluginState::PS_FUNCTION_GET_FAILED;
            plugin_info.error_code = GetLastError();
            continue;
        }
        plugin_info.TMPluginOptions = (pfTMPluginOptions)::GetProcAddress(plugin_info.plugin_module, "TMPluginOptions");
        //获取插件信息
        pfTMPluginGetInfo TMPluginGetInfo = (pfTMPluginGetInfo)::GetProcAddress(plugin_info.plugin_module, "TMPluginGetInfo");
        if (TMPluginGetInfo != nullptr)
        {
            plugin_info.name = WcharArrayToWString(TMPluginGetInfo(TMI_NAME));
            plugin_info.description = WcharArrayToWString(TMPluginGetInfo(TMI_DESCRIPTION));
            plugin_info.author = WcharArrayToWString(TMPluginGetInfo(TMI_AUTHOR));
            plugin_info.copyright = WcharArrayToWString(TMPluginGetInfo(TMI_COPYRIGHT));
        }

        //获取插件显示项目
        int index = 0;
        while (true)
        {
            std::shared_ptr<IPluginItem> item = std::shared_ptr<IPluginItem>(TMPluginCreateInstance(index));
            if (item == nullptr)
                break;
            plugin_info.plugin_items.push_back(item);
            m_plugins.push_back(item);
            index++;
        }
    }
}

const std::vector<std::shared_ptr<IPluginItem>>& CPluginManager::GetPluginItems()
{
    return m_plugins;
}

const std::vector<CPluginManager::PluginInfo>& CPluginManager::GetPlugins()
{
    return m_modules;
}

std::shared_ptr<IPluginItem> CPluginManager::GetItemByName(const std::wstring& item_name)
{
    for (const auto& item : m_plugins)
    {
        if (item->GetPluginName() == item_name)
            return item;
    }
    return nullptr;
}
