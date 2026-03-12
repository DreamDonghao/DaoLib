#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
namespace dao {
    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;
    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    using f32 = float;
    using f64 = double;

    using utf32char = char32_t;
    using utf32str = std::u32string;
    using utf32str_view = std::u32string_view;

    template<typename K, typename V>
    using hash_map = std::unordered_map<K, V>;
}
