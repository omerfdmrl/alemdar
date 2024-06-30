#ifndef ALEMDAR_LAYER_SHUFFLE_H

#define ALEMDAR_LAYER_SHUFFLE_H

ALEMDAR_DEF void layer_shuffle_forward(Layer layer);
ALEMDAR_DEF Layer layer_shuffle(size_t inputSize, size_t outputSize);

void layer_shuffle_forward(Layer layer) {
    int *used_indices = (int *)malloc(layer.outputSize * sizeof(int));
    int source_index;
    for (size_t i = 0; i < layer.outputSize; i++) {
        used_indices[i] = 0;
    }
    for (size_t i = layer.outputSize - 1; i > 0; i--)
    {
        source_index = rand() % layer.outputSize;
        while (used_indices[source_index]) {
            source_index = rand() % layer.outputSize;
        }
        used_indices[source_index] = 1;
        layer.output.data[i] = layer.input.data[source_index];
    }
}

Layer layer_shuffle(size_t inputSize, size_t outputSize) {
    Layer l;
    l.name = Shuffle;
    l.inputSize = inputSize;
    l.outputSize = outputSize;
    l.input = vec_alloc(inputSize);
    l.output = vec_alloc(outputSize);
    l.forward = layer_shuffle_forward;
    l.bias = vec_alloc(0);
    l.weight = matrix_alloc(0, 0);
    l.params = vec_alloc(0);
    return l;
}

#endif // !ALEMDAR_LAYER_SHUFFLE_H