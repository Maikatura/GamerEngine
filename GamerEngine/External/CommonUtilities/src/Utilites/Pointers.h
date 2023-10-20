#pragma once
#include <memory>

template<typename T>
using SrdPtr = std::shared_ptr<T>;


template<typename T>
using UnqPtr = std::unique_ptr<T>;

template<typename T, typename... Args>
inline SrdPtr<T> MakeSrdPtr(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline UnqPtr<T> MakeUnqPtr(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}