#ifndef ALEMDAR_IO_H

#define ALEMDAR_IO_H

#ifdef ALEMDAR_IO_IMPLEMENTATION

ALEMDAR_DEF Model import_model(const char *fileName);
ALEMDAR_DEF void export_model(Model model, const char *fileName);

Model import_model(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    ALEMDAR_ASSERT(file != NULL);

    char line[202400];
    bool isLayer = false;
    int layer_count = 0;
    char **op_type = NULL;
    int *inputs = NULL;
    int *outputs = NULL;
    float **bias_values = NULL;
    int *bias_counts = NULL;
    float **weight_values = NULL;
    int *weight_counts = NULL;
    float **param_values = NULL;
    int *param_counts = NULL;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "\"layers\": [") != NULL) {
            isLayer = true;
            op_type = (char **)malloc(sizeof(char *));
            inputs = (int *)malloc(sizeof(int));
            outputs = (int *)malloc(sizeof(int));
            bias_values = (float **)malloc(sizeof(float *));
            bias_counts = (int *)malloc(sizeof(int));
            weight_values = (float **)malloc(sizeof(float *));
            weight_counts = (int *)malloc(sizeof(int));
            param_values = (float **)malloc(sizeof(float *));
            param_counts = (int *)malloc(sizeof(int));

            if (op_type == NULL || inputs == NULL || outputs == NULL ||
                bias_values == NULL || bias_counts == NULL ||
                weight_values == NULL || weight_counts == NULL ||
                param_values == NULL || param_counts == NULL) {
                fclose(file);
                exit(EXIT_FAILURE);
            }
            continue;
        } else if (isLayer && strstr(line, "]\n}") != NULL) {
            break;
        } else if (isLayer && strstr(line, "{") != NULL) {
            if (layer_count > 0) {
                op_type = (char **)realloc(op_type, (layer_count + 1) * sizeof(char *));
                inputs = (int *)realloc(inputs, (layer_count + 1) * sizeof(int));
                outputs = (int *)realloc(outputs, (layer_count + 1) * sizeof(int));
                bias_values = (float **)realloc(bias_values, (layer_count + 1) * sizeof(float *));
                bias_counts = (int *)realloc(bias_counts, (layer_count + 1) * sizeof(int));
                weight_values = (float **)realloc(weight_values, (layer_count + 1) * sizeof(float *));
                weight_counts = (int *)realloc(weight_counts, (layer_count + 1) * sizeof(int));
                param_values = (float **)realloc(param_values, (layer_count + 1) * sizeof(float *));
                param_counts = (int *)realloc(param_counts, (layer_count + 1) * sizeof(int));

                if (op_type == NULL || inputs == NULL || outputs == NULL ||
                    bias_values == NULL || bias_counts == NULL ||
                    weight_values == NULL || weight_counts == NULL ||
                    param_values == NULL || param_counts == NULL) {
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
            }
        } else if (isLayer && strstr(line, "}") != NULL) {
            layer_count++;
        } else if (isLayer && strstr(line, "\"op_type\":") != NULL) {
            char *op_type_token = strtok(line, ":");
            op_type_token = strtok(NULL, "\"");
            op_type_token = strtok(NULL, "\"");
            op_type[layer_count] = strdup(op_type_token);
        } else if (isLayer && strstr(line, "\"inputs\":") != NULL) {
            char *inputs_token = strtok(line, ":");
            inputs_token = strtok(NULL, ",");
            inputs[layer_count] = atoi(inputs_token);
        } else if (isLayer && strstr(line, "\"outputs\":") != NULL) {
            char *outputs_token = strtok(line, ":");
            outputs_token = strtok(NULL, ",");
            outputs[layer_count] = atoi(outputs_token);
        } else if (isLayer && strstr(line, "\"bias\": [") != NULL) {
            bias_values[layer_count] = (float *)malloc(sizeof(float) * outputs[layer_count]);
            if (bias_values[layer_count] == NULL) {
                fclose(file);
                exit(EXIT_FAILURE);
            }
            bias_counts[layer_count] = 0;
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "]")) break;
                char *bias_token = strtok(line, ",");
                bias_values[layer_count][bias_counts[layer_count]++] = atof(bias_token);
            }
        } else if (isLayer && strstr(line, "\"weight\": [") != NULL) {
            weight_values[layer_count] = (float *)malloc(sizeof(float) * outputs[layer_count] * inputs[layer_count]);
            if (weight_values[layer_count] == NULL) {
                fclose(file);
                exit(EXIT_FAILURE);
            }
            weight_counts[layer_count] = 0;
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "]")) break;
                char *weight_token = strtok(line, ",");
                weight_values[layer_count][weight_counts[layer_count]++] = atof(weight_token);
            }
        } else if (isLayer && strstr(line, "\"params\": [") != NULL) {
            param_values[layer_count] = (float *)malloc(sizeof(float) * 5);
            if (param_values[layer_count] == NULL) {
                fclose(file);
                exit(EXIT_FAILURE);
            }
            param_counts[layer_count] = 0;
            if (strstr(line, "]")) {
                continue;
            }
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "]")) break;
                char *param_token = strtok(line, ",");
                param_values[layer_count][param_counts[layer_count]++] = atof(param_token);
            }
        }
    }

    Model model = model_alloc(layer_count - 1);
    for (size_t i = 0; i < layer_count - 1; i++)
    {
        bool isLayer = false;
        Layer layer;
        if(!op_type[i]) break;
        if(strcmp(op_type[i], "Activation") == 0) {
            isLayer = true;
            layer = layer_activation((ActivationTypes)param_values[i][0]);
        }else if(strcmp(op_type[i], "Dense") == 0) {
            isLayer = true;
            layer = layer_dense(inputs[i],outputs[i]);
            for (size_t j = 0; j < layer.bias.size; j++)
            {
                layer.bias.data[j] = bias_values[i][j];
            }
            size_t w = 0;
            for (size_t j = 0; j < layer.weight.rows; j++)
            {
                for (size_t k = 0; k < layer.weight.cols; k++)
                {
                    layer.weight.data[j][k] = weight_values[i][w];
                    w++;
                }
            }
        }else if(strcmp(op_type[i], "MaxPooling") == 0) {
            isLayer = true;
            layer = layer_max_pooling(inputs[i],param_values[i][0]);
        }else if(strcmp(op_type[i], "MenaPooling") == 0) {
            isLayer = true;
            layer = layer_mean_pooling(inputs[i],param_values[i][0]);
        }else if(strcmp(op_type[i], "MinPooling") == 0) {
            isLayer = true;
            layer = layer_min_pooling(inputs[i],param_values[i][0]);
        }else if(strcmp(op_type[i], "Conv") == 0) {
            isLayer = true;
            layer = layer_conv(inputs[i],param_values[i][0], param_values[i][1], param_values[i][2], param_values[i][3], param_values[i][4],param_values[i][5]);
        }
        if(isLayer) model_add(&model, layer);
    }
    
    for (int i = 0; i < layer_count - 1; ++i) {
        /* free(weight_values[i]); */
        free(op_type[i]);
        free(bias_values[i]);
        free(param_values[i]);
    }
    free(op_type);
    free(inputs);
    free(outputs);
    free(bias_values);
    free(bias_counts);
    free(weight_values);
    free(weight_counts);
    free(param_values);
    free(param_counts);
    fclose(file);

    return model;
}

void export_model(Model model, const char *fileName) {
    FILE *file = fopen(fileName, "w");
    ALEMDAR_ASSERT(file != NULL);

    fprintf(file, "{\n");
    fprintf(file, "  \"op_type\": \"NeuralNetwork\",\n");
    fprintf(file, "  \"metadata\": {\n");
    fprintf(file, "    \"description\": \"Pretrained model for ALEMDAR\"\n");
    fprintf(file, "  },\n");
    fprintf(file, "  \"layers\": [\n");
    for (size_t i = 0; i < model.layer_count; i++)
    {
        Layer layer = model.layers[i];
        fprintf(file, "    {\n");
        fprintf(file, "      \"op_type\": \"%s\",\n", LAYER_NAME(layer.name));
        fprintf(file, "      \"name\": \"%s\",\n", LAYER_NAME(layer.name));
        fprintf(file, "      \"inputs\": %zu,\n", layer.inputSize);
        fprintf(file, "      \"outputs\": %zu,\n", layer.outputSize);

        fprintf(file, "      \"bias\": [");
        for (size_t j = 0; j < layer.bias.size; j++) {
            fprintf(file, "\n        %f%s", layer.bias.data[j], (j == layer.bias.size - 1) ? "\n      " : ",");
        }
        fprintf(file, "],\n");

        fprintf(file, "      \"weight\": [");
        for (size_t j = 0; j < layer.weight.rows; j++) {
            for (size_t k = 0; k < layer.weight.cols; k++)
            {
                fprintf(file, "\n        %f%s", layer.weight.data[j][k], (j == layer.weight.rows - 1 && k == layer.weight.cols - 1) ? "\n      " : ",");
            }
        }
        fprintf(file, "],\n");

        fprintf(file, "      \"params\": [");
        for (size_t j = 0; j < layer.params.size; j++) {
            fprintf(file, "\n        %f%s", layer.params.data[j], (j == layer.params.size - 1) ? "\n      " : ",");
        }
        fprintf(file, "]\n");

        if(i != model.layer_count - 1)
            fprintf(file, "    },\n");
        else fprintf(file, "    }\n");
    }
    fprintf(file, "  ]\n");
    fprintf(file, "}\n");
}

#endif // ALEMDAR_IO_IMPLEMENTATION

#endif // !ALEMDAR_IO_H