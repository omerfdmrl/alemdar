#ifndef ALEMDAR_LAYER_RNN_H

#define ALEMDAR_LAYER_RNN_H

ALEMDAR_DEF void layer_rnn_forward(Layer layer);
ALEMDAR_DEF Layer layer_rnn(size_t inputSize, size_t outputSize);

void layer_rnn_forward(Layer layer) {
    for (size_t j = 0; j < layer.outputSize; j++) {
        layer.output.data[j] = 0;
        for (size_t i = 0; i < layer.inputSize; i++) {
            layer.output.data[j] += layer.input.data[i] * layer.weight.data[i][j];
        }
        layer.output.data[j] += layer.params.data[j] * layer.weight.data[layer.inputSize][j];
        layer.output.data[j] += layer.bias.data[j];
        layer.output.data[j] = tanh(layer.output.data[j]);
        layer.params.data[j] = layer.output.data[j];
    }
}

Layer layer_rnn(size_t inputSize, size_t outputSize) {
    Layer l;
    l.name = RNN;
    l.inputSize = inputSize;
    l.outputSize = outputSize;
    l.input = vec_alloc(inputSize);
    l.output = vec_alloc(outputSize);
    l.forward = layer_rnn_forward;
    l.bias = vec_alloc(outputSize);
    l.weight = matrix_alloc(inputSize + 1, outputSize);
    l.params = vec_alloc(outputSize);
    return l;
}

#endif // !ALEMDAR_LAYER_RNN_H