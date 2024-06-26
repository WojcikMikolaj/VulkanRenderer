//
// Created by Mikołaj Wójcik on 29.10.2023.
//
module;

#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

export module Vertices;

export class Vertex_p2_c3_t2 {
public:
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex_p2_c3_t2);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex_p2_c3_t2, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 2;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex_p2_c3_t2, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 3;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex_p2_c3_t2, texCoord);

        return attributeDescriptions;
    }
};
