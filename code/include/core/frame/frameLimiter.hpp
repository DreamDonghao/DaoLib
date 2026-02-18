#pragma once
#include <chrono>
#include <thread>

#include "core/tool/better_stl.hpp"

namespace dao {
    class FrameLimiter {
    public:
        explicit FrameLimiter(const uint32 target_fps)
            : m_frame_duration(std::chrono::nanoseconds(1'000'000'000 / target_fps)),
              m_last_frame_time(std::chrono::steady_clock::now()),
              m_spin_threshold(std::chrono::milliseconds(2)) {
        }

        void wait() {
            const auto now = std::chrono::steady_clock::now();
            const auto elapsed = now - m_last_frame_time;

            if (const auto sleep_duration = m_frame_duration - elapsed; sleep_duration > m_spin_threshold) {
                const auto sleep_time = sleep_duration - m_spin_threshold;
                std::this_thread::sleep_for(sleep_time);
            }

            while (std::chrono::steady_clock::now() - m_last_frame_time < m_frame_duration) {
                std::this_thread::yield();
            }

            m_last_frame_time = std::chrono::steady_clock::now();
        }

        void setFPS(int target_fps) {
            m_frame_duration = std::chrono::nanoseconds(1'000'000'000 / target_fps);
        }

    private:
        std::chrono::nanoseconds m_frame_duration;
        std::chrono::steady_clock::time_point m_last_frame_time;
        std::chrono::nanoseconds m_spin_threshold;
    };
}
