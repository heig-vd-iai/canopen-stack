#pragma once

#include <cstdint>
#include <cassert>
#include <limits>
#include <type_traits>

namespace bits {

inline constexpr void setBit(uint8_t& byte, unsigned bit, bool value) {
    if (value)
        byte |= static_cast<uint8_t>(1u << bit);
    else
        byte &= static_cast<uint8_t>(~(1u << bit));
}

inline constexpr bool getBit(uint8_t byte, unsigned bit) {
    return (byte & (1u << bit)) != 0;
}

template <class T>
using enable_uint_t = std::enable_if_t<std::is_unsigned_v<T>, int>;

// Mask of "width" bits set to 1 (width can go up to the size of T)
template <class T, enable_uint_t<T> = 0>
constexpr T width_mask(unsigned width) noexcept {
    const unsigned W = std::numeric_limits<T>::digits;
    assert(width <= W);
    if (width == 0) return T{0};
    if (width >= W) return std::numeric_limits<T>::max();
    return (T{1} << width) - T{1};
}

// Mask positioned at the offset "start"
template <class T, enable_uint_t<T> = 0>
constexpr T field_mask(unsigned start, unsigned width) noexcept {
    const unsigned W = std::numeric_limits<T>::digits;
    assert(start <= W && width <= W && start + width <= W);
    return width_mask<T>(width) << start;
}

template <class T, enable_uint_t<T> = 0>
constexpr void set_field(T& target, unsigned start, unsigned width, T field_value) noexcept {
    const unsigned W = std::numeric_limits<T>::digits;
    assert(start <= W && width <= W && start + width <= W);
    if (width == 0) return; // rien à faire

    const T m      = width_mask<T>(width) << start;
    const T fvalue = (field_value & width_mask<T>(width)) << start;
    target = (target & ~m) | (fvalue & m);
}

template <class T, enable_uint_t<T> = 0>
constexpr T get_field(T value, unsigned start, unsigned width) noexcept {
    const unsigned W = std::numeric_limits<T>::digits;
    assert(start <= W && width <= W && start + width <= W);
    if (width == 0) return T{0};
    return (value >> start) & width_mask<T>(width);
}

// Specialized shortcuts for a byte
inline constexpr void set_field8(uint8_t& byte, unsigned start, unsigned width,
                                 uint8_t v) noexcept {
    set_field<uint8_t>(byte, start, width, v);
}
inline constexpr uint8_t get_field8(uint8_t byte, unsigned start,
                                    unsigned width) noexcept {
    return get_field<uint8_t>(byte, start, width);
}

}  // namespace bits
