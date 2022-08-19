#pragma once

#include <sstream>
#include <random>
#include <string>
#if defined(__ANDROID__)
    #include <android/log.h>
#endif
namespace eg {
#define ENUM_FLAG_OPERATOR(T,X) inline T operator X (T lhs, T rhs) { return (T) (static_cast<std::underlying_type_t <T>>(lhs) X static_cast<std::underlying_type_t <T>>(rhs)); } 
#define ENUM_FLAGS(T) \
enum class T; \
inline T operator ~ (T t) { return (T) (~static_cast<std::underlying_type_t <T>>(t)); } \
ENUM_FLAG_OPERATOR(T,|) \
ENUM_FLAG_OPERATOR(T,^) \
ENUM_FLAG_OPERATOR(T,&) \
enum class T

#define EAGLE_MAKE_VERSION(major, minor, patch) \
    ((((uint32_t)(major)) << 22) | (((uint32_t)(minor)) << 12) | ((uint32_t)(patch)))

#define MAKE_FLOAT_VERSION(ver) \
    EAGLE_MAKE_VERSION(floor(abs(ver)), floor((abs(ver) - floor(abs(ver))) * 10), round((abs(ver)*10 - floor(abs(ver) * 10)) * 10))
    namespace utils {
        unsigned int random_char();
        template <typename T>
        inline T Abs(T x);
        inline bool IsEqualF(float lhs, float rhs, float precision = 0.000001f);

        inline bool IsNotEqualF(float lhs, float rhs, float precision = 0.000001f);
        std::string generateUUID(const int len = 8);
    }
#if defined(USE_PLATFORM_ANDROID_KHR)
    #define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "EagleEngine", __VA_ARGS__))
    #define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "EagleEngine", __VA_ARGS__))
    #define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "EagleEngine", __VA_ARGS__))
    #define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "EagleEngine", __VA_ARGS__))
#endif
}
