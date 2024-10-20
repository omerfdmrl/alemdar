#ifndef ISERT_H

#define ISERT_H

#include "ican.h"

void report_assertion_failure(const char* expression, const char* message, const char* file, int line) {
    if(strlen(message) > 0) {
        ILOG(ILOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file:%s:%d\n", expression, message, file, line);
    }else {
        ILOG(ILOG_LEVEL_FATAL, "Assertion Failure: %s, in file:%s:%d\n", expression, file, line);
    }
}

#endif // !ISERT_H