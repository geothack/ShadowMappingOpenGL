#shader vertex
#version 460 core


layout (location = 0) in vec3 aPos;


layout (std140) uniform Camera
{
    mat4 proj;
};

out vec3 TexCoords;


uniform mat4 view;


void main()
{
    TexCoords = aPos;
    vec4 pos = proj * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  









#shader fragment 
#version 460 core


out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}