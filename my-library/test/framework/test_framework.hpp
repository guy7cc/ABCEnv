#pragma once
#include <iostream>
#include <string>

namespace TestFramework {
    inline int passed_tests = 0;
    inline int total_tests = 0;

    inline void print_title(const std::string& title) {
        std::cout << "\n\033[1;35m=== " << title << " ===\033[0m\n";
    }

    template <typename T, typename U>
    inline void expect_eq(const T& actual, const U& expected, const std::string& test_name) {
        total_tests++;
        std::cout << "\033[36m[ TEST ]\033[0m " << test_name << " ";
        
        // 見た目を揃えるためのパディング計算
        int padding = 50 - test_name.length();
        if (padding > 0) {
            for (int i = 0; i < padding; ++i) std::cout << ".";
        }
        std::cout << " ";

        if (actual == expected) {
            std::cout << "\033[1;32mPASSED\033[0m\n";
            passed_tests++;
        } else {
            std::cout << "\033[1;31mFAILED\033[0m\n";
            std::cout << "         \033[33m| Expected : " << expected << "\033[0m\n";
            std::cout << "         \033[33m| Actual   : " << actual << "\033[0m\n";
        }
    }

    inline void print_summary() {
        std::cout << "\n\033[1;36m===============================================================\033[0m\n";
        if (passed_tests == total_tests && total_tests > 0) {
            std::cout << "  \033[1;32m[ SUCCESS ] All " << total_tests << " tests passed brilliantly!\033[0m\n";
        } else {
            std::cout << "  \033[1;31m[ FAILURE ] " << (total_tests - passed_tests) << " / " << total_tests << " tests failed.\033[0m\n";
        }
        std::cout << "\033[1;36m===============================================================\033[0m\n\n";
    }
}
