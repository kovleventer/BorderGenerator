#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/**
 * Contains functions about distance calculations
 */

int no_height(int startHeight, int destHeight);
int basic_height(int startHeight, int destHeight);
int delta_height(int startHeight, int destHeight);
int tanh_height(int startHeight, int destHeight);
int relu_height(int startHeight, int destHeight);
int elu_height(int startHeight, int destHeight);
int softplus_height(int startHeight, int destHeight);
int sqrt_height(int startHeight, int destHeight);
int sine_height(int startHeight, int destHeight);

#endif // FUNCTIONS_H
