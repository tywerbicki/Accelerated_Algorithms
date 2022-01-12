#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "../cardio_algo.h"

static const double DOUBLE_MIN = DBL_MIN;

typedef struct {   
    double m, m1;
    } distances_t;

static inline distances_t* d_mm1(const double *x_i, const double *x_j, const int l) {
    /*
    Find distances between x_i and x_j for both m and m+1 lengths.
    Doing this simultaneously prevents any double calculation and
    therefore drastically increases the speed of the algorithm.
    Parameter 'l' should be set to m+1.
    */

    distances_t *max_dists = (distances_t*) malloc( sizeof(distances_t) );
    max_dists->m = DOUBLE_MIN;
    double dist;
    
    for (size_t i = 0; i < l - 1; i++)
    {   
        dist = fabs( x_i[i] - x_j[i] );
        if (dist > max_dists->m)
        {   max_dists->m = dist;    }
    }
    dist = fabs( x_i[l-1] - x_j[l-1] );
    if (dist < max_dists->m) {
        max_dists->m1 = max_dists->m;
    } else {
        max_dists->m1 = dist;
    }

    return max_dists; 
}

static inline double d(const double *x_i, const double *x_j, const int l) {
    /*
    Find distances between x_i and x_j for m legnth only.
    Parameter 'l' should be set to m.
    */

    double dist, max_dist = DOUBLE_MIN;

    for (size_t i = 0; i < l; i++)
    {   
        dist = fabs( x_i[i] - x_j[i] );
        if (dist > max_dist)
        {   max_dist = dist;    }
    }

    return max_dist; 
}

double apentropy(const double *U, const int len_U, const int m, const int r) {
    /*
    Calculates the approximate entropy of the vector U given hyperparameters
    m and r. 
    */

    // Number of vectors to be compared for m+1, so m will require
    // one additional comparison. 
    const int NUM_BIG_VEC = (len_U - m);
    // Store the number of vector distances <= r for size m.
    double *distance_vec_m = (double*) calloc(sizeof(double), NUM_BIG_VEC + 1);
    // Store the number of vector distances <= r for size m+1.
    double *distance_vec_m1 = (double*) calloc(sizeof(double), NUM_BIG_VEC);
    // Initialize.
    distances_t *dists;
    const double *x_i, *x_j;
    size_t j;

    for (size_t i = 0; i < NUM_BIG_VEC; i++)
    {   
        // First vector for distance calculation.
        x_i = U + i;
        // Distance between vector and itself is 0 < r.
        distance_vec_m[i] += 1.0;
        distance_vec_m1[i] += 1.0;
        for (j = i+1; j < NUM_BIG_VEC; j++)
        {   
            // Second vector for distance calculation.
            x_j = U + j;
            // Find distances between x_i and x_j for both m and m+1 lengths.
            dists = d_mm1(x_i, x_j, m+1);
            // If m-length distance <= r, record it.
            if ( dists->m <= r ) 
            {
                distance_vec_m[i] += 1.0;
                distance_vec_m[j] += 1.0;
            }
            // If m+1-length distance <= r, record it.  
            if ( dists->m1 <= r ) 
            {
                distance_vec_m1[i] += 1.0;
                distance_vec_m1[j] += 1.0;
            } 
            // 'dists' points to dynamically allocated memory. 
            free(dists);
        }
        // Now have to consider the last m-length vector for the given i.
        j = NUM_BIG_VEC;
        x_j = U + j;
        if ( d(x_i, x_j, m) <= r )
        {
            distance_vec_m[i] += 1.0;
            distance_vec_m[j] += 1.0;
        }
    }
    // Distance between last m-length vector and itself is 0 < r.
    distance_vec_m[NUM_BIG_VEC] += 1;
    
    // Calculate phi_m and phi_m+1.
    double phi_m = 0.0, phi_m1 = 0.0;
    // Number of m-length vectors.
    const double N_M = (double)(NUM_BIG_VEC + 1);
    // Number of m+1-length vectors.
    const double N_M1 = (double)(NUM_BIG_VEC);

    for (size_t i = 0; i < NUM_BIG_VEC; i++)
    {   
        phi_m += log( distance_vec_m[i] / N_M );
        phi_m1 += log( distance_vec_m1[i] / N_M1 );
    }
    phi_m += log( distance_vec_m[NUM_BIG_VEC] / N_M );
    phi_m /= N_M; phi_m1 /= N_M1;

    free(distance_vec_m);
    free(distance_vec_m1);

    return phi_m - phi_m1;
}