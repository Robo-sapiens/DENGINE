find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(graphics_sources
        src/gobject.cpp
        src/gcontext.cpp
        src/gwindow.cpp
        )

add_library(graphics STATIC ${graphics_sources})
target_include_directories(graphics PUBLIC
        include)
target_link_libraries(graphics PUBLIC
        ${OPENGL_LIBRARIES}
        glfw
        glad
        )

add_subdirectory(example)

if (BUILD_TESTING)
    add_subdirectory(test)
endif (BUILD_TESTING)
