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

Iray2D *standard_scaler(Iray2D *data) {
    Iray2D *scaledData = iray2d_alloc(data->rows, data->cols);

    for (int j = 0; j < data->cols; j++) {
        double sum = 0.0;
        for (int i = 0; i < data->rows; i++) {
            sum += data->data[i][j];
        }
        double mean = sum / data->rows;

        double stddev_sum = 0.0;
        for (int i = 0; i < data->rows; i++) {
            stddev_sum += (data->data[i][j] - mean) * (data->data[i][j] - mean);
        }
        double stddev = sqrt(stddev_sum / data->rows);

        for (int i = 0; i < data->rows; i++) {
            if (stddev != 0) {
                scaledData->data[i][j] = (data->data[i][j] - mean) / stddev;
            } else {
                scaledData->data[i][j] = data->data[i][j];
            }
        }
    }
    return scaledData;
}

#endif // !IPROCESS_H