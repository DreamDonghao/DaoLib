#pragma once

#include <chrono>

namespace dao {
    /// @brief 节拍器
    class TickTimer {
    public:
        using clock = std::chrono::steady_clock;

        /// @param interval 触发间隔（如 16ms ≈ 60fps）
        explicit TickTimer(const std::chrono::milliseconds interval)
            : m_interval(interval),
              m_last(clock::now()) {
        }

        /// @brief 判断当前是否到达触发时间
        ///
        /// 行为：
        /// - 到达间隔 → 返回 true，并推进内部时间
        /// - 未到 → 返回 false
        ///
        /// 特别注意：
        /// 如果主线程卡顿较久（例如 40ms，而 interval=16ms），
        /// 本函数会只推进一个 interval，下一次调用仍可能返回 true，
        /// 从而保证时间轴连续，不丢逻辑帧。
        explicit operator bool() {
            if (const auto now = clock::now(); now - m_last >= m_interval) {
                // 防止时间漂移：按固定步长推进，而不是直接跳到 now
                m_last += m_interval;
                return true;
            }
            return false;
        }

        /// @brief 重置计时器，从当前时间重新开始
        void reset() {
            m_last = clock::now();
        }

        /// @brief 修改触发间隔
        void setInterval(std::chrono::milliseconds interval) {
            m_interval = interval;
        }

        /// @brief 获取当前间隔
        [[nodiscard]]
        std::chrono::milliseconds interval() const {
            return m_interval;
        }

    private:
        std::chrono::milliseconds m_interval; ///< 触发间隔
        clock::time_point m_last;             ///< 上次触发时间点
    };
}
