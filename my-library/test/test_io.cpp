#include <bits/stdc++.h>
#include "../guy7cc/io.hpp"
#include "framework/test_framework.hpp"

using namespace std;
using namespace TestFramework;

struct StdoutRedirect {
    streambuf* orig;
    ostringstream oss;
    StdoutRedirect() {
        orig = cout.rdbuf(oss.rdbuf());
    }
    ~StdoutRedirect() { cout.rdbuf(orig); }
    string str() const { return oss.str(); }
};

void test_output_renamed() {
    print_title("Renamed Output Utilities (pr, prl)");
    {
        StdoutRedirect _;
        pr(10, 20, "abc");
        expect_eq(_.str(), string("10 20 abc"), "pr(10, 20, \"abc\")");
    }
    {
        StdoutRedirect _;
        prl(1, 2);
        expect_eq(_.str(), string("1 2\n"), "prl(1, 2)");
    }
    {
        StdoutRedirect _;
        pr(sep(","), 1, 2, 3);
        expect_eq(_.str(), string("1,2,3"), "pr(sep(\",\"), 1, 2, 3)");
    }
}

int main() {
    test_output_renamed();
    print_summary();
    return (passed_tests == total_tests) ? 0 : 1;
}
