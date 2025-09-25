cmake_minimum_required(VERSION 3.0.0)
project(OGLEngine LANGUAGES C CXX)

set(OSBitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()
#Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(OutputDir "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}${OSBitness}/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OutputDir}/static_libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OutputDir}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OutputDir}")

#use the --config Debug option for select the configuration  
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include(FetchContent)
#Fetch GLFW
Fetchcontent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)
#Fetch GLAD
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad
    GIT_TAG 2.0.8
)
FetchContent_MakeAvailable(glad)

#link
target_link_libraries(main PRIVATE glfw glad)

#include GLAD headers
target_include_directories(main PRIVATE ${glad_SOURCE_DIR}/include)