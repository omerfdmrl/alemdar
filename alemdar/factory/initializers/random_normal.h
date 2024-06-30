#ifndef ALEMDAR_INITIALIZER_RANDOM_NORMAL_H

#define ALEMDAR_INITIALIZER_RANDOM_NORMAL_H

ALEMDAR_DEF void model_initializer_random_normal(Model model, va_list args);

void model_initializer_random_normal(Model model, va_list args) {
    float mean = va_arg(args, double);
    float stddev = va_arg(args, double);
    if(stddev == 0.0) {
        stddev = 0.05;
    }
    ALEMDAR_ASSERT(mean != stddev);
    for (size_t i = 0; i < model.layer_count; i++)
    {
        for (size_t j = 0; j < model.layers[i].weight.rows; j++)
        {
            for (size_t k = 0; k < model.layers[i].weight.cols; k++)
            {
                model.layers[i].weight.data[j][k] = random_normal(mean, stddev);
            }
        }
        for (size_t j = 0; j < model.layers[i].bias.size; j++)
        {
            model.layers[i].bias.data[j] = random_normal(mean, stddev);
        }
    }
}

#endif // !ALEMDAR_INITIALIZER_RANDOM_NORMAL_H