﻿#pragma once
#include "PluginInterface.h"

class CPluginSystemTime : public IPluginItem
{
public:
    // 通过 IPluginItem 继承
    virtual const wchar_t* GetPluginName() const override;
    virtual const wchar_t* GetItemLableText() const override;
    virtual const wchar_t* GetItemValueText() const override;
    virtual const wchar_t* GetItemValueSampleText() const override;
};
