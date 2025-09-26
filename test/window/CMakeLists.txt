add_executable(Window
    main.cpp)

target_link_libraries(Window PRIVATE glfw
    PRIVATE glad_gl_core_mx_33
    PRIVATE OpenGL::GL)
    
set_target_properties(Window PROPERTIES FOLDER "Tests")