cmake_minimum_required(VERSION 3.8)
project(terrain) # The name of your choice for the project comes here

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# imgui stuff
include_directories(include)
file(GLOB INCLUDE "include/*.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

IF(WIN32)
# set stuff for windows
find_package(OpenGL REQUIRED)


# for windows
include_directories(C:/glfw-3.3.9.bin.WIN64/include/)
include_directories(C:/glew-2.1.0/include/)
include_directories(C:/glew-2.1.0/include/GL/)
include_directories(C:/GLM_0.9.9.8/g-truc-glm-bf71a83/)

link_directories(C:/glew-2.1.0/lib/Release/x64/)
link_directories(C:/glfw-3.3.9.bin.WIN64/lib-vc2019/)


include_directories(src/)

aux_source_directory(src SOURCE)

add_executable(${PROJECT_NAME} ${SOURCE} ${INCLUDE})

target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY} glew32s glfw3)

ELSE()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_PREFIX_PATH
    /encs # For ENCS lab computers
    /opt/local # Macports
)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED
    HINTS /encs/pkg/glfw-3.3.4/root # ENCS installation of glfw
)

# NOTE: ENCS glm installation is missing links to *.inl files so we need this line
include_directories(/encs/pkg/glm-0.9.9.8/root/include)

include_directories(src/)

aux_source_directory(src SOURCE)

add_executable(${PROJECT_NAME} ${SOURCE} ${INCLUDE})

target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW::glew glfw ${CMAKE_DL_LIBS})

ENDIF()
