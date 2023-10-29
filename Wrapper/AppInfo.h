//
// Created by Mikołaj Wójcik on 29.10.2023.
//

#ifndef VULKANRENDERER_APPINFO_H
#define VULKANRENDERER_APPINFO_H

#include <string>

class AppInfo{
private:
    std::string AppName;
    std::string EngineName;

public:
    AppInfo(std::string AppName, std::string EngineName)
    {
        this->AppName = std::string(AppName);
        this->EngineName = std::string(AppName);
    }

    const char* GetAppName();
    uint32_t GetAppMajorVersion();
    uint32_t GetAppMinorVersion();
    uint32_t GetAppPatchVersion();

    const char* GetEngineName();
    uint32_t GetEngineMajorVersion();
    uint32_t GetEngineMinorVersion();
    uint32_t GetEnginePatchVersion();
};
#endif //VULKANRENDERER_APPINFO_H
