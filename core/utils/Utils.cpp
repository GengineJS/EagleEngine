#include <utils/Utils.h>
#include <sstream>
#include <random>
#include <string>
namespace eg {
    namespace utils {
        unsigned int random_char() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 255);
            return dis(gen);
        }
        template <typename T>
        T Abs(T x) {
            return x > 0 ? x : -x;
        }
        bool IsEqualF(float lhs, float rhs, float precision) {
            return (Abs<float>(lhs - rhs) < precision);
        }

        bool IsNotEqualF(float lhs, float rhs, float precision) {
            return (Abs<float>(lhs - rhs) > precision);
        }
        std::string generateUUID(const int len) {
            std::stringstream ss;
            for (auto i = 0; i < len; i++) {
                const auto rc = random_char();
                std::stringstream hexstream;
                hexstream << std::hex << rc;
                auto hex = hexstream.str();
                ss << (hex.length() < 2 ? '0' + hex : hex);
            }
            return ss.str();
        }

    }
}