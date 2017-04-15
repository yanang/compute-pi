#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "computepi.h"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0
#define M_PI acos(-1.0)

int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    if (argc < 2) return -1;

    int N = atoi(argv[1]);
    int i, loop = 500;
    double pi, diff, error;
    // Baseline
    clock_gettime(CLOCK_ID, &start);
    for (i = 0; i < loop; i++) {
        pi=compute_pi_baseline(N);
    }
    clock_gettime(CLOCK_ID, &end);
    diff = pi - M_PI > 0 ? pi - M_PI : M_PI -pi;
    error = diff / M_PI;
    printf("%lf,",error);
    //printf("%lf,", (double) (end.tv_sec - start.tv_sec) +
    //       (end.tv_nsec - start.tv_nsec)/ONE_SEC);

    // OpenMP with 2 threads
    clock_gettime(CLOCK_ID, &start);
    for (i = 0; i < loop; i++) {
        pi=compute_pi_openmp(N, 2);
    }
    clock_gettime(CLOCK_ID, &end);
    diff = pi - M_PI > 0 ? pi - M_PI : M_PI -pi;
    error = diff / M_PI;
    printf("%lf,",error);
    //printf("%lf,", (double) (end.tv_sec - start.tv_sec) +
    //       (end.tv_nsec - start.tv_nsec)/ONE_SEC);

    // OpenMP with 4 threads
    clock_gettime(CLOCK_ID, &start);
    for (i = 0; i < loop; i++) {
        pi=compute_pi_openmp(N, 4);
    }
    clock_gettime(CLOCK_ID, &end);
    diff = pi - M_PI > 0 ? pi - M_PI : M_PI -pi;
    error = diff / M_PI;
    printf("%lf,",error);
    //printf("%lf,", (double) (end.tv_sec - start.tv_sec) +
    //       (end.tv_nsec - start.tv_nsec)/ONE_SEC);

    // AVX SIMD
    clock_gettime(CLOCK_ID, &start);
    for (i = 0; i < loop; i++) {
        pi=compute_pi_avx(N);
    }
    clock_gettime(CLOCK_ID, &end);
    diff = pi - M_PI > 0 ? pi - M_PI : M_PI -pi;
    error = diff / M_PI;
    printf("%lf,",error);
    //printf("%lf,", (double) (end.tv_sec - start.tv_sec) +
    //       (end.tv_nsec - start.tv_nsec)/ONE_SEC);

    // AVX SIMD + Loop unrolling
    clock_gettime(CLOCK_ID, &start);
    for (i = 0; i < loop; i++) {
        pi = compute_pi_avx_unroll(N);
    }
    clock_gettime(CLOCK_ID, &end);
    diff = pi - M_PI > 0 ? pi - M_PI : M_PI -pi;
    error = diff / M_PI;
    printf("%lf\n",error);
    //printf("%lf\n", (double) (end.tv_sec - start.tv_sec) +
    //       (end.tv_nsec - start.tv_nsec)/ONE_SEC);

    return 0;
}
