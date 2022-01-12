#ifndef APENTROPY_H
#define APENTROPY_H

double apentropy(const double *U, const int len_U, const int m, const int r);

#endif

#ifndef CURVE_LENGTH_H
#define CURVE_LENGTH_H
 
float curve_length(const float *X, const int len_X);
float* curve_length_mt(const float *X, const int nrow, const int ncol, const int n_threads);  

#endif
