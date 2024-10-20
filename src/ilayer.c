#ifndef ILAYER_H

#define ILAYER_H

#include "ican.h"

Layer *layer_alloc(LayerNames name, size_t inputSize, size_t outputSize, size_t paramsSize, void (*forward)(Layer *layer), void (*backward)(Layer *layer)) {
	Layer *layer = malloc(sizeof(Layer));
    layer->name = name;
    layer->inputSize = inputSize;
    layer->outputSize = outputSize;
    layer->input = iray1d_alloc(inputSize);
    layer->output = iray1d_alloc(outputSize);
    layer->forward = forward;
    layer->backward = backward;
    layer->bias = iray1d_alloc(outputSize);
    layer->weight = iray2d_alloc(inputSize, outputSize);
    layer->params = iray1d_alloc(paramsSize);
	return layer;
}

void layer_free(Layer *layer) {
	iray1d_free(layer->bias);
	iray1d_free(layer->params);
	iray1d_free(layer->input);
	iray1d_free(layer->output);
	iray2d_free(layer->weight);
	free(layer);
}

void layer_print(Layer *layer) {
	size_t param = layer->bias->rows + layer->weight->cols * layer->weight->rows + layer->params->rows;
	printf("%-20s(%zu, %zu)%-16s%zu\n", ILAYER_NAME(layer->name), layer->inputSize, layer->outputSize,"", param);
}

void layer_dense_forward(Layer *layer) {
    for (size_t i = 0; i < layer->outputSize; i++)
    {
        layer->output->data[i] = 0;
        for (size_t j = 0; j < layer->inputSize; j++) {
            layer->output->data[i] += layer->weight->data[j][i] * layer->input->data[j];
        }
        layer->output->data[i] += layer->bias->data[i];
    }
}

void layer_activation_sigmoid_forward(Layer *layer) {
    for (size_t i = 0; i < layer->outputSize; i++)
    {
        layer->output->data[i] = sigmoid(layer->input->data[i]);
    }
}
void layer_activation_tanh_forward(Layer *layer) {
    for (size_t i = 0; i < layer->outputSize; i++)
    {
        layer->output->data[i] = tanhf(layer->input->data[i]);
    }
}
void layer_activation_softmax_forward(Layer *layer) {
    float max_val = layer->input->data[0];
    for (size_t i = 1; i < layer->inputSize; i++) {
        if (layer->input->data[i] > max_val) {
            max_val = layer->input->data[i];
        }
    }
    float sum = 0.0;
    for (size_t i = 0; i < layer->inputSize; i++)
    {
        layer->output->data[i] = expf(layer->input->data[i] - max_val);
        sum += layer->output->data[i];
    }
    for (size_t i = 0; i < layer->inputSize; i++)
    {
        layer->output->data[i] /= sum;
    }
    
}
void layer_activation_sigmoid_backward(Layer *layer) {
    for (size_t i = 0; i < layer->outputSize; i++)
    {
        layer->input->data[i] = dsigmoid(layer->output->data[i]);
    }
}
void layer_activation_tanh_backward(Layer *layer) {
    for (size_t i = 0; i < layer->outputSize; i++)
    {
        layer->input->data[i] = dtanh(layer->output->data[i]);
    }
}
void layer_activation_softmax_backward(Layer *layer) {
    for (size_t i = 0; i < layer->inputSize; i++) {
        float softmax_i = layer->output->data[i];
        for (size_t j = 0; j < layer->outputSize; j++) {
            if (i == j) {
                layer->input->data[i] *= softmax_i * (1.0 - softmax_i);
            } else {
                layer->input->data[i] *= -softmax_i * layer->output->data[j];
            }
        }
    }
}

void layer_dropout_forward(Layer *layer) {
    for (size_t i = 0; i < layer->outputSize; i++)
    {
        float rand = random_uniform(0, 1);
        if(rand < layer->params->data[0]) {
            layer->output->data[i] = 0;    
        }else {
            layer->output->data[i] = layer->input->data[i];
        }
    }
}

void layer_shuffle_forward(Layer *layer) {
    int *used_indices = (int *)malloc(layer->outputSize * sizeof(int));
    int source_index;
    for (size_t i = 0; i < layer->outputSize; i++) {
        used_indices[i] = 0;
    }
    for (size_t i = layer->outputSize - 1; i > 0; i--)
    {
        source_index = rand() % layer->outputSize;
        while (used_indices[source_index]) {
            source_index = rand() % layer->outputSize;
        }
        used_indices[source_index] = 1;
        layer->output->data[i] = layer->input->data[source_index];
    }
}

Layer *layer_dense(size_t inputSize, size_t outputSize) {
    return layer_alloc(Dense, inputSize, outputSize, 0, layer_dense_forward, NULL);
}

Layer *layer_activation(ActivationTypes activation) {
    Layer *layer = layer_alloc(Activation, 0, 0, 1, NULL, NULL);
    layer->params->data[0] = activation;
    switch (activation)
    {
    case Sigmoid:
        layer->forward = layer_activation_sigmoid_forward;
        layer->backward = layer_activation_sigmoid_backward;
        break;
    case Tanh:
        layer->forward = layer_activation_tanh_forward;
        layer->backward = layer_activation_tanh_backward;
        break;
    case Softmax:
        layer->forward = layer_activation_softmax_forward;
        layer->backward = layer_activation_softmax_backward;
    }
    return layer;
}

Layer *layer_dropout(float rate) {
    ISERT_MSG(1 > rate, "Rate should be less then 1");
    ISERT_MSG(rate > 0, "Rate should be more then 0");
    Layer *layer = layer_alloc(Dropout, 0, 0, 1, layer_dropout_forward, NULL);
    layer->params->data[0] = rate;
    return layer;
}

Layer *layer_shuffle(float rate) {
    ISERT_MSG(1 > rate, "Rate should be less then 1");
    ISERT_MSG(rate > 0, "Rate should be more then 0");
    Layer *layer = layer_alloc(Shuffle, 0, 0, 1, layer_shuffle_forward, NULL);
    layer->params->data[0] = rate;
    return layer;
}

#endif // ILAYER_H