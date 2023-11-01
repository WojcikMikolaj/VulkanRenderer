cmake_minimum_required(VERSION 3.8)
project(VulkanRenderer)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(Vulkan REQUIRED FATAL_ERROR)

set(PROJECT_SOURCES main.cpp)

add_executable(VulkanRenderer Wrapper/Wrapper.cpp Wrapper/Wrapper.h Wrapper/HelperStructs/QueueFamilyIndices.h Wrapper/HelperStructs/SwapChainSupportDetails.h Wrapper/HelperStructs/VertexStructs/Vertices.h Wrapper/HelperStructs/UniformBufferObjectStructs/UniformBufferObjects.h Wrapper/Debug/ValidationLayers.cpp Wrapper/Debug/ValidationLayers.h Wrapper/Instance.cpp Wrapper/Instance.h Wrapper/AppInfo.cpp Wrapper/AppInfo.h Wrapper/Debug/DebugMessenger.h Wrapper/Device.cpp Wrapper/Device.h Wrapper/PhysicalDevice.cpp Wrapper/PhysicalDevice.h)
target_sources(VulkanRenderer PRIVATE ${PROJECT_SOURCES})

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
