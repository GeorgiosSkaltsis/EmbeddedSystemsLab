cmake_minimum_required(VERSION 3.5)

set(LIBRARY_NAME imgui)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE INCLUDE_FILES "*.h" "*.hpp")
file(GLOB_RECURSE SRC_FILES "*.c" "*.cpp")

# Assemble library
add_library(${LIBRARY_NAME} STATIC ${SRC_FILES} ${INCLUDE_FILES})
target_link_libraries(${LIBRARY_NAME} 
    glfw 
    glew_s)

# Structure into external group for visual studio
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER "${EXTERN_DIR}/imgui")