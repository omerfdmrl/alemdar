#ifndef ALEMDAR_LAYER_MEAN_POOLING_2D_H

#define ALEMDAR_LAYER_MEAN_POOLING_2D_H

ALEMDAR_DEF void layer_mean_pooling_2d_forward(Layer layer);
ALEMDAR_DEF Layer layer_mean_pooling_2d(size_t inputSize, int poolSize);

void layer_mean_pooling_2d_forward(Layer layer) {
    int index = 0;
    float mean_val;
    for (size_t i = 0; i < layer.inputSize; i += layer.params.data[0]) {
        mean_val = layer.input.data[i];
        for (size_t j = 1; j < layer.params.data[0]; j++) {
            mean_val += layer.input.data[i + j];
        }
        layer.output.data[index++] = mean_val / layer.params.data[0];
    }
}

Layer layer_mean_pooling_2d(size_t inputSize, int poolSize) {
    ALEMDAR_ASSERT(poolSize > 0);
    Layer l;
    l.name = MeanPooling2D;
    l.inputSize = inputSize;
    l.outputSize = inputSize / poolSize;
    l.input = vec_alloc(inputSize);
    l.output = vec_alloc(l.outputSize);
    l.forward = layer_mean_pooling_2d_forward;
    l.bias = vec_alloc(0);
    l.weight = matrix_alloc(0, 0);
    l.params = vec_alloc(1);
    l.params.data[0] = poolSize;
    return l;
}

#endif // !ALEMDAR_LAYER_MEAN_POOLING_2D_H