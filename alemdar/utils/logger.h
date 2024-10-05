#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

// Disable debug and trace logging for release builds.
#if ALEMDAR_RELASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum LogLevels {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} LogLevels;

ALEMDAR_DEF bool initialize_logging();
ALEMDAR_DEF void shutdown_logging();

ALEMDAR_DEF void ALEMDAR_LOG(LogLevels level, const char* message, ...);

bool initialize_logging() {
    // TODO: create log file.
    return true;
}

void shutdown_logging() {
    // TODO: cleanup logging/write queued entries.
}

void ALEMDAR_LOG(LogLevels level, const char* message, ...) {
    if(level == LOG_LEVEL_WARN && LOG_WARN_ENABLED != 1) return;
    if(level == LOG_LEVEL_INFO && LOG_INFO_ENABLED != 1) return;
    if(level == LOG_LEVEL_DEBUG && LOG_DEBUG_ENABLED != 1) return;
    if(level == LOG_LEVEL_TRACE && LOG_TRACE_ENABLED != 1) return;
    const char* level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]:  ", "[INFO]:  ", "[DEBUG]: ", "[TRACE]: "};
    // b8 is_error = level < 2;

    // Technically imposes a 32k character limit on a single log entry, but...
    // DON'T DO THAT!
    char out_message[32000];
    memset(out_message, 0, sizeof(out_message));

    // Format original message.
    // NOTE: Oddly enough, MS's headers override the GCC/Clang va_list type with a "typedef char* va_list" in some
    // cases, and as a result throws a strange error here. The workaround for now is to just use __builtin_va_list,
    // which is the type GCC/Clang's va_start expects.
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, 32000, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[32001];
    sprintf(out_message2, "%s%s\n", level_strings[level], out_message);

    // TODO: platform-specific output.
    printf("%s", out_message2);
}
