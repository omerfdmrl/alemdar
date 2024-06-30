#ifndef ALEMDAR_INITIALIZER_ZEROS_H

#define ALEMDAR_INITIALIZER_ZEROS_H

ALEMDAR_DEF void model_initializer_zeros(Model model, va_list args);

void model_initializer_zeros(Model model, va_list args) {
    for (size_t i = 0; i < model.layer_count; i++)
    {
        for (size_t j = 0; j < model.layers[i].weight.rows; j++)
        {
            for (size_t k = 0; k < model.layers[i].weight.cols; k++)
            {
                model.layers[i].weight.data[j][k] = 0;
            }
        }
        for (size_t j = 0; j < model.layers[i].bias.size; j++)
        {
            model.layers[i].bias.data[j] = 0;
        }
    }
}

#endif // !ALEMDAR_INITIALIZER_ZEROS_H