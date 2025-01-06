#pragma once
#include <stb_image.h>
#include <glad/gl.h>

#include <iostream>
#include <string>


class Texture
{
public:
	Texture() = default;
	~Texture();

	void Load(const std::string& path, bool flip = false);
	void Use();


	inline unsigned int GetId() { return m_texture_id; }

private:
	unsigned int m_texture_id;

	int m_width;
	int m_height;
	int m_channels;


};

