#pragma once
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;


template<typename T>
using UnqRef = std::unique_ptr<T>;

template<typename T, typename... Args>
inline Ref<T> MakeRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline UnqRef<T> MakeUnqRef(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}