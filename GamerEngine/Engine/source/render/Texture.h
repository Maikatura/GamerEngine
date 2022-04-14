#pragma once
#include <string>

namespace GamerEngine
{
	class Texture
	{
	public:
		Texture();
		Texture(const std::string& aPath);
		~Texture();

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

	};
}
