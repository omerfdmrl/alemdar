/**
 * make run SOURCE=./examples/mnist.c
 */

#include "../src/ican.h"
#include <time.h>

int main() {
    srand(time(0));
    
    // https://www.kaggle.com/datasets/oddrationale/mnist-in-csv
    Iray2D *data = csv_read("./data/mnist_train.csv", 60000, 785);
    Iray2D *outputs = iray2d_alloc(60000, 1);
    Iray2D *inputs = iray2d_alloc(60000, 784);
    
    for (int i = 0; i < data->rows; i++) {
        outputs->data[i][0] = data->data[i][0];
        for (int j = 1; j < data->cols; j++) {
            inputs->data[i][j] = data->data[i][j] / 255;
        }
    }
    iray2d_free(data);

    Iray2D *outputencoded = one_hot_encoding(outputs);

    iray2d_free(outputs);

    Layer *l1 = layer_dense(inputs->cols, inputs->cols / 2);
    Layer *a1 = layer_activation(Sigmoid);
    Layer *l2 = layer_dense(inputs->cols / 2, outputencoded->cols);
    Layer *a2 = layer_activation(Sigmoid);
    Model *m = model_alloc(5);
    model_add(m, l1);
    model_add(m, a1);
    model_add(m, l2);
    model_add(m, a2);
    model_randomize(RandomXavier, m, 0, 1);
    model_learn(BatchGradientDescent, m, 2, inputs, outputencoded);
    model_print(m);
    printf("Cost = %.10f\n", model_cost(m, inputs->data[0], outputencoded->data[0]));
    printf("\n");
    for (size_t i = 0; i < 10; i++)
    {
        model_input(m, inputs->data[i]);
        model_forward(m);
        printf("\nOutput = ");
        for (size_t j = 0; j < outputencoded->cols; j++)
        {
            if(outputencoded->data[i][j] == 1)
                printf("%zu ", j);
        }
        float *founded = MODEL_OUTPUT(m);
        printf("\nFounded = ");
        for (size_t j = 0; j < outputencoded->cols; j++)
        {
            if(founded[j] > 0.6)
                printf("%zu ^ %f", j, founded[j]);
        }
        
    }
    layer_free(l1);
    layer_free(a1);
    layer_free(a2);
    layer_free(l2);
    iray2d_free(inputs);
    iray2d_free(outputencoded);
    
    return 0;
}