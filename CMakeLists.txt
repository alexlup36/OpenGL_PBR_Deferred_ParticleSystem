cmake_minimum_required(VERSION 3.7)

project(openglframework)

find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SOIL REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES} "main.cpp")
target_link_libraries(${PROJECT_NAME} glm GLEW glfw assimp SOIL ${OPENGL_LIBRARY})