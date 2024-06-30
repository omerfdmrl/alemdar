#ifndef ALEMDAR_ACTIVATION_H

#define ALEMDAR_ACTIVATION_H

ALEMDAR_DEF float sigmoid(float x);
ALEMDAR_DEF float dsigmoid(float x);
ALEMDAR_DEF float dtanh(float x);

float sigmoid(float x) {
	return 1.f / (1.f + expf(-x));
}
float dsigmoid(float x) {
	return x * (1 - x);
}
float dtanh(float x) {
	return 1.0 / (coshf(x) * coshf(x));
}

#endif // !ALEMDAR_ACTIVATION_H
