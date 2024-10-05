#ifndef ALEMDAR_LAYER_CONV_H

#define ALEMDAR_LAYER_CONV_H

ALEMDAR_DEF void layer_conv_forward(Layer layer);
ALEMDAR_DEF Layer layer_conv(size_t input_width, size_t input_height, size_t input_channels, size_t output_channels, size_t kernel_size, size_t stride, size_t padding);

void layer_conv_forward(Layer layer) {
    size_t kernel_size = layer.params.data[4]; // Assuming square kernel
    size_t stride = layer.params.data[5]; // Stride
    size_t padding = layer.params.data[6]; // Padding
    size_t input_width = layer.params.data[0];
    size_t input_height = layer.params.data[1];
    size_t output_width = (input_width - kernel_size + 2 * padding) / stride + 1;
    size_t output_height = (input_height - kernel_size + 2 * padding) / stride + 1;

    // Apply padding to the input if necessary
    // ...

    for (size_t y = 0; y < output_height; ++y) {
        for (size_t x = 0; x < output_width; ++x) {
            float sum = 0.0f;
            for (size_t ky = 0; ky < kernel_size; ++ky) {
                for (size_t kx = 0; kx < kernel_size; ++kx) {
                    size_t input_y = y * stride + ky;
                    size_t input_x = x * stride + kx;
                    size_t kernel_idx = ky * kernel_size + kx; // Flatten the kernel index
                    size_t input_idx = input_y * input_width + input_x; // Flatten the input index
                    
                    // Ensure indices are within bounds
                    if (input_y < input_height && input_x < input_width) {
                        sum += layer.weight.data[kernel_idx][0] * layer.input.data[input_idx];
                    }
                }
            }
            layer.output.data[y * output_width + x] = sum;
        }
    }
}

Layer layer_conv(size_t input_width, size_t input_height, size_t input_channels, size_t output_channels, size_t kernel_size, size_t stride, size_t padding) {
    Layer l;
    l.name = Conv;
    l.inputSize = input_width * input_height * input_channels;
    l.outputSize = (input_width / stride - kernel_size + 1) * (input_height / stride - kernel_size + 1) * output_channels;
    l.input = vec_alloc(l.inputSize);
    l.output = vec_alloc(l.outputSize);
    l.bias = vec_alloc(output_channels);
    l.weight = matrix_alloc(kernel_size * kernel_size * input_channels, output_channels);
    l.params = vec_alloc(7);
    l.params.data[0] = input_width;
    l.params.data[1] = input_height;
    l.params.data[2] = input_channels;
    l.params.data[3] = output_channels;
    l.params.data[4] = kernel_size;
    l.params.data[5] = stride;
    l.params.data[6] = padding;
    l.forward = layer_conv_forward;
    return l;
}

#endif // !ALEMDAR_LAYER_CONV_H