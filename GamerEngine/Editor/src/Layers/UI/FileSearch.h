#pragma once
#include <string>
#include "Layers/Layer.h"

class FileSearch : public Layer
{
	inline static bool myIsOpen = false;
public:

	static bool Search(const std::string& aFileExtension);

	void OnImGuiRender() override;

};