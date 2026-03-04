#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <core/tool/better_stl.hpp>

namespace dao {
    /// @brief 全局环境上下文管理器

    class Context {
    public:
        /// @brief 默认构造函数
        Context() = default;

        /// @brief 注册或替换一个状态对象
        /// @tparam T 状态类型
        /// @tparam Args 构造函数参数类型
        /// @param args 传递给构造函数的参数
        /// @return 新创建的状态对象的引用
        template<typename T, typename... Args>
        T &emplaceState(Args &&... args) {
            auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
            m_states[typeid(T)] = ptr;
            return *ptr;
        }

        /// @brief 获取已注册的状态对象
        /// @tparam T 状态类型
        /// @return 状态对象的引用
        template<typename T>
        T &getState() {
            const auto it = m_states.find(typeid(T));
            if (it == m_states.end()) {
                DAO_ERROR_LOG("状态不存在");
            }
            return *std::static_pointer_cast<T>(it->second);
        }

        /// @brief 检查是否已注册指定类型的状态
        /// @tparam T 状态类型
        /// @return true 如果状态已注册，否则 false
        template<typename T>
        [[nodiscard]] bool hasState() const {
            return m_states.contains(typeid(T));
        }

        /// @brief 注册或替换一个服务对象
        /// @tparam T 服务类型
        /// @tparam Args 构造函数参数类型
        /// @param args 传递给构造函数的参数
        /// @return 新创建的服务对象的引用
        template<typename T, typename... Args>
        T &emplaceService(Args &&... args) {
            auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
            m_services[typeid(T)] = ptr;
            return *ptr;
        }

        /// @brief 获取已注册的服务对象
        /// @tparam T 服务类型
        /// @return 服务对象的引用
        template<typename T>
        T &service() {
            const auto it = m_services.find(typeid(T));
            if (it == m_services.end()) {
                DAO_ERROR_LOG("服务不存在");
            }
            return *std::static_pointer_cast<T>(it->second);
        }

        /// @brief 检查是否已注册指定类型的服务
        /// @tparam T 服务类型
        /// @return true 如果服务已注册，否则 false
        template<typename T>
        [[nodiscard]] bool hasService() const {
            return m_services.contains(typeid(T));
        }

    private:
        std::unordered_map<std::type_index, std::shared_ptr<void> > m_states; ///< 状态存储映射表
        std::unordered_map<std::type_index, std::shared_ptr<void> > m_services;///< 服务存储映射表
    };
}
