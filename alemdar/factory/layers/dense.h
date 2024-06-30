#ifndef ALEMDAR_LAYER_DENSE_H

#define ALEMDAR_LAYER_DENSE_H

ALEMDAR_DEF void layer_dense_forward(Layer layer);
ALEMDAR_DEF Layer layer_dense(size_t inputSize, size_t outputSize);

void layer_dense_forward(Layer layer) {
    for (size_t i = 0; i < layer.outputSize; i++)
    {
        layer.output.data[i] = 0;
        for (size_t j = 0; j < layer.inputSize; j++) {
            layer.output.data[i] += layer.weight.data[j][i] * layer.input.data[j];
        }
        layer.output.data[i] += layer.bias.data[i];
    }
}

Layer layer_dense(size_t inputSize, size_t outputSize) {
    Layer l;
    l.name = Dense;
    l.inputSize = inputSize;
    l.outputSize = outputSize;
    l.input = vec_alloc(inputSize);
    l.output = vec_alloc(outputSize);
    l.forward = layer_dense_forward;
    l.bias = vec_alloc(outputSize);
    l.weight = matrix_alloc(inputSize, outputSize);
    l.params = vec_alloc(0);
    return l;
}

#endif // !ALEMDAR_LAYER_DENSE_H