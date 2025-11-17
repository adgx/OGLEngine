
add_library(Logmanager STATIC logmanager.cpp)
target_include_directories(Logmanager PRIVATE ${CMAKE_SOURCE_DIR}/include/managers)
target_link_libraries(Logmanager PUBLIC spdlog
    )
#macros
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Macro SPACE_ENGINE_DEBUG defined")
    target_compile_definitions(Logmanager PUBLIC SPACE_ENGINE_DEBUG=1)
endif()
set_target_properties(Logmanager PROPERTIES FOLDER "Logmanager")
