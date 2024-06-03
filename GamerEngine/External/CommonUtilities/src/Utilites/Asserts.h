#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#if defined(_DEBUG) || defined(_RELEASE)

// Function to convert uint64_t to string
template<typename T>
inline std::string to_string(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// Function to replace placeholders in the message string
template<typename... Args>
inline std::string replacePlaceholder(const std::string& message, Args&&... args) {
    std::string result;
    size_t pos = 0;
    size_t argIndex = 0;
    while ((pos = message.find("{}", pos)) != std::string::npos) {
        result.append(message, argIndex, pos - argIndex);
        ((result += to_string(std::forward<Args>(args))), ...);
        argIndex = pos + 2;
        pos += 2;
    }
    result += message.substr(argIndex);
    return result;
}

#define GE_LOG(message) \
    std::cout << "Log: (" << message << ") in " << __FILE__ << " at line " << __LINE__ << std::endl;


#define GE_WARN(message, ...) \
    { \
        std::stringstream ss; \
        ss << replacePlaceholder(message, ##__VA_ARGS__); \
        ss << " in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        std::cerr << ss.str(); \
    }

#define GE_LOG_ERROR(message, ...) \
    do { \
        std::stringstream ss; \
        ss << "ERROR: " << replacePlaceholder(message, ##__VA_ARGS__); \
        ss << " in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        std::cerr << ss.str(); \
    } while (false)

#define GE_ASSERT_NO_MSG(condition) \
    if (!(condition)) \
    { \
        std::cerr << "Assertion failed: " << #condition << " (" << "No message" << ") in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        std::abort(); \
    }

#define GE_ASSERT(condition, ...) \
    do { \
        if (!(condition)) \
        { \
            std::stringstream ss; \
            ss << "ERROR: " << replacePlaceholder("Condition Meet", ##__VA_ARGS__); \
            ss << " in " << __FILE__ << " at line " << __LINE__ << std::endl; \
            std::cerr << ss.str(); \
        } \
    } while (false)

#define GE_VALID(pointer) \
    GE_ASSERT((pointer) != nullptr, "Pointer is null")

#else
#define GE_LOG(message)
#define GE_LOG_ERROR(message)
#define GE_ASSERT_NO_MSG(condition)
#define GE_ASSERT(condition, message)
#define GE_VALID(pointer)
#endif