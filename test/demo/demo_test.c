#include "test.h"

#include <stdio.h>
#include <stdlib.h>

unsigned int factorial(unsigned int n) {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

DEFINE_TEST(factorial_zero)
    ASSERT_EQ(factorial(0), 7)
END_TEST

DEFINE_TEST(factorial_one)
    ASSERT_EQ(factorial(1), 1)
END_TEST

DEFINE_TEST(factorial_five)
    ASSERT_EQ(factorial(5), 120)
END_TEST

REGISTER_TESTS(
    factorial_zero,
    factorial_one,
    factorial_five
)