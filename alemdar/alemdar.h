#ifndef ALEMDAR_H
#define ALEMDAR_H

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifndef ALEMDAR_MALLOC
#define ALEMDAR_MALLOC malloc
#endif // !ALEMDAR_MALLOC

#ifndef ALEMDAR_REALLOC
#define ALEMDAR_REALLOC realloc
#endif // !ALEMDAR_REALLOC

#ifndef ALEMDAR_FREE
#define ALEMDAR_FREE free
#endif // !ALEMDAR_FREE

#ifndef ALEMDAR_ASSERT
#include <assert.h>
#define ALEMDAR_ASSERT assert
#endif // !ALEMDAR_ASSERT

#ifndef ALEMDAR_DEF
#define ALEMDAR_DEF static inline
#endif // !ALEMDAR_DEF

#ifdef ALEMDAR_IMPLEMENTATION

#define PI 3.14159265358979323846
typedef enum {
    false,
    true
} bool;

#include "./utils/helper.h"

#include "./utils/random.h"

#include "./utils/activation.h"

#include "./utils/vec.h"

#include "./utils/matrix.h"

#include "./utils/utils.h"

#include "./utils/csv.h"

#include "./utils/img.h"

#include "./utils/conv.h"

#include "./factory/layer.h"

#include "./factory/model.h"

#include "./utils/io.h"

#endif // ALEMDAR_IMPLEMENTATION

#endif // !ALEMDAR_H
