#ifndef ALEMDAR_LAYER_MAX_POOLING_H

#define ALEMDAR_LAYER_MAX_POOLING_H

ALEMDAR_DEF void layer_max_pooling_forward(Layer layer);
ALEMDAR_DEF Layer layer_max_pooling(size_t inputSize, int poolSize);

void layer_max_pooling_forward(Layer layer) {
    int index = 0;
    float max_val;
    for (size_t i = 0; i < layer.inputSize; i += layer.params.data[0]) {
        max_val = layer.input.data[i];
        for (size_t j = 1; j < layer.params.data[0]; j++) {
            if (layer.input.data[i + j] > max_val) {
                max_val = layer.input.data[i + j];
            }
        }
        layer.output.data[index++] = max_val;
    }
}

Layer layer_max_pooling(size_t inputSize, int poolSize) {
    ALEMDAR_ASSERT(poolSize > 0);
    Layer l;
    l.name = MaxPooling;
    l.inputSize = inputSize;
    l.outputSize = (inputSize + poolSize - 1) / poolSize;
    l.input = vec_alloc(inputSize);
    l.output = vec_alloc(l.outputSize);
    l.forward = layer_max_pooling_forward;
    l.bias = vec_alloc(0);
    l.weight = matrix_alloc(0, 0);
    l.params = vec_alloc(1);
    l.params.data[0] = poolSize;
    return l;
}

#endif // !ALEMDAR_LAYER_MAX_POOLING_H