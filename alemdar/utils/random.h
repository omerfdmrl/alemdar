#ifndef ALEMDAR_RANDOM_H

#define ALEMDAR_RANDOM_H

ALEMDAR_DEF float random_uniform(float low,float high);
ALEMDAR_DEF float random_normal(float mean, float stddev);
ALEMDAR_DEF float random_xavier(float fan_in, float fan_out);
ALEMDAR_DEF float random_xavier_sqrt(float fan_in, float fan_out);
ALEMDAR_DEF float random_xavier_rand(float a);
ALEMDAR_DEF float random_heuniform(float fan_in, float fan_out);

float random_uniform(float low,float high) {
	return ((float) rand() / (float) RAND_MAX) * (high - low) + low;
}
float random_normal(float mean, float stddev) {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;

    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
    
    return mean + z0 * stddev;
}
float random_xavier(float fan_in, float fan_out) {
    float a = sqrt(6.0 / (fan_in + fan_out));
    return ((float)rand() / RAND_MAX) * 2.0 * a - a;
}
float random_xavier_sqrt(float fan_in, float fan_out) {
    float a = sqrt(6.0 / (fan_in + fan_out));
    return a;
}
float random_xavier_rand(float a) {
    return ((float)rand() / RAND_MAX) * 2.0 * a - a;
}
float random_heuniform(float fan_in, float fan_out) {
    float limit = sqrt(6.0 / (fan_in + fan_out));
    return ((float)rand() / RAND_MAX) * 2.0 * limit - limit;
}

#endif // !ALEMDAR_RANDOM_H
