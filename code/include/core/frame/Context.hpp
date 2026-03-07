#pragma once

#include <core/tool/Log.hpp>
#include <core/tool/type.hpp>
#include <memory>
#include <typeindex>
#include <typeinfo>

namespace dao {
    /// @brief 上下文容器类
    /// @details 用于存储和检索任意类型的共享对象，通过类型信息作为键进行索引。
    ///          支持两种模式：
    ///          - 外部对象引用：通过 emplace 引用已存在的对象，不持有所有权
    ///          - 内部对象创建：在容器内创建新对象，容器持有所有权
    class Context {
    public:
        Context() = default;

        /// @brief 向容器中插入或创建对象
        /// @tparam T 对象类型
        /// @tparam Args 构造参数类型
        /// @param args 若为单个 T 类型参数，则引用该对象；否则用于构造新对象
        /// @returns 对象的引用
        /// @note 当传入单个 T 类型参数时，仅持有引用而不获取所有权；
        ///       当传入其他参数时，容器创建并拥有新对象
        template<typename T, typename... Args>
        T &emplace(Args &&...args) {
            if constexpr (sizeof...(args) == 1 &&
                          std::is_same_v<T, std::decay_t<std::tuple_element_t<0, std::tuple<Args...>>>>) {
                // 引用模式：传入单个 T 类型对象，不持有所有权
                auto &context = std::get<0>(std::forward_as_tuple(std::forward<Args>(args)...));
                m_context[typeid(T)] = std::shared_ptr<void>(std::addressof(context), [](void *) {});
                return context;
            } else {
                // 创建模式：构造新对象并持有所有权
                auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
                m_context[typeid(T)] = ptr;
                return *ptr;
            }
        }

        /// @brief 获取容器中的对象
        /// @tparam T 对象类型
        /// @returns 对象指针，若不存在则返回 nullptr
        template<typename T>
        T *get() {
            const auto it = m_context.find(typeid(T));
            if (it == m_context.end()) {
                DAO_ERROR_LOG("状态不存在");
                return nullptr;
            }
            return std::static_pointer_cast<T>(it->second).get();
        }

        /// @brief 检查容器中是否存在指定类型的对象
        /// @tparam T 对象类型
        /// @returns 存在返回 true，否则返回 false
        template<typename T>
        [[nodiscard]] bool has() const {
            return m_context.contains(typeid(T));
        }

    private:
        hash_map<std::type_index, std::shared_ptr<void>> m_context; ///< 类型索引到对象的映射表
    };
} // namespace dao
