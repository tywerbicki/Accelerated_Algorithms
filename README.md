# Accelerated Algorithms

The following is a collection of accelerated algorithms that I use in my work. These implementations are highly optimized and do not contain any checks. As such, please
use with caution.

<br/>

Modules included in this repository thus far:
* Cardiovascular Algorithms 
    1) [Approximate Entropy](#approximate-entropy)
    2) [Curve Length](#curve-length)
    
<br/>

How to compile `debug*.c` source code files to ensure that the libraries are working:
* *cardio_debug.c*
    
    > `gcc -Wall cardio_debug.c ./cardio_algo_src/* -o test.exe -lm -lpthread`
         
<br/>
<br/>

**All module header files (1 per module) are located in `./incude/`.** 

---

## Approximate Entropy

An excellent description of the *approximate entropy* algorithm can be found [here](https://en.wikipedia.org/wiki/Approximate_entropy#The_interpretation). The function definition of the algorithm is titled *apentropy*, and can be found in `./cardio_algo_src/apentropy.c`. When compiling source code that uses *apentropy*, the `-lm` compiler flag must be included so that the `math.h` library is linked.

**Performance Metrics**
* Takes *~0.5-1 second* for a 10,000 element vector, m = 2, and r = 3.
* About 300-500 times faster than the Wikipedia Python implementation.

---

## Curve Length

A definition of the *curve length* algorithm can be found [here](https://lcp.mit.edu/pdf/Zong06.pdf). The function definition of the algorithm is titled *curve_length*, and can be found in `./cardio_algo_src/curve_length.c`. There is also a multithreaded version of *curve_length* titled *curve_length_mt* that is in the same source file. This version expects an `m` by `n` matrix instead of a vector, where `m` (number of rows) is the number of time series and `n` (number of columns) is the length of each time series. This algorithm parallelizes the computation of *curve_length* on different time series. When compiling source code that uses *curve_length_mt*, the `-lm` and `-lpthread` compiler flags must be included so that the `math.h` and `pthread.h` libraries are linked.

**Performance Metrics**

---
