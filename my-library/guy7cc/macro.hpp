#pragma once
#include <vector>
#include <algorithm>
#include <utility>

namespace guy7cc {

// Types
using ll = long long;
using pii = std::pair<int, int>;
using pll = std::pair<ll, ll>;
using vi = std::vector<int>;
using vll = std::vector<ll>;
using vvi = std::vector<vi>;
using vvll = std::vector<vll>;

// Loops
#define rep(i, n) for (int i = 0; i < (int)(n); ++i)
#define rep3(i, m, n) for (int i = (int)(m); (int)(m) < (int)(n) ? i < (int)(n) : i > (int)(n); (int)(m) < (int)(n) ? ++i : --i)
#define all(v) (v).begin(), (v).end()

// Utils
template<class T> bool chmax(T &a, const T &b) { if (a < b) { a = b; return 1; } return 0; }
template<class T> bool chmin(T &a, const T &b) { if (b < a) { a = b; return 1; } return 0; }

} // namespace guy7cc

