#ifndef ALEMDAR_INITIALIZER_RANDOM_UNIFORM_H

#define ALEMDAR_INITIALIZER_RANDOM_UNIFORM_H

ALEMDAR_DEF void model_initializer_random_uniform(Model model, va_list args);

void model_initializer_random_uniform(Model model, va_list args) {
    float low = va_arg(args, double);
    float high = va_arg(args, double);
    if(low == 0.0) {
        low = -0.05;
    }
    if(high == 0.0) {
        high = 0.05;
    }
    ALEMDAR_ASSERT(high > low);
    for (size_t i = 0; i < model.layer_count; i++)
    {
        for (size_t j = 0; j < model.layers[i].weight.rows; j++)
        {
            for (size_t k = 0; k < model.layers[i].weight.cols; k++)
            {
                model.layers[i].weight.data[j][k] = random_uniform(low, high);
            }
        }
        for (size_t j = 0; j < model.layers[i].bias.size; j++)
        {
            model.layers[i].bias.data[j] = random_uniform(low, high);
        }
    }
}

#endif // !ALEMDAR_INITIALIZER_RANDOM_UNIFORM_H