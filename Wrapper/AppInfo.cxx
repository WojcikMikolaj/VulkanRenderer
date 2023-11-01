//
// Created by Mikołaj Wójcik on 29.10.2023.
//
module;

import <string>;

export module AppInfo;

export class AppInfo
{
private:
    std::string AppName;
    std::string EngineName;

public:
    AppInfo(std::string AppName, std::string EngineName)
    {
        this->AppName = std::string(AppName);
        this->EngineName = std::string(AppName);
    }

    const char *AppInfo::GetAppName()
    {
        return AppName.c_str();
    }

    uint32_t AppInfo::GetAppMajorVersion()
    {
        return 1;
    }

    uint32_t AppInfo::GetAppMinorVersion()
    {
        return 0;
    }

    uint32_t AppInfo::GetAppPatchVersion()
    {
        return 0;
    }

    const char *AppInfo::GetEngineName()
    {
        return EngineName.c_str();
    }

    uint32_t AppInfo::GetEnginePatchVersion()
    {
        return 0;
    }

    uint32_t AppInfo::GetEngineMinorVersion()
    {
        return 0;
    }

    uint32_t AppInfo::GetEngineMajorVersion()
    {
        return 0;
    }
};