#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <type_traits>

namespace guy7cc {

// ---- Input Utilities ----

template<typename T>
struct Reader {
    operator T() const {
        T x;
        if (!(std::cin >> x)) return T();
        return x;
    }
};

inline constexpr Reader<int>         ri;
inline constexpr Reader<long long>   rll;
inline constexpr Reader<std::string> rs;
inline constexpr Reader<char>        rc;
inline constexpr Reader<double>      rdb;

template<typename T>
T r() { T x; std::cin >> x; return x; }

template<typename... Ts>
std::tuple<Ts...> rt() {
    std::tuple<Ts...> tup;
    std::apply([](auto&... args) { (std::cin >> ... >> args); }, tup);
    return tup;
}

template<typename T>
std::vector<T> rv(int n) {
    std::vector<T> vec(n);
    for (auto& x : vec) std::cin >> x;
    return vec;
}

template<typename T>
std::vector<std::vector<T>> rvv(int h, int w) {
    std::vector<std::vector<T>> grid(h, std::vector<T>(w));
    for (auto& row : grid)
        for (auto& x : row) std::cin >> x;
    return grid;
}

// ---- Output Utilities ----

// Separator helper
struct sep {
    const char* s;
    explicit sep(const char* s_) : s(s_) {}
};

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>>
    : std::true_type {};
template<> struct is_iterable<std::string> : std::false_type {};

// Forward declarations
template<typename T> void pr(const T& x, const char* s = " ");
template<typename T, typename... Args> void pr_impl(const char* s, const T& x, const Args&... args);

// Single value or container print (with internal separator s)
template<typename T>
void pr(const T& x, const char* s) {
    if constexpr (is_iterable<T>::value) {
        bool first = true;
        for (const auto& i : x) {
            if (!first) std::cout << s;
            pr(i, s);
            first = false;
        }
    } else {
        std::cout << x;
    }
}

// Recursive implementation for variadic
template<typename T, typename... Args>
void pr_impl(const char* s, const T& x, const Args&... args) {
    pr(x, s);
    if constexpr (sizeof...(args) > 0) {
        std::cout << s;
        pr_impl(s, args...);
    }
}

// Case 1: pr(sep(","), a, b, c);
template<typename... Args>
void pr(sep s, const Args&... args) {
    if constexpr (sizeof...(args) > 0) {
        pr_impl(s.s, args...);
    }
}

// Case 2: pr(a, b, c); (default space)
template<typename T, typename... Args>
void pr(const T& x, const Args&... args) {
    if constexpr (std::is_same_v<T, sep>) {
        pr(x, args...);
    } else {
        pr_impl(" ", x, args...);
    }
}

// Case 3: pr();
inline void pr() {}

// Variadic println
template<typename... Args>
void prl(const Args&... args) {
    pr(args...);
    std::cout << "\n";
}

inline void Yes(bool b = true) { std::cout << (b ? "Yes\n" : "No\n"); }
inline void No(bool b = true)  { Yes(!b); }
inline void YES(bool b = true) { std::cout << (b ? "YES\n" : "NO\n"); }
inline void NO(bool b = true)  { YES(!b); }

// Aliases
template<typename... Args> void print(const Args&... args) { pr(args...); }
template<typename... Args> void println(const Args&... args) { prl(args...); }

} // namespace guy7cc
using namespace guy7cc;
