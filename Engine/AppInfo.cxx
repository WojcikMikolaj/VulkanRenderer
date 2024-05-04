//
// Created by Mikołaj Wójcik on 29.10.2023.
//
module;

#include <string>

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
        this->EngineName = std::string(EngineName);
    }

    const char* GetAppName()
    {
        return AppName.c_str();
    }

    uint32_t GetAppMajorVersion()
    {
        return 1;
    }

    uint32_t GetAppMinorVersion()
    {
        return 0;
    }

    uint32_t GetAppPatchVersion()
    {
        return 0;
    }

    const char* GetEngineName()
    {
        return EngineName.c_str();
    }

    uint32_t GetEnginePatchVersion()
    {
        return 0;
    }

    uint32_t GetEngineMinorVersion()
    {
        return 0;
    }

    uint32_t GetEngineMajorVersion()
    {
        return 0;
    }
};
