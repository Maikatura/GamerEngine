#pragma once

#include <functional>

#include "Core/Model/Entity.h"

namespace GamerEngine
{

	enum class ContactType : int8_t { None = -1, CollisionBegin, CollisionEnd, TriggerBegin, TriggerEnd };
	using ContactCallbackFn = std::function<void(ContactType, Entity, Entity)>;

}
