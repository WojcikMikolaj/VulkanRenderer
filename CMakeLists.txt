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
            Wrapper/Debug/DebugMessenger.cxx
            Wrapper/Wrapper.cxx
            Wrapper/HelperStructs/QueueFamilyIndices.cxx
            Wrapper/HelperStructs/SwapChainSupportDetails.cxx
            Wrapper/HelperStructs/VertexStructs/Vertices.cxx
            Wrapper/HelperStructs/UBO/UBO.cxx
            Wrapper/Debug/ValidationLayers.cxx
            Wrapper/Instance.cxx
            Wrapper/AppInfo.cxx
            Wrapper/PhysicalDevice.cxx
            Wrapper/Surface.cxx
            Wrapper/Window.cxx
            Wrapper/LogicalDevice.cxx
            Wrapper/SwapChain.cxx
            Wrapper/ImageView.cxx
            Wrapper/Image.cxx
            Wrapper/HelperStructs/ImageInfo.cxx
            Wrapper/MemoryHelper.cxx)

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
