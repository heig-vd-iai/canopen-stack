// phf_lookup.hpp — generated for trial 1
#pragma once
#include <cstdint>
#include <cassert>
#include <utility>


namespace phf {
constexpr uint16_t A0 = 21267u;
constexpr uint16_t A1 = 1u;
constexpr uint32_t B_BITS = 5u;
constexpr uint32_t M_BITS = 7u;
constexpr uint32_t KEY_BITS = 32u;
constexpr uint32_t B = (1u << B_BITS);
constexpr uint32_t M = (1u << M_BITS);

using key_t    = uint32_t;
using disp_t   = uint8_t;
using value_t  = uint8_t;

extern const disp_t  g_displace[B];
extern const key_t   g_keys[M];
extern const value_t g_values[M];

static inline key_t mix(key_t x) noexcept {
    return key_t(x + (x >> 3)); // wrap modulo largeur de key_t
}

static inline uint32_t h0(key_t x) noexcept {
    key_t prod = key_t(uint32_t(mix(x)) * uint32_t(A0)); // modulo 2^KEY_BITS
    return (uint32_t(prod) >> (KEY_BITS - B_BITS)) & (B - 1);
}

static inline uint32_t h1(key_t x) noexcept {
    key_t prod = key_t(uint32_t(mix(x)) * uint32_t(A1));
    return uint32_t(prod) & (M - 1);
}

static inline int16_t find(key_t key) noexcept {
    const uint32_t b = h0(key);
    const uint32_t i = (uint32_t)((h1(key) + g_displace[b]) & (M - 1));
#ifdef DEBUG
    assert(b < B);
    assert(i < M);
#endif
    return g_keys[i] == key ? g_values[i] : -1;
}

} // namespace phf
