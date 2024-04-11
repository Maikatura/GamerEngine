#pragma once
#include <iostream>
#include <cassert>

template <typename T>
class FastVector
{
    public:
    FastVector() : size_(0), capacity_(1), data_(new T[capacity_]) {}

    ~FastVector() { delete[] data_; }

    void push_back(const T& value)
    {
        if (size_ == capacity_)
        {
            resize(2 * capacity_);
        }
        data_[size_++] = value;
    }

    T& operator[](size_t index)
    {
        assert(index < size_);
        return data_[index];
    }

    const T& operator[](size_t index) const
    {
        assert(index < size_);
        return data_[index];
    }

    size_t size() const { return size_; }

    private:
    void resize(size_t new_capacity)
    {
        assert(new_capacity > size_);
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i)
        {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    size_t size_;
    size_t capacity_;
    T* data_;
};