//
// Created by Mikołaj Wójcik on 29.10.2023.
//
module;

#include <glm/glm.hpp>

export module UBO;

export class UBO_M4_V4_P4 {
public:
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};
