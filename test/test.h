#include <stdio.h>
#include <stdlib.h>

enum test_result {
    FAIL = 0,
    PASS
};

typedef enum test_result test_result_t;

#define ASSERT_TRUE(expr) if (!expr) { \
    fprintf(stderr, "Test failed: ASSERT_TRUE failed on expression %s," \
        "file: %s, line: %d. \n", #expr, __FILE__, __LINE__); \
    return FAIL; \
}

#define ASSERT_FALSE(expr) if (expr) { \
    fprintf(stderr, "Test failed: ASSERT_FALSE failed on expression %s," \
        "file: %s, line: %d. \n", #expr, __FILE__, __LINE__); \
    return FAIL; \
}

#define ASSERT_EQ(expected, val) if (expected != val) { \
    fprintf(stderr, "Test failed: ASSERT_EQ failed on expressions %s and %s," \
        "file %s, line %d. \n", #expected, #val, __FILE__, __LINE__); \
    return FAIL; \
}

#define DEFINE_TEST(name) test_result_t name() { \
    printf("Running test %s...", #name);

#define END_TEST printf("Test passed!\n"); \
    return PASS; \
}

#define REGISTER_TESTS(args...) int main() { \
    printf("Running tests: %s...\n", __FILE__); \
    test_result_t (*test_funcs[])(void) = {args, NULL}; \
    int index = 0; \
    int passed = 0; \
    int total = 0; \
    while (test_funcs[index] != NULL) { \
        passed += test_funcs[index](); \
        total += 1; \
        index += 1; \
    } \
    printf("Passed %d of %d tests.\n", passed, total); \
    return (passed == total) ? 0 : -1; \
}
