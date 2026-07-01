#pragma once
#include <web/http/HttpTypes.hpp>
#include <chrono>
#include <memory>
#include <mutex>
#include <optional>

namespace dao::web {
    class HttpClient;

    /// @brief HTTP 定时轮询器
/// @details 封装定时和异步请求管理，在 Page::update() 中每帧调用 tick()，
/// 到达间隔时自动发起 GET 请求，通过 hasResponse() / takeResponse() 消费结果。
///
/// @note tick() / hasResponse() / takeResponse() 必须在同一线程调用。
/// 请求完成的回调在线程池执行，内部用 mutex 保护状态，保证与主线程安全交互。
///
/// 内部 State 结构体通过 shared_ptr 共享给异步回调，析构时将 alive 置 false，
/// 回调在持有锁的情况下检查 alive，避免访问已销毁的对象。
///
/// @code
///   class MyPage : public dao::ifc::IGeneralPage {
///       dao::web::HttpClient m_client{getDefaultGateway(), 50000};
///       dao::web::HttpPoller m_poller{m_client,
///           dao::web::HttpRequest{.path = "/syncHosts"},
///           std::chrono::seconds(5)};
///
///       void update() override {
///           m_poller.tick();
///           while (m_poller.hasResponse()) {
///               auto resp = m_poller.takeResponse();
///               // 处理 resp...
///           }
///       }
///   };
/// @endcode
    class HttpPoller {
    public:
        /// @brief 构造轮询器
    /// @param client HttpClient 引用，生命周期必须长于 HttpPoller
    /// @param req 请求模板
    /// @param interval 轮询间隔
        HttpPoller(HttpClient &client, HttpRequest req,
                   std::chrono::seconds interval);

        /// @brief 析构时标记停止，阻止悬空回调写入已销毁成员
        ~HttpPoller();

        /// @brief 每帧调用，到达间隔时自动发起异步 GET 请求
        void tick();

        /// @brief 检查是否有已完成的响应
        [[nodiscard]] bool hasResponse() const;

        /// @brief 取出响应（消费，取出后内部清除）
        [[nodiscard]] HttpResponse takeResponse();

        /// @brief 修改轮询间隔
        void setInterval(std::chrono::seconds interval);

        /// @brief 修改请求模板
        void setRequest(const HttpRequest &req);

    private:
        /// @brief 跨线程共享的可变状态，通过 shared_ptr 安全传递到异步回调中
        struct State {
            std::mutex mtx;                              ///< 保护所有成员
            bool alive = true;                           ///< 析构标记，防止悬空访问
            bool requestInFlight = false;                ///< 是否有未完成的请求
            std::optional<HttpResponse> pendingResponse; ///< 已完成的响应
        };

        std::shared_ptr<State> m_state = std::make_shared<State>();
        HttpClient *m_client;
        HttpRequest m_req;
        std::chrono::seconds m_interval;
        std::chrono::steady_clock::time_point m_lastRequest;
    };
}