#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define ALEMDAR_IMPLEMENTATION
#include "alemdar/alemdar.h"

float input[5][1] = {
   {0.0},
   {1.0},
   {1.0},
   {0.0},
   {1.0},

};
float output[5][1] = {
    {0.0},
    {1.0},
    {0.0},
    {1.0},
    {0.0},
};

int main() {
    srand(time(0));

    Matrix inputData = matrix_alloc(5, 1);
    for (size_t i = 0; i < inputData.rows; i++)
    {
        for (size_t j = 0; j < inputData.cols; j++)
        {
            inputData.data[i][j] = input[i][j];
        }
        
    }
    Matrix outputData = matrix_alloc(5, 1);
    for (size_t i = 0; i < outputData.rows; i++)
    {
        for (size_t j = 0; j < outputData.cols; j++)
        {
            outputData.data[i][j] = output[i][j];
        }
        
    }
    Layer d1 = layer_dense(1,8);
    Layer a2 = layer_activation(Sigmoid);
    Layer r1 = layer_gru(8, 8);
    Layer d2 = layer_dense(8, 1);
    Layer a1 = layer_activation(Sigmoid);
    Model m = model_alloc(5);
    model_add(&m, d1);
    model_add(&m, a2);
    model_add(&m, r1);
    model_add(&m, d2);
    model_add(&m, a1);


    model_randomize(RandomXavier,m);
    model_learn(BatchGradientDescent,m, 100000, inputData, outputData);

    model_print(m);

    for (size_t i = 0; i < 5; i++)
    {
        model_input(m, inputData.data[i]);
        model_forward(m);
        printf("\nİnput = %f, Output = %f, Actual Output = %f\n", inputData.data[i][0], MODEL_OUTPUT(m)[0], outputData.data[i][0]);
    }
    
    return 0;
}
