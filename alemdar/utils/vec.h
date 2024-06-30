#ifndef ALEMDAR_VEC_H

#define ALEMDAR_VEC_H

typedef struct {
    size_t size;
    float *data;
} Vec;

ALEMDAR_DEF Vec vec_alloc(size_t size);
ALEMDAR_DEF void vec_realloc(Vec *vec, size_t size);
ALEMDAR_DEF Vec vec_add(Vec A, Vec B);
ALEMDAR_DEF void vec_trans(Vec A, Vec B);
ALEMDAR_DEF Vec vec_slice(Vec v, size_t start, size_t end);
ALEMDAR_DEF Vec vec_apply(Vec vec, float(*fn)(float x));
void vec_fill(Vec vec, float value);
Vec vec_copy(Vec vec);
ALEMDAR_DEF void vec_print(Vec vec);
ALEMDAR_DEF void vec_free(Vec vec);

Vec vec_alloc(size_t size) {
    Vec vec;
    vec.size = size;
    vec.data = ALEMDAR_MALLOC(sizeof(*vec.data) * size);
    return vec;
}
void vec_realloc(Vec *vec, size_t size) {
    vec->size = size;
    vec->data = ALEMDAR_REALLOC(vec->data, sizeof(*vec->data) * size);
}
Vec vec_add(Vec A, Vec B) {
    ALEMDAR_ASSERT(A.size == B.size);
    Vec output = vec_alloc(A.size);
    for (size_t i = 0; i < A.size; i++)
    {
        output.data[i] = A.data[i] + B.data[i];
    }
    return output;
}
void vec_trans(Vec A, Vec B) {
    ALEMDAR_ASSERT(A.size == B.size);
    for (size_t i = 0; i < A.size; i++)
    {
        A.data[i] = B.data[i];
    }
}
Vec vec_slice(Vec v, size_t start, size_t end) {
    size_t new_size = end - start;
    
    Vec result = vec_alloc(new_size);
    
    for (size_t i = start, j = 0; i < end; i++, j++) {
        result.data[j] = v.data[i];
    }
    
    return result;
}
Vec vec_apply(Vec vec, float(*fn)(float x)) {
    Vec output = vec_alloc(vec.size);
    for (size_t i = 0; i < vec.size; i++)
    {
        output.data[i] = fn(vec.data[i]);
    }
    return output;
}
void vec_fill(Vec vec, float value) {
    for (size_t i = 0; i < vec.size; i++)
    {
        vec.data[i] = value;
    }
}
Vec vec_copy(Vec vec) {
    Vec output = vec_alloc(vec.size);
    for (size_t i = 0; i < vec.size; i++)
    {
        output.data[i] = vec.data[i];
    }
    return output;
}
void vec_print(Vec vec) {
    printf("size = %zu\n", vec.size);
    for (size_t i = 0; i < vec.size; i++) {
        printf("%.1f ", vec.data[i]);
    }
}
void vec_free(Vec vec) {
    ALEMDAR_FREE(vec.data);
}

#endif // ALEMDAR_VEC_H