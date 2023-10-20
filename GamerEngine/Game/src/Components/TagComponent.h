#pragma once
#include <string>
#include "Component.h"

class TagComponent : public Component
{
public:
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag)
		: Tag(tag)
	{}
};
