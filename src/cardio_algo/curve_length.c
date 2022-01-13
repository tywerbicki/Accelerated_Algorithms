#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

#include "../../include/cardio_algo.h"

// Holds data that must be sent to each thread.
typedef struct {
    // Location of input matrix.
    const float* mat;
    // Row that the thread should start computations on.
    int starting_row;
    // Number of curve lengths to compute.
    int n_rows_compute;
    // Number of columns in the input matrix.
    int ncol;
    // Location of the output matrix.
    float* out;
} thread_data_t;

float curve_length(const float *X, const int len_X) {
    /*
    Calculate the curve length of the vector X.
    */

    float cl = 0.0;

    for (size_t i = 0; i < len_X-1; i++)
    {   cl += sqrtf( 1 + (X[i+1] - X[i])*(X[i+1] - X[i]) );  }
    
    return cl;
}

static void* thread_curve_lengths(void* thread_arg) {
    /*
    This is a worker function for 'curve_length_mt`.
    Calculate 'n_rows_compute' curve lengths and store them in ta->out
    (the output matrix that is returned by 'curve_length_mt').
    */

    // Get data for this thread to use.
    thread_data_t *ta = (thread_data_t*) thread_arg;
    // The location of the input matrix that this thread should start at.
    const float* compute_start;

    for (size_t i = ta->starting_row; i < ta->starting_row + ta->n_rows_compute; i++)
    {   
        // Point to the first element in the i'th row.    
        compute_start = ta->mat + (i * ta->ncol);
        // Store the curve length of the i'th row in ta->out.
        ta->out[i] = curve_length(compute_start, ta->ncol);   
    }

    // For brevity.
    return NULL;
}

float* curve_length_mt(const float *X, const int nrow, const int ncol, const int n_threads) {

    // Create array of threads.
    pthread_t threads[n_threads];
    // Create array of data that will be sent to the threads.
    thread_data_t thread_data[n_threads];
    // Allocate memory for the output vector.
    float* result = (float*) malloc( sizeof(float) * nrow );

    // Minimum number of rows of X that each thread will compute.
    int n_rows_per_thread = nrow / n_threads;
    // Number of leftover rows.
    int n_remaining_rows = nrow % n_threads;
    // Row at which the thread should start at.
    int starting_row = 0;

    for (size_t i = 0; i < n_threads; i++)
    {   
        thread_data[i].starting_row = starting_row;
        // Pass input matrix.
        thread_data[i].mat = X;
        // Pass number of columns of input matrix.
        thread_data[i].ncol = ncol;
        // Pass location of output vector.
        thread_data[i].out = result;
        
        // Allocate remaining rows 1 by 1 to threads.
        if (i < n_remaining_rows)
        {
            thread_data[i].n_rows_compute = n_rows_per_thread + 1;
            starting_row += n_rows_per_thread + 1;
        } else {
            thread_data[i].n_rows_compute = n_rows_per_thread;
            starting_row += n_rows_per_thread;
        }

        // Spawn i'th thread.
        pthread_create(&threads[i], NULL, &thread_curve_lengths, (void*) &thread_data[i]);
    }

    for (size_t i = 0; i < n_threads; i++)
    {   pthread_join(threads[i], NULL);     }

    return result;
}
