#pragma once
#include <cmath>
#include <iostream>
#include <algorithm>

namespace CommonUtilities
{
    static const uint32_t PRIME_CONST = 31;
    static const uint32_t ARR_SIZE = 3001;

    uint32_t HashString(const std::string& aKey)
    {
        uint32_t sum = 0;
        for(int i = 0; i < aKey.length(); i++) 
        {
            sum += (aKey[i] * (uint32_t)pow(PRIME_CONST, i)) % ARR_SIZE;
        }
        return sum % ARR_SIZE;
    };
}