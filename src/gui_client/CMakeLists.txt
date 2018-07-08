# CMake script for gui client application
# M. van de Ruit
# June 2018

# Configure project
cmake_minimum_required (VERSION 3.5)
set(PROJECT_NAME gui_client)
set(CMAKE_CXX_STANDARD 14) # wooh, experimental language stuff
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(${PROJECT_NAME} LANGUAGES CXX C)
add_definitions(-DDATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

# Build project
add_subdirectory(extern)
add_executable(${PROJECT_NAME} 
    ../shared/frame.c
    ../shared/byte_queue.c
    ../shared/comm_protocol.c
    ../shared/types.c
    src/client.cpp
    src/imgui_wrapper.cpp
    src/window.cpp
    src/protocol.cpp
    src/gui.cpp
    src/graphs.cpp
    src/state.cpp
    src/gains.cpp
    src/control.cpp
    src/background.cpp
    src/misc.cpp
    src/main.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ../shared
    include
    ${EXTERN_INCLUDES}
)
target_link_libraries(${PROJECT_NAME} ${EXTERN_LIBRARIES})

