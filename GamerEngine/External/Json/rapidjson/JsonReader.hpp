#pragma once
#pragma warning( push )
#pragma warning( disable : 4996 )
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

namespace JsonReader
{

	static rapidjson::Document ReadJson(const char* aPath)
	{
		
		FILE* file = fopen(aPath, "rb");
		char readBuffer[600]{};
		rapidjson::FileReadStream frStream(file, readBuffer, sizeof(readBuffer));
		rapidjson::Document doc;
		doc.ParseStream(frStream);
		fclose(file);
		return doc;
	}

	

#pragma warning(pop)
}
