#ifndef ALEMDAR_LAYER_H

#define ALEMDAR_LAYER_H

typedef enum {
	Dense,
	RNN,
	GRU,
	Activation,
	Dropout,
	Shuffle,
	MaxPooling2D,
	MinPooling2D,
	MeanPooling2D,
	Flatten
} LayerNames;
static const char* LayerNamesChar[] = {
	[Dense] = "Dense",
	[RNN] = "Recurrent",
	[GRU] = "Gated Recurrent",
	[Activation] = "Activation",
	[Dropout] = "Dropout",
	[Shuffle] = "Shuffle",
	[MaxPooling2D] = "MaxPooling2D",
	[MinPooling2D] = "MinPooling2D",
	[MeanPooling2D] = "MeanPooling2D",
	[Flatten] = "Flatten"
};

typedef struct Layer Layer;

struct Layer {
	LayerNames name;
	size_t inputSize;
	size_t outputSize;
	Vec input;
	Vec output;
	Vec bias;
	Vec params;
	Matrix weight;
	void (*forward)(Layer layer);
	void (*backward)(Layer layer);
};

#include "./layers/dense.h"
#include "./layers/rnn.h"
#include "./layers/gru.h"
#include "./layers/activation.h"
#include "./layers/dropout.h"
#include "./layers/shuffle.h"
#include "./layers/max_pooling_2d.h"
#include "./layers/min_pooling_2d.h"
#include "./layers/mean_pooling_2d.h"
#include "./layers/flatten.h"

ALEMDAR_DEF void layer_print(Layer layer);
ALEMDAR_DEF void layer_free(Layer layer);
#define LAYER_NAME(name) LayerNamesChar[(name)]

void layer_print(Layer layer) {
	size_t param = layer.bias.size + layer.weight.cols * layer.weight.rows + layer.params.size;
	printf("%-20s(%zu, %zu)%-16s%zu\n", LAYER_NAME(layer.name), layer.inputSize, layer.outputSize,"", param);
}
void layer_free(Layer layer) {
	if(layer.bias.size > 0)
		vec_free(layer.bias);
	if(layer.weight.rows > 0)
		matrix_free(layer.weight);
	if(layer.params.size > 0)
		vec_free(layer.params);
}

#endif // !ALEMDAR_LAYER_H
