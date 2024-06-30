#ifndef ALEMDAR_MODEL_H

#define ALEMDAR_MODEL_H

typedef struct
{
	size_t layer_count;
	Layer *layers;
} Model;

typedef enum
{
	FiniteDiff,
	BatchGradientDescent,
} ModelOptimizers;
typedef enum
{
	Zeros,
	Ones,
	RandomUniform,
	RandomNormal,
	RandomXavier,
	RandomHeUniform
} ModelInitializers;


ALEMDAR_DEF Model model_alloc(int layerCount);
ALEMDAR_DEF void model_add(Model *model, Layer layer);
ALEMDAR_DEF void model_randomize(ModelInitializers initializer, Model model, ...);
ALEMDAR_DEF void model_forward(Model model);
ALEMDAR_DEF void model_input(Model model, float input[]);
ALEMDAR_DEF float model_cost(Model model, float input[], float output[]);
ALEMDAR_DEF void model_learn(ModelOptimizers optimizer, Model model, size_t epoch, Matrix inputs, Matrix outputs, ...);
ALEMDAR_DEF void model_print(Model model);
ALEMDAR_DEF void model_free(Model model);
#define MODEL_OUTPUT(m) m.layers[m.layer_count - 1].output.data

#include "./optimizers/finite_diff.h"
#include "./optimizers/batch_gradient_descent.h"
#include "./initializers/zeros.h"
#include "./initializers/ones.h"
#include "./initializers/random_uniform.h"
#include "./initializers/random_normal.h"
#include "./initializers/random_xavier.h"
#include "./initializers/random_heuniform.h"

Model model_alloc(int layerCount) {
	Model m;
	m.layer_count = 0;
	m.layers = ALEMDAR_MALLOC(sizeof(*m.layers) * layerCount);
	return m;
}
void model_add(Model *model, Layer layer) {
	if(model->layer_count != 0) {
		if(layer.inputSize == 0) {
			layer.inputSize = model->layers[model->layer_count - 1].outputSize;
			vec_realloc(&layer.input, layer.inputSize);
		}
		if(layer.outputSize == 0) {
			layer.outputSize = layer.inputSize;
			vec_realloc(&layer.output, layer.outputSize);
		}
	}

	model->layers[model->layer_count] = layer;
	model->layer_count++;
}
void model_randomize(ModelInitializers initializer, Model model, ...) {
	void (*initializerFunction)(Model model, va_list args);
	switch (initializer)
	{
	case Zeros:
		initializerFunction = model_initializer_zeros;
		break;

	case Ones:
		initializerFunction = model_initializer_ones;
		break;
	
	case RandomUniform:
		initializerFunction = model_initializer_random_uniform;
		break;

	case RandomNormal:
		initializerFunction = model_initializer_random_normal;
		break;

	case RandomXavier:
		initializerFunction = model_initializer_random_xavier;
		break;

	case RandomHeUniform:
		initializerFunction = model_initializer_random_heuniform;
		break;
	}
	va_list args;
	va_start(args, model);
	initializerFunction(model, args);
	va_end(args);
}
void model_forward(Model model) {
	model.layers[0].forward(model.layers[0]);
	for (size_t i = 1; i < model.layer_count; i++)
	{
		model.layers[i].input = model.layers[i - 1].output;
		model.layers[i].forward(model.layers[i]);
	}
}
void model_input(Model model, float input[]) {
	for (size_t i = 0; i < model.layers[0].inputSize; i++)
	{
		model.layers[0].input.data[i] = input[i];
	}
}
float model_cost(Model model, float input[], float output[]) {
	model_input(model, input);
	model_forward(model);
	float c = 0;
	float d = 0;
	size_t n = model.layers[model.layer_count - 1].outputSize;
	for (size_t i = 0; i < n; i++)
	{
		d = (output[i] - model.layers[model.layer_count - 1].output.data[i]);
		c += d * d;
	}
	return c / n;
}
void model_learn(ModelOptimizers optimizer, Model model, size_t epoch, Matrix inputs, Matrix outputs, ...) {
	ALEMDAR_ASSERT(inputs.cols == model.layers[0].inputSize);
	ALEMDAR_ASSERT(outputs.cols == model.layers[model.layer_count - 1].outputSize);
	void (*optimizerFunction)(Model model, Matrix inputs, Matrix outputs, va_list args);
	va_list args;
	va_list cpyargs;
	va_start(args, outputs);
	switch (optimizer)
	{
		case FiniteDiff:
			optimizerFunction = model_learn_finite_diff;
			break;

		case BatchGradientDescent:
			optimizerFunction = model_learn_batch_gradient_descent;
			break;
	}
	clock_t timer = clock();
	for (size_t e = 0; e < epoch; e++)
	{
        print_progress_header(e,epoch, model_cost(model, inputs.data[0], outputs.data[0]));
		va_copy(cpyargs, args);
		optimizerFunction(model, inputs, outputs, cpyargs);
		va_end(cpyargs);
	}
	timer = clock() - timer;
	print_progress_footer(timer, model_cost(model, inputs.data[0], outputs.data[0]));
	va_end(args);
	printf("\n");
}
void model_print(Model model) {
	printf("%-20s%-20s%-20s\n", "Layer", "Shape", "Param");
	printf("=================================================\n");
	for (size_t i = 0; i < model.layer_count; i++)
	{
		layer_print(model.layers[i]);
	}
	printf("=================================================\n");
	/* printf("Input Values [ ");
	vec_print(model.layers[0].input);
	printf("]\nOutput Values [ ");
	vec_print(model.layers[model.layer_count - 1].output);
	printf("]\n"); */
}
void model_free(Model model) {
	for (size_t i = 0; i < model.layer_count; i++)
	{
		layer_free(model.layers[i]);
	}
	ALEMDAR_FREE(model.layers);
	
}

#endif // !ALEMDAR_MODEL_H