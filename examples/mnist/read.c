#define ALEMDAR_IMPLEMENTATION
#define ALEMDAR_CSV_IMPLEMENTATION
#define ALEMDAR_CONV_IMPLEMENTATION
#define ALEMDAR_IO_IMPLEMENTATION

#include "alemdar/alemdar.h"
#include <time.h>

int main() {
    srand(time(0));
    
    // https://www.kaggle.com/datasets/oddrationale/mnist-in-csv
    Matrix data = csv_read("./data/mnist_train.csv", 60000, 785);
    Matrix outputs = matrix_alloc(60000, 1);
    Matrix inputs = matrix_alloc(60000, 784);
    
    for (int i = 0; i < data.rows; i++) {
        outputs.data[i][0] = data.data[i][0];
        for (int j = 1; j < data.cols; j++) {
            inputs.data[i][j] = data.data[i][j] / 255;
        }
    }
    matrix_free(data);

    Matrix outputencoded = one_hot_encoding(outputs);

    matrix_free(outputs);

    Model m = import_model("mnist");

    model_print(m);
    printf("Cost = %.10f\n", model_cost(m, inputs.data[0], outputencoded.data[0]));
    printf("\n");
    for (size_t i = 0; i < 10; i++)
    {
        model_input(m, inputs.data[i]);
        model_forward(m);
        printf("\nOutput = ");
        for (size_t j = 0; j < outputencoded.cols; j++)
        {
            if(outputencoded.data[i][j] == 1)
                printf("%zu ", j);
        }
        float *founded = MODEL_OUTPUT(m);
        printf("\nFounded = ");
        for (size_t j = 0; j < outputencoded.cols; j++)
        {
            if(founded[j] > 0.6)
                printf("%zu ^ %f", j, founded[j]);
        }
        
    }
    return 0;
}
