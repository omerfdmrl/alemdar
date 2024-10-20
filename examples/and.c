/**
 * make run SOURCE=./examples/and.c
 */

#include "../src/ican.h"

float input[4][2] = {
   {0.0, 0.0},
   {1.0, 0.0},
   {0.0, 1.0},
   {1.0, 1.0},

};
float output[4][1] = {
    {0.0},
    {0.0},
    {0.0},
    {1.0}
};

int main() {
    srand(1);
    Iray2D *inputData = iray2d_alloc(4, 2);
    for (size_t i = 0; i < inputData->rows; i++)
    {
        for (size_t j = 0; j < inputData->cols; j++)
        {
            inputData->data[i][j] = input[i][j];
        }
        
    }
    Iray2D *outputData = iray2d_alloc(4, 1);
    for (size_t i = 0; i < outputData->rows; i++)
    {
        for (size_t j = 0; j < outputData->cols; j++)
        {
            outputData->data[i][j] = output[i][j];
        }
        
    }
    Layer *l1 = layer_dense(2, 1);
    Layer *a1 = layer_activation(Sigmoid);
    Model *m = model_alloc(2);
    model_add(m, l1);
    model_add(m, a1);
    layer_print(a1);
    model_randomize(RandomXavier, m, 0, 1);
    model_learn(BatchGradientDescent, m, 50000, inputData, outputData);
    model_print(m);
    printf("Cost = %.10f\n", model_cost(m, input[0], output[0]));
    printf("\n");
    for (size_t i = 0; i < inputData->rows; i++)
    {
        model_input(m, inputData->data[i]);
        model_forward(m);
        for (size_t j = 0; j < inputData->cols; j++)
        {
            printf("%f ^ ", inputData->data[i][j]);
        }
        float *out = MODEL_OUTPUT(m);
        printf(" = %f\n", out[0]);
        
    }
    layer_free(l1);
    layer_free(a1);
    iray2d_free(inputData);
    iray2d_free(outputData);
    return 0;
}