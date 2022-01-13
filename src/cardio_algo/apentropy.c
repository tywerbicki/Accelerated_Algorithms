#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "../include/cardio_algo.h"

static const double FLOAT_MIN = FLT_MIN;

// Holds maximum distances for lengths m and m+1.
typedef struct {   
    float m; 
    float m1;
    } distances_t;

static inline distances_t* d_mm1(const float *x_i, const float *x_j, const int l) {
    /*
    Find distances between x_i and x_j for both m and m+1 lengths.
    Doing this simultaneously prevents any double calculation and
    therefore drastically increases the speed of the algorithm.
    Parameter 'l' should be set to m+1.
    */

    distances_t *max_dists = (distances_t*) malloc( sizeof(distances_t) );
    max_dists->m = FLOAT_MIN;
    float dist;
    
    for (size_t i = 0; i < l - 1; i++)
    {   
        dist = fabsf( x_i[i] - x_j[i] );
        if (dist > max_dists->m)
        {   max_dists->m = dist;    }
    }
    dist = fabsf( x_i[l-1] - x_j[l-1] );
    if (dist < max_dists->m) {
        max_dists->m1 = max_dists->m;
    } else {
        max_dists->m1 = dist;
    }

    return max_dists; 
}

static inline float d(const float *x_i, const float *x_j, const int l) {
    /*
    Find distances between x_i and x_j for m legnth only.
    Parameter 'l' should be set to m.
    */

    float dist, max_dist = FLOAT_MIN;

    for (size_t i = 0; i < l; i++)
    {   
        dist = fabsf( x_i[i] - x_j[i] );
        if (dist > max_dist)
        {   max_dist = dist;    }
    }

    return max_dist; 
}

float apentropy(const float *U, const int len_U, const int m, const float r) {
    /*
    Calculates the approximate entropy of the vector U given hyperparameters
    m and r. 
    */

    // Number of vectors to be compared for m+1, so m will require
    // one additional comparison. 
    const int NUM_BIG_VEC = (len_U - m);
    // Store the number of vector distances <= r for size m.
    float *distance_vec_m = (float*) calloc(sizeof(float), NUM_BIG_VEC + 1);
    // Store the number of vector distances <= r for size m+1.
    float *distance_vec_m1 = (float*) calloc(sizeof(float), NUM_BIG_VEC);
    // Initialize.
    distances_t *dists;
    const float *x_i, *x_j;
    size_t j;

    for (size_t i = 0; i < NUM_BIG_VEC; i++)
    {   
        // First vector for distance calculation.
        x_i = U + i;
        // Distance between vector and itself is 0 < r.
        distance_vec_m[i] += 1.0F;
        distance_vec_m1[i] += 1.0F;
        for (j = i+1; j < NUM_BIG_VEC; j++)
        {   
            // Second vector for distance calculation.
            x_j = U + j;
            // Find distances between x_i and x_j for both m and m+1 lengths.
            dists = d_mm1(x_i, x_j, m+1);
            // If m-length distance <= r, record it.
            if ( dists->m <= r ) 
            {
                distance_vec_m[i] += 1.0F;
                distance_vec_m[j] += 1.0F;
            }
            // If m+1-length distance <= r, record it.  
            if ( dists->m1 <= r ) 
            {
                distance_vec_m1[i] += 1.0F;
                distance_vec_m1[j] += 1.0F;
            } 
            // 'dists' points to dynamically allocated memory. 
            free(dists);
        }
        // Now have to consider the last m-length vector for the given i.
        j = NUM_BIG_VEC;
        x_j = U + j;
        if ( d(x_i, x_j, m) <= r )
        {
            distance_vec_m[i] += 1.0F;
            distance_vec_m[j] += 1.0F;
        }
    }
    // Distance between last m-length vector and itself is 0 < r.
    distance_vec_m[NUM_BIG_VEC] += 1.0F;
    
    // Calculate phi_m and phi_m+1.
    float phi_m = 0.0, phi_m1 = 0.0;
    // Number of m-length vectors.
    const float N_M = (float)(NUM_BIG_VEC + 1);
    // Number of m+1-length vectors.
    const float N_M1 = (float)(NUM_BIG_VEC);

    for (size_t i = 0; i < NUM_BIG_VEC; i++)
    {   
        phi_m += logf( distance_vec_m[i] / N_M );
        phi_m1 += logf( distance_vec_m1[i] / N_M1 );
    }
    phi_m += logf( distance_vec_m[NUM_BIG_VEC] / N_M );
    phi_m /= N_M; phi_m1 /= N_M1;

    free(distance_vec_m);
    free(distance_vec_m1);

    return phi_m - phi_m1;
}