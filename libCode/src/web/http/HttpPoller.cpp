#include <web/http/HttpPoller.hpp>
#include <web/http/HttpClient.hpp>

namespace dao::web {
    HttpPoller::HttpPoller(HttpClient &client, HttpRequest req,
                           std::chrono::seconds interval)
        : m_client(&client), m_req(std::move(req)), m_interval(interval),
          m_lastRequest(std::chrono::steady_clock::now()) {
    }

    HttpPoller::~HttpPoller() {
        std::lock_guard lock(m_state->mtx);
        m_state->alive = false;
    }

    void HttpPoller::tick() {
        bool shouldRequest = false;
        {
            std::lock_guard lock(m_state->mtx);
            if (!m_state->alive) return;
            const auto now = std::chrono::steady_clock::now();
            if (now - m_lastRequest >= m_interval && !m_state->requestInFlight) {
                m_state->requestInFlight = true;
                m_lastRequest = now;
                shouldRequest = true;
            }
        }
        if (shouldRequest) {
            m_client->get(m_req.path, [state = m_state](HttpResponse resp) {
                std::lock_guard lock(state->mtx);
                if (!state->alive) return;
                state->pendingResponse = std::move(resp);
                state->requestInFlight = false;
            });
        }
    }

    bool HttpPoller::hasResponse() const {
        std::lock_guard lock(m_state->mtx);
        return m_state->pendingResponse.has_value();
    }

    HttpResponse HttpPoller::takeResponse() {
        std::lock_guard lock(m_state->mtx);
        auto resp = std::move(*m_state->pendingResponse);
        m_state->pendingResponse.reset();
        return resp;
    }

    void HttpPoller::setInterval(std::chrono::seconds interval) {
        std::lock_guard lock(m_state->mtx);
        m_interval = interval;
    }

    void HttpPoller::setRequest(const HttpRequest &req) {
        std::lock_guard lock(m_state->mtx);
        m_req = req;
    }
}