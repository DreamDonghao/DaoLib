//
// Created by donghao on 25-12-19.
//
#pragma once
#include <any>
#include <queue>
#include <string>

namespace dao {
    /// @brief 页面命令类型
    enum class PageCmdType {
        switchPage, ///< 切换页面
    };

    /// @brief 页面命令
    struct PageCmd {
        PageCmdType type; ///< 页面命令类型
        std::any data;    ///< 数据
    };

    class PageCmdQueue {
    public:
        PageCmdQueue() = default;

        PageCmdQueue(const PageCmdQueue &) = delete;

        void addSwitchPage(const std::string &title) {
            m_events.push(PageCmd{PageCmdType::switchPage, std::any{title}});
        }

        [[nodiscard]] bool isPresenceMessage() const {
            return !m_events.empty();
        }

        PageCmd popEvent() {
            auto event = m_events.front();
            m_events.pop();
            return event;
        }

    private:
        std::queue<PageCmd> m_events;
    };
}
