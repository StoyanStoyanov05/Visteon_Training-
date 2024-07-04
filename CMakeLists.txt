cmake_minimum_required(VERSION 3.12)
project(GLTFLOADER VERSION 1.0)
set(CMAKE_CXX_STANDRAT 14)
set(CMAKE_CXX_STANDRAT_REQUIRED True)
add_executable(GLTFLoader
    src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/src/glad.c
)

    if(WIN32)
    target_include_directories(GLTFLoader PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
        )

endif()

 target_include_directories(GLTFLoader PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tinyGLTF
        )

    target_compile_definitions(GLTFLoader PRIVATE
        -DTINYGLTF_IMPLEMENTATION
        -DSTB_IMAGE_IMPLEMENTATION
        -DSTB_IMAGE_WRITE_IMPLEMENTATION
   )
    
    

set(GLFW_BUILD_WAYLAND OFF)

add_subdirectory(thirdparty/glfw/)

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