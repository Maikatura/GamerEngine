#pragma once
#include <string>
#include <array>
#include "shader/shader_util.h"

namespace GamerEngine
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		void LoadTexture(const std::string& aPath);
		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		int GetWidth() const;
		int GetHeight() const;
		unsigned int GetRenderID() const;
		

	private:
		unsigned int myRendererID;
		std::string myFilePath;
		unsigned char* myLocalBuffer;
		int myWidth;
		int myHeight;
		int myBPP;

		std::array<float, 16> myPosition;
		std::array<unsigned int, 6> myIndicaes;

		std::unique_ptr<Shader> myShader;

	};

}
