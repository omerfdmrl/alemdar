#ifndef IPROCESS_H

#define IPROCESS_H

#include "ican.h"

Iray2D *one_hot_encoding(Iray2D *data) {
    size_t category_length = 0;
    float *categories = NULL;
    Iray2D *temp;

    for (size_t i = 0; i < data->rows; i++) {
        for (size_t j = 0; j < data->cols; j++) {
            bool exists = false;
            for (size_t k = 0; k < category_length; k++) {
                if (categories[k] == data->data[i][j]) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                categories = (float *)realloc(categories, (category_length + 1) * sizeof(float));
                categories[category_length] = data->data[i][j];
                category_length++;
            }
        }
    }

    temp = iray2d_alloc(data->rows, category_length);

    for (size_t i = 0; i < data->rows; i++) {
        for (size_t j = 0; j < data->cols; j++) {
            for (size_t k = 0; k < category_length; k++) {
                if (categories[k] == data->data[i][j]) {
                    temp->data[i][k] = 1;
                } else {
                    temp->data[i][k] = 0;
                }
            }
        }
    }
    return temp;
}

#endif // !IPROCESS_H