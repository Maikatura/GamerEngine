#pragma once
#include <iostream>

struct AllocationGoblin
{
	uint32_t myAllocated;
	uint32_t myFreed;

	uint32_t GetCurrent() { return (myAllocated - myFreed); };
};

static AllocationGoblin myAllocation;


inline void* operator new(size_t size)
{
	myAllocation.myAllocated += size;
	return malloc(size);
}

inline void operator delete(void* memory, size_t size)
{
	myAllocation.myFreed += size;
	free(memory);
}