cmake_minimum_required(VERSION 3.7)
project(mash_graph)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp MyShader.cpp MyShader.h Model.cpp Model.h)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/libs/include")
link_directories("${PROJECT_SOURCE_DIR}/libs")
add_executable(mash_graph ${SOURCE_FILES})
target_link_libraries (mash_graph SOIL glfw3 glew32s.lib ${OPENGL_LIBRARIES})



