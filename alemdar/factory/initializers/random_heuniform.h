#ifndef ALEMDAR_INITIALIZER_RANDOM_HEUNIFORM_H

#define ALEMDAR_INITIALIZER_RANDOM_HEUNIFORM_H

ALEMDAR_DEF void model_initializer_random_heuniform(Model model, va_list args);

void model_initializer_random_heuniform(Model model, va_list args) {
    for (size_t i = 0; i < model.layer_count; i++)
    {
        for (size_t j = 0; j < model.layers[i].weight.rows; j++)
        {
            for (size_t k = 0; k < model.layers[i].weight.cols; k++)
            {
                model.layers[i].weight.data[j][k] = random_heuniform(model.layers[i].inputSize, model.layers[i].outputSize);
            }
        }
        for (size_t j = 0; j < model.layers[i].bias.size; j++)
        {
            model.layers[i].bias.data[j] = random_heuniform(model.layers[i].inputSize, model.layers[i].outputSize);
        }
    }
}

#endif // !ALEMDAR_INITIALIZER_RANDOM_HEUNIFORM_H