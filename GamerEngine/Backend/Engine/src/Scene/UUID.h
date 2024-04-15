#pragma once
#include <cstdint>


namespace GamerEngine
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return myUUID; }
		uint64_t Get() { return myUUID; }
	private:
		uint64_t myUUID;
	};
}
