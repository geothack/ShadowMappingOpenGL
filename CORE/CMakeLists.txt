# Define the project class files and executable



set(SOURCES

Src/Classes/App.cpp

Src/Classes/Window.h
Src/Classes/Window.cpp
Src/Classes/Shader.h
Src/Classes/Shader.cpp
Src/Classes/Texture.h
Src/Classes/Texture.cpp
Src/Classes/Loader.h
Src/Classes/Loader.cpp
Src/Classes/Model.h
Src/Classes/Model.cpp
Src/Classes/Camera.h
Src/Classes/Camera.cpp
Src/Classes/Skybox.h
Src/Classes/Skybox.cpp
Src/Classes/Keyboard.h
Src/Classes/Keyboard.cpp
Src/Classes/Framebuffer.h
Src/Classes/Framebuffer.cpp
Src/Classes/Depthbuffer.h
Src/Classes/Depthbuffer.cpp
Src/Classes/GameObject.h
Src/Classes/GameObject.cpp
Src/Classes/Mouse.h
Src/Classes/Mouse.cpp
Src/Classes/GraphicsMain.h
Src/Classes/GraphicsMain.cpp
Src/Classes/Sound.h
Src/Classes/Sound.cpp
Src/Classes/Plane.h
Src/Classes/Plane.cpp

)



add_executable(Core ${SOURCES})
 


# Link common library
target_link_libraries(Core PRIVATE LIBRARY)
target_link_libraries(Core PRIVATE opengl32)

# After you have build the project for the first time 
# target_link_libraries(Core PRIVATE Debug/SDL3)
# target_link_libraries(Core PRIVATE Debug/sndfile)
# target_link_libraries(Core PRIVATE Debug/OpenAL32)

# For build the project for the first time.
target_link_libraries(Core PRIVATE ../../LIBRARY/Src/Lib/SDL3)
target_link_libraries(Core PRIVATE ../../LIBRARY/Src/Lib/sndfile)
target_link_libraries(Core PRIVATE ../../LIBRARY/Src/Lib/OpenAL32)

# Include directories
target_include_directories(Core PRIVATE include ../LIBRARY/Src/Include)


add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/LIBRARY/Src/Lib/SDL3.dll  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying DLL to the build directory"
)

add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/LIBRARY/Src/Lib/SDL3.lib  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying Lib to the build directory"
)

add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/LIBRARY/Src/Lib/OpenAL32.dll  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying DLL to the build directory"
)

add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/LIBRARY/Src/Lib/OpenAL32.lib  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying Lib to the build directory"
)

add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/LIBRARY/Src/Lib/sndfile.lib  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying Lib to the build directory"
)

add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/CORE/Src/Images  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying folder file to the build directory"
)

add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/CORE/Src/Shaders/depthBuffer.shader  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying shader file to the build directory"
)

add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/CORE/Src/Shaders/DepthTest.shader  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying shader file to the build directory"
)

add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/CORE/Src/Shaders/Shadow.shader  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying shader file to the build directory"
)

add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/CORE/Src/Shaders/Skybox.shader  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying shader file to the build directory"
)


add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/CORE/Src/Sounds/ping.wav  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying sound file to the build directory"
)

add_custom_command(
    TARGET Core  # Replace with your target name
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/CORE/Src/Models/monkey3.obj  # Source DLL
        ${PROJECT_BINARY_DIR}/Core/Debug  # Destination directory
    COMMENT "Copying obj file to the build directory"
)