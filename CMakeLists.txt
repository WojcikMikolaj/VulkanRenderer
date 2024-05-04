cmake_minimum_required(VERSION 3.28)
project(VulkanRenderer)

set(CMAKE_CXX_STANDARD 23)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(Vulkan REQUIRED FATAL_ERROR)

set(PROJECT_SOURCES main.cxx)
add_executable(VulkanRenderer)
target_sources(VulkanRenderer PRIVATE ${PROJECT_SOURCES}
        PUBLIC
        FILE_SET CXX_MODULES FILES
        Engine/Debug/DebugMessenger.cxx
        Engine/Engine.cxx
        Engine/HelperStructs/QueueFamilyIndices.cxx
        Engine/HelperStructs/SwapChainSupportDetails.cxx
        Engine/HelperStructs/VertexStructs/Vertices.cxx
        Engine/HelperStructs/UBO/UBO.cxx
        Engine/Debug/ValidationLayers.cxx
        Engine/Instance.cxx
        Engine/AppInfo.cxx
        Engine/PhysicalDevice.cxx
        Engine/Surface.cxx
        Engine/Window.cxx
        Engine/LogicalDevice.cxx
        Engine/SwapChain.cxx
        Engine/ImageView.cxx
        Engine/Image.cxx
        Engine/HelperStructs/ImageInfo.cxx
        Engine/MemoryHelper.cxx
        Engine/Buffer.cxx
        Engine/CommandBuffers/CommandBuffer.cxx
        Engine/CommandPool.cxx
        Engine/CommandBuffers/SingleTimeCommandBuffer.cxx
        Engine/Sampler.cxx)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/vert.spv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Shaders/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/frag.spv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Shaders/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Textures/texture.jpg
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Textures/)

include_directories($ENV{VULKAN_SDK}/Include)
link_directories($ENV{VULKAN_SDK}/Lib;$ENV{VULKAN_SDK}/Bin)
target_link_libraries(VulkanRenderer Vulkan::Vulkan)

add_subdirectory(glfw)
target_link_libraries(VulkanRenderer glfw)

add_subdirectory(glm)
target_link_libraries(VulkanRenderer glm)

add_subdirectory(stb)
target_link_libraries(VulkanRenderer stb_image)

add_executable (glslang::validator IMPORTED)
find_program (GLSLANG_VALIDATOR "glslangValidator" HINTS $ENV{VULKAN_SDK}/bin REQUIRED)
set_property (TARGET glslang::validator PROPERTY IMPORTED_LOCATION "${GLSLANG_VALIDATOR}")
