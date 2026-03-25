#pragma once
#include <iostream>
#include <utility>
#include <cstdint>

// 64bit long long 用のMOD構造体
template <long long Mod>
struct ModInt64 {
    long long val;

    // --- モンゴメリ乗算用の内部定数とヘルパー関数 ---
    using uint64 = uint64_t;

    // N' = -Mod^-1 mod 2^64 (ニュートン法による逆元計算)
    static constexpr uint64 N_INV = []() {
        uint64 inv = (uint64)Mod;
        for (int i = 0; i < 5; ++i) inv *= 2 - (uint64)Mod * inv;
        return inv;
    }();

    // R^2 mod Mod (R = 2^64)
    static constexpr uint64 R2 = []() {
        uint64 r = (uint64(1) << 62) % (uint64)Mod;
        for (int i = 0; i < 66; i++) {
            r <<= 1;
            if (r >= (uint64)Mod) r -= (uint64)Mod;
        }
        return r;
    }();

    // 64bit整数の積の上位64bitを求める (128bit型不使用)
    static constexpr uint64 mul_high(uint64 x, uint64 y) noexcept {
        uint64 xh = x >> 32, xl = x & 0xFFFFFFFFULL;
        uint64 yh = y >> 32, yl = y & 0xFFFFFFFFULL;
        uint64 a = xh * yh, b = xl * yl, c = xh * yl, d = xl * yh;
        uint64 mid = (b >> 32) + (c & 0xFFFFFFFFULL) + (d & 0xFFFFFFFFULL);
        return a + (c >> 32) + (d >> 32) + (mid >> 32);
    }

    // モンゴメリ削減: x * y * R^-1 mod Mod
    static constexpr uint64 mul_mr(uint64 x, uint64 y) noexcept {
        uint64 low = x * y;
        uint64 hi = mul_high(x, y);
        // 公式: high(XY) + high(low(XY) * (-N_INV), Mod) + (low(XY) != 0)
        uint64 res = hi + mul_high(low * uint64(-N_INV), (uint64)Mod) + (low != 0);
        return res >= (uint64)Mod ? res - (uint64)Mod : res;
    }
    // ----------------------------------------------

    constexpr ModInt64(long long v = 0) noexcept : val(v % Mod) {
        if (val < 0) val += Mod;
    }

    constexpr long long get() const noexcept { return val; }

    constexpr ModInt64& operator+=(const ModInt64& rhs) noexcept {
        val += rhs.val;
        if (val >= Mod) val -= Mod;
        return *this;
    }

    constexpr ModInt64& operator-=(const ModInt64& rhs) noexcept {
        val -= rhs.val;
        if (val < 0) val += Mod;
        return *this;
    }

    constexpr ModInt64& operator*=(const ModInt64& rhs) noexcept {
        // 標準値を維持するため、2段階の削減で a * b mod Mod を計算
        uint64 res1 = mul_mr((uint64)val, (uint64)rhs.val); // val * rhs * R^-1
        val = (long long)mul_mr(res1, R2);                 // (val * rhs * R^-1) * R^2 * R^-1
        return *this;
    }

    constexpr ModInt64& operator/=(const ModInt64& rhs) noexcept {
        return *this *= rhs.inv();
    }

    constexpr ModInt64 operator+(const ModInt64& rhs) const noexcept { return ModInt64(*this) += rhs; }
    constexpr ModInt64 operator-(const ModInt64& rhs) const noexcept { return ModInt64(*this) -= rhs; }
    constexpr ModInt64 operator*(const ModInt64& rhs) const noexcept { return ModInt64(*this) *= rhs; }
    constexpr ModInt64 operator/(const ModInt64& rhs) const noexcept { return ModInt64(*this) /= rhs; }
    constexpr ModInt64 operator+() const noexcept { return ModInt64(*this); }
    constexpr ModInt64 operator-() const noexcept { return ModInt64(Mod - val); }
    constexpr bool operator==(const ModInt64& rhs) const noexcept { return val == rhs.val; }
    constexpr bool operator!=(const ModInt64& rhs) const noexcept { return val != rhs.val; }

    constexpr ModInt64 pow(long long n) const noexcept {
        ModInt64 res = 1, a = *this;
        while (n > 0) {
            if (n & 1) res *= a;
            a *= a;
            n >>= 1;
        }
        return res;
    }

    constexpr ModInt64 inv() const noexcept {
        long long a = val, b = Mod, u = 1, v = 0;
        while (b) {
            long long t = a / b;
            a -= t * b; std::swap(a, b);
            u -= t * v; std::swap(u, v);
        }
        return ModInt64(u);
    }

    friend std::ostream& operator<<(std::ostream& os, const ModInt64& m) {
        return os << m.val;
    }

    friend std::istream& operator>>(std::istream& is, ModInt64& m) {
        long long t;
        is >> t;
        m = ModInt64(t);
        return is;
    }
};

using ModInt61 = ModInt64<(1LL << 61) - 1>;