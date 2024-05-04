//
// Created by Mikołaj Wójcik on 05.11.2023.
//
module;

#include <memory>
#include <vulkan/vulkan.h>

import CommandBuffer;
import CommandPool;
import LogicalDevice;

export module SingleTimeCommandBuffer;

export class SingleTimeCommandBuffer : CommandBuffer
{
    bool submitted;
    std::shared_ptr<CommandPool> pCommandPool;
    std::shared_ptr<LogicalDevice> pLogicalDevice;
public:
    VkCommandBuffer commandBuffer;

    SingleTimeCommandBuffer(std::shared_ptr<CommandPool> pCommandPool, std::shared_ptr<LogicalDevice> pLogicalDevice)
    {
        this->pCommandPool = pCommandPool;
        this->pLogicalDevice = pLogicalDevice;
        submitted = false;

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = this->pCommandPool->commandPool;
        allocInfo.commandBufferCount = 1;

        vkAllocateCommandBuffers(this->pLogicalDevice->device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
    }

    void EndAndSubmitBuffer()
    {
        if(!submitted)
        {
            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            vkQueueSubmit(pLogicalDevice->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(pLogicalDevice->graphicsQueue);

            vkFreeCommandBuffers(pLogicalDevice->device, this->pCommandPool->commandPool, 1, &commandBuffer);

            pCommandPool.reset();
            pLogicalDevice.reset();

            submitted = true;
        }
    }

    ~SingleTimeCommandBuffer()
    {
        EndAndSubmitBuffer();
    }
};