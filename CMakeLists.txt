cmake_minimum_required(VERSION 3.20)
project(BrownianMotion)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(COPY src/assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
file(COPY src/shaders/ DESTINATION ${CMAKE_BINARY_DIR}/shaders)
file(COPY src/Roboto/ DESTINATION ${CMAKE_BINARY_DIR}/Roboto)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14")
# set(source_dir "${PROJECT_SOURCE_DIR}/src/")

# file (GLOB source_files "${source_dir}/*.cpp" "${source_dir}/*.h")

set(source_files src/main.cpp
        src/headers/stb_image.cpp           src/headers/stb_image.h
        src/headers/resource_manager.cpp    src/headers/resource_manager.h
        src/headers/shader.cpp              src/headers/shader.h
        src/headers/texture.cpp             src/headers/texture.h
        src/headers/SpriteRenderer.cpp      src/headers/SpriteRenderer.h
        src/headers/RigidBody.cpp           src/headers/RigidBody.h
        src/headers/Simulation.cpp          src/headers/Simulation.h
        src/headers/window_maker.cpp        src/headers/window_maker.h
        src/headers/sprite.cpp              src/headers/sprite.h
                                            src/headers/Shape.h
        src/headers/SandBox2D.cpp           src/headers/SandBox2D.h
)


add_executable(BrownianMotion ${source_files})

add_library(
    Glad STATIC
    "${PROJECT_SOURCE_DIR}/src/glad.c"
)


#target_include_directories(Glad PUBLIC "/usr/include/glad")
#target_include_directories(Glad PUBLIC "/usr/include/glad")
#link_directories("/usr/include/glad")


set(IMGUI_PATH  "${PROJECT_SOURCE_DIR}/src/imgui/")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIR_freetype2})
include_directories(${FREETYPE_INCLUDE_DIR_ft2build})



find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(BrownianMotion glfw OpenGL::GL Glad ImGui freetype)
