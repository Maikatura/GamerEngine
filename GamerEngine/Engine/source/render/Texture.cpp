#include "pch.h"
#include "Texture.h"
#include "ImageLoader.h"

GamerEngine::Texture::Texture()
{
}

GamerEngine::Texture::Texture(const std::string& aPath) :
	myRendererID(0), myFilePath(aPath), myLocalBuffer(nullptr), myWidth(0),
	myHeight(0), myBPP(0)
{
	

	stbi_set_flip_vertically_on_load(1);
	myLocalBuffer = stbi_load(aPath.c_str(), &myWidth, &myHeight, &myBPP, 4);

	glGenTextures(1, &myRendererID);
	glBindTexture(GL_TEXTURE_2D, myRendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, myWidth, myHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, myLocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	Bind();
}

GamerEngine::Texture::~Texture()
{
	glDeleteTextures(1, &myRendererID);
}


void GamerEngine::Texture::Bind(unsigned slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, myRendererID);
}

void GamerEngine::Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int GamerEngine::Texture::GetWidth() const
{
	return myWidth;
}

int GamerEngine::Texture::GetHeight() const
{
	return myHeight;
}

unsigned int GamerEngine::Texture::GetRenderID() const
{
	return myRendererID;
}
