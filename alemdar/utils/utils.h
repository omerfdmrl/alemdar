#ifndef ALEMDAR_UTILS_H

#define ALEMDAR_UTILS_H

ALEMDAR_DEF Matrix one_hot_encoding(Matrix data);

Matrix one_hot_encoding(Matrix data) {
    size_t category_length = 0;
    float *categories = NULL;
    Matrix temp;

    for (size_t i = 0; i < data.rows; i++) {
        for (size_t j = 0; j < data.cols; j++) {
            bool exists = false;
            for (size_t k = 0; k < category_length; k++) {
                if (categories[k] == data.data[i][j]) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                categories = (float *)ALEMDAR_REALLOC(categories, (category_length + 1) * sizeof(float));
                categories[category_length] = data.data[i][j];
                category_length++;
            }
        }
    }

    temp = matrix_alloc(data.rows, category_length);

    for (size_t i = 0; i < data.rows; i++) {
        for (size_t j = 0; j < data.cols; j++) {
            for (size_t k = 0; k < category_length; k++) {
                if (categories[k] == data.data[i][j]) {
                    temp.data[i][k] = 1;
                } else {
                    temp.data[i][k] = 0;
                }
            }
        }
    }
    return temp;
}

#endif // !ALEMDAR_UTILS_H