#include "Texture.h"

Texture::~Texture()
{
}

void Texture::Load(const std::string& path, bool flip)
{
    glGenTextures(1, &m_texture_id);

    stbi_set_flip_vertically_on_load(flip);

    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
    if (data)
    {
        unsigned int format;
        if (m_channels == 1)
            format = GL_RED;
        else if (m_channels == 3)
            format = GL_RGB;
        else if (m_channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, m_texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}

void Texture::Use()
{
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
}
