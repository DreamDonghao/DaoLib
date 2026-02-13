//
// Created by donghao on 2026/2/12.
//
#pragma once
#include <vector>

namespace dao {
    template<typename Type>
    class RingVector {
    public:
        RingVector() {
        };

        void push_back(const Type &value) {
            m_data.push_back(value);
        }

        void push_back(Type &&value) {
            m_data.push_back(value);
        }

        Type& val() {
            return m_data[m_index];
        }

        void next() {
            m_index = (m_index + 1) % m_data.size();
        }

    private:
        std::vector<Type> m_data;
        size_t m_index{0};
    };
}
