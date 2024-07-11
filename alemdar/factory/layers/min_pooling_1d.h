#ifndef ALEMDAR_LAYER_MIN_POOLING_1D_H

#define ALEMDAR_LAYER_MIN_POOLING_1D_H

ALEMDAR_DEF void layer_min_pooling_1d_forward(Layer layer);
ALEMDAR_DEF Layer layer_min_pooling_1d(size_t inputSize, int poolSize);

void layer_min_pooling_1d_forward(Layer layer) {
    int index = 0;
    float min_val;
    for (size_t i = 0; i < layer.inputSize; i += layer.params.data[0]) {
        min_val = layer.input.data[i];
        for (size_t j = 1; j < layer.params.data[0]; j++) {
            if (layer.input.data[i + j] < min_val) {
                min_val = layer.input.data[i + j];
            }
        }
        layer.output.data[index++] = min_val;
    }
}

Layer layer_min_pooling_1d(size_t inputSize, int poolSize) {
    ALEMDAR_ASSERT(poolSize > 0);
    Layer l;
    l.name = MinPooling1D;
    l.inputSize = inputSize;
    l.outputSize = (inputSize + poolSize - 1) / poolSize;
    l.input = vec_alloc(inputSize);
    l.output = vec_alloc(l.outputSize);
    l.forward = layer_min_pooling_1d_forward;
    l.bias = vec_alloc(0);
    l.weight = matrix_alloc(0, 0);
    l.params = vec_alloc(0);
    l.params.data[0] = poolSize;
    return l;
}

#endif // !ALEMDAR_LAYER_MIN_POOLING_1D_H