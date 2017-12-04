#include "functions.h"

#include <math.h>

int no_height(int startHeight, int destHeight) {
	return 1;
}

int basic_height(int startHeight, int destHeight) {
	return destHeight;
}

int delta_height(int startHeight, int destHeight) {
	return destHeight - startHeight + 10;
}

int tanh_height(int startHeight, int destHeight) {
	return (tanh(destHeight - startHeight) + 1) * 10000;
}

int relu_height(int startHeight, int destHeight) {
	return destHeight - startHeight < 0 ? 0 : destHeight - startHeight;
}
