#pragma once
#include <memory>

// Self made pointer to data

template <class T>
class SharedRef
{
public:
	// constructors
	SharedRef() : myData(nullptr) {}
	SharedRef(T* data) : myData(data) { RefCount++; }
	SharedRef(T& data) : myData(&data) { RefCount++; }
	SharedRef(const SharedRef<T>& other) : myData(other.myData) { RefCount++; }

	// destructor
	~SharedRef()
	{
		RefCount--;
		if(RefCount <= 0)
		{
			delete myData;
		}
	}

	// assignment
	SharedRef<T>& operator=(const SharedRef<T>& other)
	{
		RefCount++;
		myData = other.myData;
		return *this;
	}

	// access
	T& operator*() const { return *myData; }
	T* operator->() const { return myData; }
	T* Get() const { return myData; }
	operator T* () const { return myData; }
	operator bool() const { return myData != nullptr; }

	bool operator==(const SharedRef<T>& aOther) const { return myData == aOther.myData; }
	bool operator!=(const SharedRef<T>& aOther) const { return myData != aOther.myData; }

private:
	// pointer to data
	T* myData;
	size_t RefCount = 0;
};

// pointer to data
template <class T>
class SharedRef<T[]>
{
public:
	// constructors
	SharedRef() : myData(nullptr) {}
	SharedRef(T* data) : myData(data) {}
	SharedRef(const SharedRef<T[]>& other) : myData(other.myData) {}

	// destructor
	~SharedRef()
	{
		if(RefCount <= 0)
		{
			delete[] myData;
		}
	}

	// assignment
	SharedRef<T[]>& operator=(const SharedRef<T[]>& other)
	{
		myData = other.myData;
		return *this;
	}

	// access
	T& operator[](int index) const { return myData[index]; }
	T* Get() const { return myData; }
	operator T* () const { return myData; }
	operator bool() const { return myData != nullptr; }

private:
	// pointer to data
	T* myData;
	size_t RefCount;
};

// pointer to data
template <class T>
class SharedRef<T*>
{
public:
	// constructors
	SharedRef() : myData(nullptr) {}
	SharedRef(T** data) : myData(data) { RefCount++; }
	SharedRef(const SharedRef<T*>& other) : myData(other.myData) { RefCount++; }

	// destructor
	~SharedRef()
	{
		RefCount--;
		if(RefCount <= 0)
		{
			delete myData;
		}
	}

	// assignment
	SharedRef<T*>& operator=(const SharedRef<T*>& other)
	{
		RefCount++;
		myData = other.myData;
		return *this;
	}

	// access
	T*& operator*() const { return *myData; }
	T*& operator->() const { return myData; }
	T*& Get() const { return myData; }
	operator T* () const { return myData; }
	operator bool() const { return myData != nullptr; }

private:
	// pointer to data
	T* myData;
	size_t RefCount = 0;
};

template <typename T>
SharedRef<T> MakeSharedRef()
{
	SharedRef<T> myRef = SharedRef<T>(new T());
	return myRef;
}