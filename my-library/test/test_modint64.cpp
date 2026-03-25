#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "guy7cc/modint64"

#include "framework/test_framework.hpp"

using namespace std;
using namespace TestFramework;


// --- Target Utilities ---
long long mul_128(long long a, long long b, long long mod) {
    long long res = (long long)((__int128_t)a * b % mod);
    if (res < 0) res += mod;
    return res;
}

// --- Test Cases ---
void test_operations() {
    print_title("Basic Operations & Algorithms");

    const long long MOD = (1LL << 61) - 1;
    using mint = ModInt64<MOD>;

    mint a = 12345678901234567LL;
    mint b = 98765432109876543LL;

    expect_eq((a + b).val, (a.val + b.val) % MOD, "Addition (+)");

    long long exp_sub = (a.val - b.val) % MOD;
    if (exp_sub < 0) exp_sub += MOD;
    expect_eq((a - b).val, exp_sub, "Subtraction (-)");

    expect_eq((a * b).val, mul_128(a.val, b.val, MOD), "Montgomery Multiplication (*)");

    mint inv_b = b.inv();
    expect_eq((b * inv_b).val, 1LL, "Modular Inverse via Extended GCD");

    expect_eq((a * b / b).val, a.val, "Division Consistency (A * B / B == A)");

    long long exp_p = 1;
    for (int i = 0; i < 5; ++i) exp_p = mul_128(exp_p, a.val, MOD);
    expect_eq(a.pow(5).val, exp_p, "Exponentiation Loop (pow)");
}

void test_random() {
    print_title("Stress Tests");

    const long long MOD = (1LL << 61) - 1;
    using mint = ModInt64<MOD>;
    
    mt19937_64 rng(42);
    bool all_passed = true;
    
    for (int i = 0; i < 100000; ++i) {
        long long a = rng() % MOD;
        long long b = rng() % MOD;
        
        mint ma = a;
        mint mb = b;
        
        if ((ma * mb).val != mul_128(a, b, MOD)) {
            all_passed = false;
            break;
        }
    }
    
    expect_eq(all_passed, true, "100k Random Multiplication Verification");
}

void test_coprime_mod() {
    print_title("Odd Moduli Edge Cases");

    const long long MOD = 1000000007LL;
    using mint = ModInt64<MOD>;
    
    mint a = 123456789;
    mint b = 987654321;
    
    expect_eq((a * b).val, mul_128(a.val, b.val, MOD), "Modulo 10^9+7 Montgomery Stability");
}

int main() {
    test_operations();
    test_random();
    test_coprime_mod();
    
    print_summary();
    
    // テストが全て成功した時のみ正常終了
    return (passed_tests == total_tests) ? 0 : 1;
}
