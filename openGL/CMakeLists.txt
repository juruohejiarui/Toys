cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)

project(flower)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(BUILD)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)


aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(
    ${PROJECT_NAME}
    OpenGL::GL          # OpenGL 库
    glfw                # GLFW 库
    GLEW
)

