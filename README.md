# Accelerated Algorithms

The following is a collection of accelerated algorithms that I use in my work. These implementations are highly optimized and do not contain any checks. As such, please
use with caution.

<br/>

Algorithms included in this repository thus far:
1) [Approximate Entropy](#approximate-entropy)

---

## Approximate Entropy

An excellent description of the *approximate entropy* algorithm can be found [here](https://en.wikipedia.org/wiki/Approximate_entropy#The_interpretation). The function definition of the algorithm is titled *apentropy*, and can be found in `apentropy.c`. When compiling source code that uses *apentropy*, the `-lm` compiler flag must be included so that the `math.h` library is linked. As an example:
> `gcc apentropy.c -o test.exe -lm`

**Performance Metrics**
* Takes *~0.5-1 second* for a 10,000 element vector, m = 2, and r = 3.
* About 300-500 times faster than the Wikipedia Python implementation.

---
