#pragma once
#include <chrono>
#include <iostream>
#include <source_location>

namespace dao {
    inline std::string currentDateTime() {
        using namespace std::chrono;

        const auto now = system_clock::now();

        const zoned_time china_time{"Asia/Shanghai", now};

        const auto sec = floor<seconds>(china_time.get_local_time());
        const auto ms = duration_cast<milliseconds>(china_time.get_local_time() - sec).count();

        return std::format("{:%Y-%m-%d %H:%M:%S}.{:03}", sec, ms);
    }

    inline void DAO_ERROR_LOG(const std::string &msg,
                              const std::source_location &loc = std::source_location::current()) {
        std::cerr << "[ERROR]" << currentDateTime() << ":\n"
                  << msg << "\n"
                  << "文件[File]: " << loc.file_name() << "\n"
                  << "行数[Line]: " << loc.line() << "\n"
                  << "函数[Function]: " << loc.function_name() << "\n\n";
    }
}
