#ifndef ALEMDAR_LAYER_ACTIVATION_H

#define ALEMDAR_LAYER_ACTIVATION_H

typedef enum {
	Sigmoid,
    Tanh,
    Softmax,
} ActivationTypes;

ALEMDAR_DEF void layer_activation_sigmoid_forward(Layer layer);
ALEMDAR_DEF void layer_activation_tanh_forward(Layer layer);
ALEMDAR_DEF void layer_activation_softmax_forward(Layer layer);
ALEMDAR_DEF void layer_activation_sigmoid_backward(Layer layer);
ALEMDAR_DEF void layer_activation_tanh_backward(Layer layer);
ALEMDAR_DEF void layer_activation_softmax_backward(Layer layer);
ALEMDAR_DEF Layer layer_activation(ActivationTypes activation);

void layer_activation_sigmoid_forward(Layer layer) {
    for (size_t i = 0; i < layer.outputSize; i++)
    {
        layer.output.data[i] = sigmoid(layer.input.data[i]);
    }
}
void layer_activation_tanh_forward(Layer layer) {
    for (size_t i = 0; i < layer.outputSize; i++)
    {
        layer.output.data[i] = tanhf(layer.input.data[i]);
    }
}
void layer_activation_softmax_forward(Layer layer) {
    float max_val = layer.input.data[0];
    for (int i = 1; i < layer.inputSize; i++) {
        if (layer.input.data[i] > max_val) {
            max_val = layer.input.data[i];
        }
    }
    float sum = 0.0;
    for (size_t i = 0; i < layer.inputSize; i++)
    {
        layer.output.data[i] = expf(layer.input.data[i] - max_val);
        sum += layer.output.data[i];
    }
    for (size_t i = 0; i < layer.inputSize; i++)
    {
        layer.output.data[i] /= sum;
    }
    
}
void layer_activation_sigmoid_backward(Layer layer) {
    for (size_t i = 0; i < layer.outputSize; i++)
    {
        layer.input.data[i] = dsigmoid(layer.output.data[i]);
    }
}
void layer_activation_tanh_backward(Layer layer) {
    for (size_t i = 0; i < layer.outputSize; i++)
    {
        layer.input.data[i] = dtanh(layer.output.data[i]);
    }
}
void layer_activation_softmax_backward(Layer layer) {
    for (int i = 0; i < layer.inputSize; i++) {
        float softmax_i = layer.output.data[i];
        for (int j = 0; j < layer.outputSize; j++) {
            if (i == j) {
                layer.input.data[i] *= softmax_i * (1.0 - softmax_i);
            } else {
                layer.input.data[i] *= -softmax_i * layer.output.data[j];
            }
        }
    }
}

Layer layer_activation(ActivationTypes activation) {
    Layer l;
    l.name = Activation;
    l.inputSize = 0;
    l.outputSize = 0;
    l.input = vec_alloc(l.inputSize);
    l.output = vec_alloc(l.outputSize);
    l.bias = vec_alloc(0);
    l.weight = matrix_alloc(0, 0);
    l.params = vec_alloc(1);
    l.params.data[0] = activation;
    switch (activation)
    {
    case Sigmoid:
        l.forward = layer_activation_sigmoid_forward;
        l.backward = layer_activation_sigmoid_backward;
        break;
    case Tanh:
        l.forward = layer_activation_tanh_forward;
        l.backward = layer_activation_tanh_backward;
        break;
    case Softmax:
        l.forward = layer_activation_softmax_forward;
        l.backward = layer_activation_softmax_backward;
    }
    return l;
}

#endif // !ALEMDAR_LAYER_ACTIVATION_H