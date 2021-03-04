cmake_minimum_required(VERSION 3.17)
project(Draw)

set(CMAKE_CXX_STANDARD 20)

set(glew_PATH ${CMAKE_SOURCE_DIR}/lib/glew/)
set(glfw3_PATH ${CMAKE_SOURCE_DIR}/lib/glfw3/)

find_package(OpenGL REQUIRED)
message(STATUS "OPENGL_FOUND:= ${OPENGL_FOUND}")
message(STATUS "OPENGL_LIBRARY:= ${OPENGL_LIBRARY}")
message(STATUS "OPENGL_LIBRARIES:= ${OPENGL_LIBRARIES}")

# Vertex shaders
set(REQUIRED_SHADERS ${REQUIRED_SHADERS} vertex.shader)
set(REQUIRED_SHADERS ${REQUIRED_SHADERS} vertex_lamp.shader)
set(REQUIRED_SHADERS ${REQUIRED_SHADERS} vertex_render.shader)

# Fragment shaders
set(REQUIRED_SHADERS ${REQUIRED_SHADERS} fragment.shader)
set(REQUIRED_SHADERS ${REQUIRED_SHADERS} fragment_lamp.shader)
set(REQUIRED_SHADERS ${REQUIRED_SHADERS} fragment_render.shader)

# Textures
set(REQUIRED_TEXTURES ${REQUIRED_TEXTURES} diffuse.jpg)
set(REQUIRED_TEXTURES ${REQUIRED_TEXTURES} normals.jpg)
set(REQUIRED_TEXTURES ${REQUIRED_TEXTURES} parallax.jpg)

add_executable(${PROJECT_NAME}
        main.cpp
        include/shader.h
        include/texture.h
        include/camera.h
        include/object.h
        include/lamp.h
        )

add_subdirectory(lib/soil)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        lib/soil/inc
        )

target_link_libraries(${PROJECT_NAME}
        ${glew_PATH}/glew32s.lib
        ${glfw3_PATH}/glfw3.lib
        ${OPENGL_LIBRARIES} soil
        )

# Install shaders and textures
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/textures/")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/")

foreach (SHADER ${REQUIRED_SHADERS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/shaders/${SHADER}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/")
endforeach (SHADER)

foreach (TEXTURE ${REQUIRED_TEXTURES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/textures/${TEXTURE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/textures/")
endforeach (TEXTURE)

# Install glfw3

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${glfw3_PATH}glfw3.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")