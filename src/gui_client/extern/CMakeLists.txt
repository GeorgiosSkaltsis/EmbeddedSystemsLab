cmake_minimum_required (VERSION 3.5)

# Dependencies folder
set(EXTERN_DIR "extern")

# GLFW source
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
set_property(TARGET glfw PROPERTY FOLDER "${EXTERN_DIR}/glfw")
set_property(TARGET uninstall PROPERTY FOLDER "${EXTERN_DIR}/glfw")

# GLEW source
add_subdirectory(glew/build/cmake)
set_property(TARGET glew PROPERTY FOLDER "${EXTERN_DIR}/glew")
set_property(TARGET glew_s PROPERTY FOLDER "${EXTERN_DIR}/glew")
set_property(TARGET glewinfo PROPERTY FOLDER "${EXTERN_DIR}/glew")
set_property(TARGET visualinfo PROPERTY FOLDER "${EXTERN_DIR}/glew")

# imgui source. No CMakeLists provides, do it ourselves
add_subdirectory(imgui)
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/glew/include)

# Add to list of linked libraries and headers.
set(EXTERN_LIBRARIES 
    glfw 
    glew_s
    imgui
    CACHE INTERNAL "" FORCE)
set(EXTERN_INCLUDES 
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/glew/include
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/include
    CACHE INTERNAL "" FORCE)
