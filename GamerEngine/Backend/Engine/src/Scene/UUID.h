#pragma once
#include <cstdint>


class UUID2
{
public:
	UUID2();
	UUID2(uint64_t uuid);
	UUID2(const UUID2&) = default;

	operator uint64_t() const { return myUUID; }
	uint64_t Get() { return myUUID; }
private:
	uint64_t myUUID;
};
