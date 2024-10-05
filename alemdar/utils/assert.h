// Disable assertions by commenting out the below line.

ALEMDAR_DEF void report_assertion_failure(const char* expression, const char* message, const char* file, int line);

#define ALEMDAR_ASSERT(expr)                                         \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            __builtin_trap();                                        \
        }                                                            \
    }

#define ALEMDAR_ASSERT_MSG(expr, message)                                 \
    {                                                                     \
        if (expr) {                                                       \
        } else {                                                          \
            report_assertion_failure(#expr, message, __FILE__, __LINE__); \
            __builtin_trap();                                             \
        }                                                                 \
    }

void report_assertion_failure(const char* expression, const char* message, const char* file, int line) {
    ALEMDAR_LOG(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}