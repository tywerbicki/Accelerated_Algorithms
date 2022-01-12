#include <stdio.h>
#include <stdlib.h>

#include "cardio_algo.h"

int main(void) {

    // Test apentropy.

    const int len = 51;
    double *vec = (double*) malloc( sizeof(double)*len );

    // Example from wikipedia.
    for (size_t i = 0; i < len; i++)
    {   
        if (i % 3 == 0)
        {   vec[i] = 85;    } 
        else {
            if (i % 3 == 1)
            {   vec[i] = 80;    }
            else {
                vec[i] = 89;
            }
        } 
    }
    
    double result = apentropy(vec, len, 2, 3);
    free(vec);

    printf("ApEn: %f \n", result);


    // Test curve length.

    int nrow = 50, ncol = 5000;

    float* single = (float*) malloc( sizeof(float) * ncol );
    float* matrix = (float*) malloc( sizeof(float) * nrow * ncol );

    for (size_t i = 0; i < ncol; i++)
    {   single[i] = (float)(i+1);   }

    for (size_t i = 0; i < nrow; i++)
    {
        for (size_t j = 0; j < ncol; j++)
        {   *(matrix + (i*ncol) + j) = (float)(j+1);    } 
    }

    float single_result = curve_length(single, ncol);
    float* matrix_result = curve_length_mt(matrix, nrow, ncol, 4);
    free(single);
    free(matrix);

    printf("Single curve length: %f \n", single_result);
    printf("Matrix curve lengths: \n");
    for (size_t i = 0; i < nrow; i++)
    {   printf("%f ", matrix_result[i]);    }
    printf("\n");
    
}