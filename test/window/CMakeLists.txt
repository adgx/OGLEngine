add_executable(Window
    main.cpp)

target_link_libraries(Window PRIVATE OpenGL::GL)
#set_target_properties(Window PROPERTIES FOLDER "Tests")