#ifndef ALEMDAR_INITIALIZER_RANDOM_XAVIER_H

#define ALEMDAR_INITIALIZER_RANDOM_XAVIER_H

ALEMDAR_DEF void model_initializer_random_xavier(Model model, va_list args);

void model_initializer_random_xavier(Model model, va_list args) {
    for (size_t i = 0; i < model.layer_count; i++)
    {
        float a = random_xavier(model.layers[i].inputSize, model.layers[i].outputSize);
        for (size_t j = 0; j < model.layers[i].weight.rows; j++)
        {
            for (size_t k = 0; k < model.layers[i].weight.cols; k++)
            {
                model.layers[i].weight.data[j][k] = random_xavier_rand(a);
            }
        }
        for (size_t j = 0; j < model.layers[i].bias.size; j++)
        {
            model.layers[i].bias.data[j] = random_xavier_rand(a);
        }
    }
}

#endif // !ALEMDAR_INITIALIZER_RANDOM_XAVIER_H