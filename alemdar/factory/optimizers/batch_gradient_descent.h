#ifndef ALEMDAR_OPTIMIZER_BATCH_GRADIENT_DESCENT_H

#define ALEMDAR_OPTIMIZER_BATCH_GRADIENT_DESCENT_H

ALEMDAR_DEF void model_learn_batch_gradient_descent(Model model, Matrix inputs, Matrix outputs, va_list args);

void model_learn_batch_gradient_descent(Model model, Matrix inputs, Matrix outputs, va_list args) {
    float rate = va_arg(args, double);
    if(rate == 0.0) {
        rate = 1e-1;
    }
    for (size_t i = 0; i < inputs.rows; i++)
    {
        float *input = inputs.data[i];
        float *output = outputs.data[i];
        model_input(model, input);
        model_forward(model);
        float *parsed = MODEL_OUTPUT(model);
        bool first = true;
        float **delta = (float **)ALEMDAR_MALLOC(sizeof(float) * model.layer_count);
        for (int l = model.layer_count - 1; l >= 0; l--)
        {
            Layer layer = model.layers[l];
            delta[l] = (float *)ALEMDAR_MALLOC(layer.outputSize * sizeof(float));
            if(layer.name == Activation) {
                layer.backward(layer);
                continue;
            }
            if(l + 1 <= l)
                vec_trans(layer.output, model.layers[l + 1].input);
            Layer prevLayer;
            float *prevDelta;
            for (size_t ll = l + 1; ll < model.layer_count - 1; ll++)
            {
                if(model.layers[ll].name != Activation){
                    prevLayer = model.layers[ll];
                    prevDelta = delta[ll];
                    break;
                }
            }
            if(first) {
                for (size_t j = 0; j < layer.output.size; j++)
                {
                    delta[l][j] = (output[j] - parsed[j]) * layer.output.data[j];
                }
                first = false;
            }else {
                for (size_t j = 0; j < layer.outputSize; j++)
                {
                    delta[l][j] = 0.0;
                    for (size_t k = 0; k < prevLayer.outputSize; k++)  
                    {
                        delta[l][j] += prevDelta[k] * prevLayer.weight.data[k][j];
                    }
                    delta[l][j] *= layer.output.data[j];
                }
            }
            for (size_t j = 0; j < layer.inputSize; j++)
            {
                for (size_t k = 0; k < layer.outputSize; k++)
                {
                    float dw = rate * delta[l][k] * layer.input.data[j];
                    layer.weight.data[j][k] += dw;
                }
            }
            for (size_t j = 0; j < layer.outputSize; j++)
            {
                float db = delta[l][j] * rate;
                layer.bias.data[j] += db;
            }
            
        }
        for (size_t l = 0; l < model.layer_count - 1; l++)
        {
            if (delta[l] != NULL) {
                ALEMDAR_FREE(delta[l]);
            }
        }
        ALEMDAR_FREE(delta);
        printf("%zu \n", inputs.rows);
        if(inputs.rows != 1) 
            print_progress(i, inputs.rows, model_cost(model, inputs.data[i], outputs.data[i]));
    }
}

#endif // !ALEMDAR_OPTIMIZER_BATCH_GRADIENT_DESCENT_H