//
// Created by Mikołaj Wójcik on 29.10.2023.
//

#ifndef VULKANRENDERER_QUEUEFAMILYINDICES_H
#define VULKANRENDERER_QUEUEFAMILYINDICES_H

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value()
               && presentFamily.has_value();
    }
};

#endif //VULKANRENDERER_QUEUEFAMILYINDICES_H
