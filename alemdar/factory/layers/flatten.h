#ifndef ALEMDAR_LAYER_FLATTEN_H

#define ALEMDAR_LAYER_FLATTEN_H

ALEMDAR_DEF void layer_flatten_forward(Layer layer);
ALEMDAR_DEF Layer layer_flatten(size_t imageCount, size_t imageWidth, size_t imageHeight);

void layer_flatten_forward(Layer layer) {
    
}
/* 150(image count) -> 150*150(image pixel size) -> 4 (rgba); */
/* 150(image count) -> 150*150(image pixel size) * 4 (rgba) */
Layer layer_flatten(size_t imageCount, size_t imageWidth, size_t imageHeight) {
    Layer l;
    l.name = Flatten;
    l.inputSize = imageCount;
    l.outputSize = imageCount;
    l.input = vec_alloc(l.inputSize);
    l.output = vec_alloc(l.outputSize);
    l.forward = layer_flatten_forward;
    l.bias = vec_alloc(0);
    l.weight = matrix_alloc(0, 0);
    l.params = vec_alloc(0);
    return l;
}

#endif // !ALEMDAR_LAYER_FLATTEN_H