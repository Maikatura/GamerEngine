#pragma once
#include <memory>

template <typename T>
class UniquePtr
{
	std::unique_ptr<T> myPtr;

public:
	UniquePtr() = default;
	UniquePtr(T* aPtr) : myPtr(aPtr) {}
	UniquePtr(const UniquePtr<T>& aOther) = delete;
	UniquePtr(UniquePtr<T>&& aOther) noexcept = default;
	~UniquePtr() = default;

	UniquePtr<T>& operator=(const UniquePtr<T>& aOther) = delete;
	UniquePtr<T>& operator=(UniquePtr<T>&& aOther) noexcept = default;

	T* operator->() const { return myPtr.get(); }
	T& operator*() const { return *myPtr; }

	operator bool() const { return myPtr != nullptr; }

	bool operator==(const UniquePtr<T>& aOther) const { return myPtr == aOther.myPtr; }
	bool operator!=(const UniquePtr<T>& aOther) const { return myPtr != aOther.myPtr; }

	T* Get() const { return myPtr.get(); }
};
