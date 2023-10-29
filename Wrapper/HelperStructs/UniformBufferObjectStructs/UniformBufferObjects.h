//
// Created by Mikołaj Wójcik on 29.10.2023.
//

#ifndef VULKANRENDERER_UNIFORMBUFFEROBJECTS_H
#define VULKANRENDERER_UNIFORMBUFFEROBJECTS_H

struct UniformBufferObject_M4_V4_P4 {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

#endif //VULKANRENDERER_UNIFORMBUFFEROBJECTS_H
