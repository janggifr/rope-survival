cmake_minimum_required(VERSION 3.24)
 
project(LAB3_MESH_PROCESSING)
  
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src/thirdparties/glm)
add_subdirectory(src/thirdparties/glad)
add_subdirectory(src/thirdparties/glfw)
add_subdirectory(src/thirdparties/imgui)
### IMGUI

### END IMGUI

file(GLOB SOURCE_FILES
    "src/*.*"
)

file(GLOB HELPERS_FILES
    "src/helpers/*.*"
)

file(GLOB SHADER_FILES
    "shaders/*.*"
)


add_executable(RopeSurvival
${SOURCE_FILES}
${HELPERS_FILES}
${SHADER_FILES})

# Add the directory of include files
include_directories(src/thirdparties)

target_compile_definitions(RopeSurvival PRIVATE ROOT_FOLDER=\"${CMAKE_SOURCE_DIR}/\")
target_link_libraries(
        RopeSurvival
        imgui
        glm
        glad
        glfw
)
 
source_group("sources" FILES ${SOURCE_FILES})
source_group("helpers" FILES ${HELPERS_FILES})
source_group("shaders" FILES ${SHADER_FILES}) 
