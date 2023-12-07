#pragma once


#if _DEBUG

#define GE_LOG(message) \
    if (!(condition)) \
    { \
        std::cout << "Log: (" << message << ") in " << __FILE__ << " at line " << __LINE__ << std::endl; \
    }

#define GE_LOG_ERROR(message) \
        std::cerr << "Log Error: (" << message << ") in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        std::abort(); \

#define GE_ASSERT_NO_MSG(condition) \
    if (!(condition)) \
    { \
        std::cerr << "Assertion failed: " << #condition << " (" << "No message" << ") in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        std::abort(); \
    }

#define GE_ASSERT(condition, message) \
    if (!(condition)) \
    { \
        std::cerr << "Assertion failed: " << #condition << " (" << message << ") in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        std::abort(); \
    }

#define GE_VALID(pointer) \
    GE_ASSERT((pointer) != nullptr, "Pointer is null")

#else
#define GE_LOG(message)
#define GE_LOG_ERROR(message)
#define GE_ASSERT_NO_MSG(condition)
#define GE_ASSERT(condition, message)
#define GE_VALID(pointer)
#endif