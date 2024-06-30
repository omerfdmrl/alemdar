#ifndef ALEMDAR_LAYER_GRU_H

#define ALEMDAR_LAYER_GRU_H

ALEMDAR_DEF void layer_gru_forward(Layer layer);
ALEMDAR_DEF Layer layer_gru(size_t inputSize, size_t outputSize);

void layer_gru_forward(Layer layer) {
    Vec input = layer.input;
    Vec params = layer.params;
    Vec output = layer.output;
    Matrix weight = layer.weight;

    Vec bias_reset = vec_slice(layer.bias, 0, layer.outputSize);
    Vec bias_update = vec_slice(layer.bias, layer.outputSize, layer.outputSize * 2);
    Vec bias_hidden = vec_slice(layer.bias, layer.outputSize * 2, layer.outputSize * 3);

    size_t inputSize = layer.inputSize;
    size_t outputSize = layer.outputSize;

    for (size_t j = 0; j < outputSize; j++) {
        output.data[j] = 0;
    }

    for (size_t j = 0; j < outputSize; j++) {
        float reset_gate = 0;
        float update_gate = 0;

        for (size_t i = 0; i < inputSize; i++) {
            reset_gate += input.data[i] * weight.data[i][j];
            update_gate += input.data[i] * weight.data[inputSize][j];
        }

        reset_gate += bias_reset.data[j];
        update_gate += bias_update.data[j];

        reset_gate = sigmoid(reset_gate);
        update_gate = sigmoid(update_gate);

        float hidden_value = 0;
        for (size_t i = 0; i < inputSize; i++) {
            hidden_value += input.data[i] * weight.data[inputSize + 1][j];
        }

        hidden_value += reset_gate * params.data[j] * weight.data[inputSize + 2][j];
        hidden_value += bias_hidden.data[j];
        hidden_value = tanh(hidden_value);

        output.data[j] = (1 - update_gate) * params.data[j] + update_gate * hidden_value;
        params.data[j] = output.data[j];
    }
}

Layer layer_gru(size_t inputSize, size_t outputSize) {
    Layer l;
    l.name = GRU;
    l.inputSize = inputSize;
    l.outputSize = outputSize;
    l.input = vec_alloc(inputSize);
    l.output = vec_alloc(outputSize);
    l.forward = layer_gru_forward;
    l.bias = vec_alloc(outputSize * 3);
    l.weight = matrix_alloc(inputSize + 3, outputSize);
    l.params = vec_alloc(outputSize);

    return l;
}

#endif // !ALEMDAR_LAYER_GRU_H