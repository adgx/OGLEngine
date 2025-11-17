add_library(Logmanager STATIC logmanager.cpp)
target_include_directories(Logmanager PRIVATE ${CMAKE_SOURCE_DIR}/include/managers)
target_link_libraries(Logmanager PRIVATE spdlog
    )
set_target_properties(Logmanager PROPERTIES FOLDER "Logmanager")