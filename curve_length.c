#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

#include "accel_algo.h"

typedef struct {
    const float* mat;
    int starting_row;
    int n_rows_compute;
    int ncol;
    float* out;
} thread_data_t;

float curve_length(const float *X, const int len_X) {

    float cl = 0.0;

    for (size_t i = 0; i < len_X-1; i++)
    {   cl += sqrtf( 1 + (X[i+1] - X[i])*(X[i+1] - X[i]) );  }
    
    return cl;
}

static void* thread_curve_lengths(void* thread_arg) {

    thread_data_t *ta = (thread_data_t*) thread_arg;
    const float* compute_start;

    for (size_t i = ta->starting_row; i < ta->starting_row + ta->n_rows_compute; i++)
    {       
        compute_start = ta->mat + (i * ta->ncol);
        ta->out[i] = curve_length(compute_start, ta->ncol);   
    }

    return NULL;
}

float* curve_length_mt(const float *X, const int nrow, const int ncol, const int n_threads) {

    pthread_t threads[n_threads];
    thread_data_t thread_data[n_threads];
    float* result = (float*) malloc( sizeof(float) * nrow );

    int n_rows_per_thread = nrow / n_threads;
    int n_remaining_rows = nrow % n_threads;
    int starting_row = 0;

    for (size_t i = 0; i < n_threads; i++)
    {   
        thread_data[i].starting_row = starting_row;
        thread_data[i].mat = X;
        thread_data[i].ncol = ncol;
        thread_data[i].out = result;
        
        if (i < n_remaining_rows)
        {
            thread_data[i].n_rows_compute = n_rows_per_thread + 1;
            starting_row += n_rows_per_thread + 1;
        } else {
            thread_data[i].n_rows_compute = n_rows_per_thread;
            starting_row += n_rows_per_thread;
        }

        pthread_create(&threads[i], NULL, &thread_curve_lengths, (void*) &thread_data[i]);
    }

    for (size_t i = 0; i < n_threads; i++)
    {   pthread_join(threads[i], NULL);     }

    return result;
}
