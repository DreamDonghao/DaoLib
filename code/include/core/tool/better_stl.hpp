#pragma once
#include <cstdint>
#include <algorithm>
#include <chrono>
#include <codecvt>
#include <iostream>
#include <memory>
#include <random>
#include <source_location>
#include <unordered_map>

namespace dao {
    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;
    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    using f32 = float;
    using f64 = double;

    using utf32char = char32_t;
    using utf32str = std::u32string;
    using utf32string_view = std::u32string_view;

    template<typename K, typename V>
    using hash_map = std::unordered_map<K, V>;

    template<typename Container>
    void sort(Container &container) {
        std::sort(container.begin(), container.end());
    }

    inline int randomInt(const int min, const int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    inline void detectionError(const bool pass, const std::string_view &message) {
        if (!pass) {
            std::cerr << message << std::endl;
        }
    }


    inline std::string currentDateTime() {
        using namespace std::chrono;

        const auto now = system_clock::now();

        // 转换到中国时区
        const zoned_time china_time{"Asia/Shanghai", now};

        const auto sec = floor<seconds>(china_time.get_local_time());
        const auto ms =
                duration_cast<milliseconds>(china_time.get_local_time() - sec).count();

        return std::format("{:%Y-%m-%d %H:%M:%S}.{:03}", sec, ms);
    }


    inline void DAO_ERROR_LOG(const std::string &msg,
                              const std::source_location &loc = std::source_location::current()) {
        std::cerr << "出错[ERROR]" << currentDateTime() << ":\n"
                << msg << "\n"
                << "文件[File]: " << loc.file_name() << "\n"
                << "行数[Line]: " << loc.line() << "\n"
                << "函数[Function]: " << loc.function_name() << "\n\n";
    }

    inline std::u32string to_u32(const std::string &s) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        return conv.from_bytes(s);
    }


    inline std::u32string utf8ToUtf32(const std::string &utf8Str) {
        std::u32string utf32Str;

        for (size_t i = 0; i < utf8Str.size();) {
            uint32_t codepoint = 0;

            if (const unsigned char c = utf8Str[i]; c < 0x80) {
                // 单字节 UTF-8
                codepoint = c;
                i += 1;
            } else if ((c & 0xE0) == 0xC0) {
                // 双字节 UTF-8
                codepoint = (c & 0x1F) << 6 | utf8Str[i + 1] & 0x3F;
                i += 2;
            } else if ((c & 0xF0) == 0xE0) {
                // 三字节 UTF-8
                codepoint = ((c & 0x0F) << 12) | ((utf8Str[i + 1] & 0x3F) << 6) | (utf8Str[i + 2] & 0x3F);
                i += 3;
            } else if ((c & 0xF8) == 0xF0) {
                // 四字节 UTF-8
                codepoint = ((c & 0x07) << 18) | ((utf8Str[i + 1] & 0x3F) << 12) |
                            ((utf8Str[i + 2] & 0x3F) << 6) | (utf8Str[i + 3] & 0x3F);
                i += 4;
            }

            utf32Str.push_back(codepoint);
        }

        return utf32Str;
    }

    /// @brief 结果为浮点数的除法
    template<typename T>
        requires std::is_arithmetic_v<T> // 约束为整数或浮点类型
    f32 ratio(T a, T b) {
        return static_cast<f32>(a) / static_cast<f32>(b);
    }

    /// @brief 可控删除器
    /// @details 可以为std::unique_ptr 提供一个删除器，使其可以有观察者与管理者两种状态
    template<typename T>
    class SwitchDeleter {
    public:
        enum Mode { Observe, Manage };

        explicit SwitchDeleter(const Mode m = Manage) : mode(m) {
        }

        void operator()(const T *ptr) const noexcept {
            if (mode == Manage) {
                delete ptr;
            }
        }

        void setMode(Mode m) { mode = m; }
        Mode getMode() const { return mode; }

    private:
        Mode mode;
    };

    /// @brief 观察者构建器
    template<typename T>
    auto makeObserver(T *ptr) {
        return std::unique_ptr<T, SwitchDeleter<T> >(
            ptr, SwitchDeleter<T>(SwitchDeleter<T>::Observe)
        );
    }

    /// @brief 管理者构建器
    template<typename T>
    auto makeManage(T *ptr) {
        return std::unique_ptr<T, SwitchDeleter<T> >(
            ptr, SwitchDeleter<T>(SwitchDeleter<T>::Manage)
        );
    }
}

