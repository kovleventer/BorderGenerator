#include "functions.h"

#include <math.h>

int no_height(int startHeight, int destHeight) {
	return 1;
}

int basic_height(int startHeight, int destHeight) {
	return destHeight;
}

int delta_height(int startHeight, int destHeight) {
	int d = destHeight - startHeight;
	return d + 10;
}

int tanh_height(int startHeight, int destHeight) {
	int d = destHeight - startHeight;
	return (tanh(d) + 1) * 10000;
}

int relu_height(int startHeight, int destHeight) {
	int d = destHeight - startHeight;
	return d < 0 ? 0 : d;
}

int elu_height(int startHeight, int destHeight) {
	int d = destHeight - startHeight;
	return d < 0 ? exp(d) : d + 1;
}

int softplus_height(int startHeight, int destHeight) {
	int d = destHeight - startHeight;
	return log(1 + exp(d)) * 1000;
}

int sqrt_height(int startHeight, int destHeight) {
	int d = destHeight - startHeight;
	return d / sqrt(1 + d * d) + 1;
}

int sine_height(int startHeight, int destHeight) {
	return (sin(destHeight) + 1) * 10000;
}
