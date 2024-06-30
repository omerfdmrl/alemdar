#ifndef ALEMDAR_LAYER_DROPOUT_H

#define ALEMDAR_LAYER_DROPOUT_H

ALEMDAR_DEF void layer_dropout_forward(Layer layer);
Layer layer_dropout(size_t inputSize, float rate);

void layer_dropout_forward(Layer layer) {
    for (size_t i = 0; i < layer.outputSize; i++)
    {
        float rand = random_uniform(0, 1);
        if(rand < layer.params.data[0]) {
            layer.output.data[i] = 0;    
        }else {
            layer.output.data[i] = layer.input.data[i];
        }
    }
}

Layer layer_dropout(size_t inputSize, float rate) {
    ALEMDAR_ASSERT(1 > rate);
    ALEMDAR_ASSERT(rate > 0);
    Layer l;
    l.name = Dropout;
    l.inputSize = inputSize;
    l.outputSize = inputSize;
    l.input = vec_alloc(inputSize);
    l.output = vec_alloc(inputSize);
    l.forward = layer_dropout_forward;
    l.bias = vec_alloc(0);
    l.weight = matrix_alloc(0, 0);
    l.params = vec_alloc(1);
    l.params.data[0] = rate;
    return l;
}

#endif // !ALEMDAR_LAYER_DROPOUT_H