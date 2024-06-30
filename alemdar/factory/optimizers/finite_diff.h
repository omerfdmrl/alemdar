#ifndef ALEMDAR_OPTIMIZER_FINITE_DIFF_H

#define ALEMDAR_OPTIMIZER_FINITE_DIFF_H

ALEMDAR_DEF void model_learn_finite_diff(Model model, Matrix inputs, Matrix outputs, va_list args);

void model_learn_finite_diff(Model model, Matrix inputs, Matrix outputs, va_list args) {
    float rate = va_arg(args, double);
    float eps = va_arg(args, double);
    if(rate == 0.0) {
        rate = 3e-2;
    }
    if(eps == 0.0) {
        eps = 1e-1;
    }
    for (size_t i = 0; i < inputs.rows; i++)
    {
        float *input = inputs.data[i];
        float *output = outputs.data[i];
        float c = model_cost(model, input, output);
        float saved;
        float g;
        for (size_t l = 0; l < model.layer_count; l++)
        {
            if(model.layers[l].name != Dense) continue;
            for (size_t j = 0; j < model.layers[l].inputSize; j++)
            {
                for (size_t k = 0; k < model.layers[l].outputSize; k++)
                {
                    saved = model.layers[l].weight.data[j][k];
                    model.layers[l].weight.data[j][k] += eps;
                    g = (model_cost(model, input, output) - c) / eps;
                    model.layers[l].weight.data[j][k] = saved;
                    model.layers[l].weight.data[j][k] -= rate * g;

                    saved = model.layers[l].bias.data[k];
                    model.layers[l].bias.data[k] += eps;
                    g = (model_cost(model, input, output) - c) / eps;
                    model.layers[l].bias.data[k] = saved;
                    model.layers[l].bias.data[k] -= rate * g;
                }
            }
        }
        print_progress(i, inputs.rows, model_cost(model, inputs.data[i], outputs.data[i]));
    }
}

#endif // !ALEMDAR_OPTIMIZER_FINITE_DIFF_H