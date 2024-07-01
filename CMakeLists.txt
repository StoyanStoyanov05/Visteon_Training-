cmake_minimum_required(VERSION 3.12)
project(GLTFLOADER VERSION 1.0)
set(CMAKE_CXX_STANDRAT 14)
set(CMAKE_CXX_STANDRAT_REQUIRED True)
add_executable(GLTFLoader
    src/main.cpp
)

set(GLFW_BUILD_WAYLAND OFF)

add_subdirectory(thirdparty/glfw/)
target_include_directories(GLTFLoader PRIVATE
# 	
)
target_link_libraries(GLTFLoader PRIVATE
    glfw
)
if(WIN32)
    target_link_libraries(GLTFLoader PRIVATE
        opengl32
    )
    elseif(UNIX)
    target_link_libraries(GLTFLoader PRIVATE 
        GL
    )
    endif()