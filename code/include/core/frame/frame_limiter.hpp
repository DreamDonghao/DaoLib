#pragma once
#include <chrono>
#include <thread>
#include "core/tool/better_stl.hpp"

namespace dao {
    /// @brief 帧率限制器
    /// @details 用于精确控制应用程序的帧率，确保帧率稳定在目标值
    /// 采用混合等待策略：先睡眠等待大部分时间，然后自旋等待剩余时间
    class FrameLimiter {
    public:
        /// @brief 构造函数
        /// @param target_fps 目标帧率（每秒帧数）
        explicit FrameLimiter(const i32 target_fps)
            : m_frame_duration(std::chrono::nanoseconds(1'000'000'000 / target_fps)), // 计算每帧持续时间（纳秒）
              m_last_frame_time(std::chrono::steady_clock::now()),                    // 记录上一帧的时间点
              m_spin_threshold(std::chrono::milliseconds(2)) {
            // 自旋等待阈值（2毫秒）
        }

        /// @brief 等待下一帧
        /// @details 混合等待策略：
        /// 1. 计算已用时间
        /// 2. 如果剩余时间大于自旋阈值，先睡眠等待大部分时间
        /// 3. 自旋等待剩余时间，确保精确到达下一帧时间点
        void wait() {
            const auto now = std::chrono::steady_clock::now(); // 获取当前时间
            const auto elapsed = now - m_last_frame_time;      // 计算自上一帧以来的时间

            // 如果剩余时间大于自旋阈值，先进行睡眠等待
            if (const auto sleep_duration = m_frame_duration - elapsed; sleep_duration > m_spin_threshold) {
                const auto sleep_time = sleep_duration - m_spin_threshold; // 睡眠时间 = 剩余时间 - 自旋阈值
                std::this_thread::sleep_for(sleep_time);                   // 睡眠等待
            }

            // 自旋等待直到达到目标帧时间
            while (std::chrono::steady_clock::now() - m_last_frame_time < m_frame_duration) {
                std::this_thread::yield(); // 让出CPU时间片
            }

            m_last_frame_time = std::chrono::steady_clock::now(); // 更新上一帧时间
        }

        /// @brief 动态设置目标帧率
        /// @param target_fps 新的目标帧率
        void setFPS(const i32 target_fps) {
            m_frame_duration = std::chrono::nanoseconds(1'000'000'000 / target_fps); // 重新计算每帧持续时间
        }

    private:
        std::chrono::nanoseconds m_frame_duration;               ///< 每帧的目标持续时间（纳秒）
        std::chrono::steady_clock::time_point m_last_frame_time; ///< 上一帧的时间点
        std::chrono::nanoseconds m_spin_threshold;               ///< 自旋等待阈值（2毫秒）
    };
}
