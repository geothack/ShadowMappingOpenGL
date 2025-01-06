add_library(LIBRARY
    Src/Source.c
Src/stb_image.c
Src/gl.c

)

target_include_directories(LIBRARY PUBLIC Src/Include/glm)
target_include_directories(LIBRARY PUBLIC Src/Include/stb)
target_include_directories(LIBRARY PUBLIC Src/Include/glad)

target_include_directories(LIBRARY PUBLIC Src/Include/SDL3)
target_include_directories(LIBRARY PUBLIC Src/Include/openal)
target_include_directories(LIBRARY PUBLIC Src/Include/snd)