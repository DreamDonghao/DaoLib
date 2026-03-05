#pragma once
#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <source_location>
#include <core/tool/type.hpp>
namespace dao {
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

    inline utf32str utf8ToUtf32(const std::string &s) {
        std::u32string out;
        out.reserve(s.size());

        size_t i = 0;
        const size_t size = s.size();

        while (i < size) {
            const unsigned char c = static_cast<unsigned char>(s[i]);
            u32 codepoint = 0;
            size_t len = 0;
            if (c < 0x80) {
                codepoint = c;
                len = 1;
            } else if ((c & 0xE0) == 0xC0) {
                if (i + 1 >= size) break;
                codepoint = (c & 0x1F) << 6 |
                            static_cast<unsigned char>(s[i + 1]) & 0x3F;
                len = 2;
            } else if ((c & 0xF0) == 0xE0) {
                if (i + 2 >= size) break;
                codepoint =
                        (c & 0x0F) << 12 |
                        (static_cast<unsigned char>(s[i + 1]) & 0x3F) << 6 |
                        static_cast<unsigned char>(s[i + 2]) & 0x3F;
                len = 3;
            } else if ((c & 0xF8) == 0xF0) {
                if (i + 3 >= size) break;
                codepoint = (c & 0x07) << 18 |
                            (static_cast<unsigned char>(s[i + 1]) & 0x3F) << 12 |
                            (static_cast<unsigned char>(s[i + 2]) & 0x3F) << 6 |
                            static_cast<unsigned char>(s[i + 3]) & 0x3F;
                len = 4;
            } else {
                codepoint = 0xFFFD;
                len = 1;
            }
            out.push_back(codepoint);
            i += len;
        }

        return out;
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
