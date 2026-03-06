//
// Created by donghao on 2026/2/12.
//
#pragma once
#include <vector>

namespace dao {
    /// @brief 循环向量
    /// @details 类似循环队列的容器，支持在多个元素之间循环迭代
    template<typename Type>
    class RingVector {
    public:
        /// @brief 默认构造函数
        RingVector() {
        };

        /// @brief 添加元素到末尾
        /// @param value 要添加的元素（常量引用）
        void push_back(const Type &value) {
            m_data.push_back(value);
        }

        /// @brief 添加元素到末尾（移动语义）
        /// @param value 要添加的元素（右值引用）
        void push_back(Type &&value) {
            m_data.push_back(std::move(value));
        }

        /// @brief 获取当前索引位置的元素
        /// @return 当前元素的引用
        Type& val() {
            return m_data[m_index];
        }

        /// @brief 获取当前索引位置的元素（const 版本）
        /// @return 当前元素的常量引用
        const Type& val() const {
            return m_data[m_index];
        }

        /// @brief 移动到下一个元素
        /// @details 索引值模运算，实现循环效果
        void next() {
            m_index = (m_index + 1) % m_data.size();
        }

    private:
        std::vector<Type> m_data; ///< 数据存储
        size_t m_index{0};        ///< 当前索引
    };
}