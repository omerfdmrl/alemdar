#ifndef ALEMDAR_CSV_H

#define ALEMDAR_CSV_H

#ifdef ALEMDAR_CSV_IMPLEMENTATION

#ifndef MAX_LINE_BYTE
#define MAX_LINE_BYTE 2080
#endif // !MAX_LINE_BYTE

#include <string.h>

ALEMDAR_DEF Matrix csv_read(const char *filename, size_t rows, size_t cols);

Matrix csv_read(const char *filename, size_t rows, size_t cols) {
    FILE *file = fopen(filename, "r");
    ALEMDAR_ASSERT(file != NULL);

    Matrix data = matrix_alloc(rows, cols);

    char line[MAX_LINE_BYTE];
    size_t row = 0;
    while (fgets(line, sizeof(line), file) && row < rows) {
        char *token = strtok(line, ",");
        size_t col = 0;
        while (token != NULL && col < cols) {
            data.data[row][col] = atof(token);
            token = strtok(NULL, ",");
            col++;
        }
        row += 1;
    }

    fclose(file);
    return data;
}

#endif // ALEMDAR_CSV_IMPLEMENTATION

#endif // !ALEMDAR_CSV_H