#include <stdio.h>
#include <immintrin.h>
#include <omp.h>
#include "computepi.h"
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

cl_device_id device_id = NULL;
cl_context context = NULL;
cl_command_queue command_queue = NULL;
cl_mem memobj = NULL;
cl_program program = NULL;
cl_kernel kernel = NULL;
cl_platform_id platform_id = NULL;
size_t global_Worksize;
size_t local_Worksize = 1;
cl_uint ret_num_devices;
cl_uint ret_num_platforms;
cl_int ret;

double compute_pi_baseline(size_t N)
{
    double pi = 0.0;
    double dt = 1.0 / N;                // dt = (b-a)/N, b = 1, a = 0
    for (size_t i = 0; i < N; i++) {
        double x = (double) i / N;      // x = ti = a+(b-a)*i/N = i/N
        pi += dt / (1.0 + x * x);       // integrate 1/(1+x^2), i = 0....N
    }
    return pi * 4.0;
}

double compute_pi_openmp(size_t N, int threads)
{
    double pi = 0.0;
    double dt = 1.0 / N;
    double x;
    #pragma omp parallel num_threads(threads)
    {
        #pragma omp for private(x) reduction(+:pi)
        for (size_t i = 0; i < N; i++) {
            x = (double) i / N;
            pi += dt / (1.0 + x * x);
        }
    }
    return pi * 4.0;
}

double compute_pi_avx(size_t N)
{
    double pi = 0.0;
    double dt = 1.0 / N;
    register __m256d ymm0, ymm1, ymm2, ymm3, ymm4;
    ymm0 = _mm256_set1_pd(1.0);
    ymm1 = _mm256_set1_pd(dt);
    ymm2 = _mm256_set_pd(dt * 3, dt * 2, dt * 1, 0.0);
    ymm4 = _mm256_setzero_pd();             // sum of pi

    for (int i = 0; i <= N - 4; i += 4) {
        ymm3 = _mm256_set1_pd(i * dt);      // i*dt, i*dt, i*dt, i*dt
        ymm3 = _mm256_add_pd(ymm3, ymm2);   // x = i*dt+3*dt, i*dt+2*dt, i*dt+dt, i*dt+0.0
        ymm3 = _mm256_mul_pd(ymm3, ymm3);   // x^2 = (i*dt+3*dt)^2, (i*dt+2*dt)^2, ...
        ymm3 = _mm256_add_pd(ymm0, ymm3);   // 1+x^2 = 1+(i*dt+3*dt)^2, 1+(i*dt+2*dt)^2, ...
        ymm3 = _mm256_div_pd(ymm1, ymm3);   // dt/(1+x^2)
        ymm4 = _mm256_add_pd(ymm4, ymm3);   // pi += dt/(1+x^2)
    }
    double tmp[4] __attribute__((aligned(32)));
    _mm256_store_pd(tmp, ymm4);             // move packed float64 values to  256-bit aligned memory location
    pi += tmp[0] + tmp[1] + tmp[2] + tmp[3];
    return pi * 4.0;
}

double compute_pi_avx_unroll(size_t N)
{
    double pi = 0.0;
    double dt = 1.0 / N;
    register __m256d ymm0, ymm1, ymm2, ymm3, ymm4,
             ymm5, ymm6, ymm7, ymm8, ymm9,
             ymm10,ymm11, ymm12, ymm13, ymm14;
    ymm0 = _mm256_set1_pd(1.0);
    ymm1 = _mm256_set1_pd(dt);
    ymm2 = _mm256_set_pd(dt * 3, dt * 2, dt * 1, 0.0);
    ymm3 = _mm256_set_pd(dt * 7, dt * 6, dt * 5, dt * 4);
    ymm4 = _mm256_set_pd(dt * 11, dt * 10, dt * 9, dt * 8);
    ymm5 = _mm256_set_pd(dt * 15, dt * 14, dt * 13, dt * 12);
    ymm6 = _mm256_setzero_pd();             // first sum of pi
    ymm7 = _mm256_setzero_pd();             // second sum of pi
    ymm8 = _mm256_setzero_pd();             // third sum of pi
    ymm9 = _mm256_setzero_pd();             // fourth sum of pi

    for (int i = 0; i <= N - 16; i += 16) {
        ymm14 = _mm256_set1_pd(i * dt);

        ymm10 = _mm256_add_pd(ymm14, ymm2);
        ymm11 = _mm256_add_pd(ymm14, ymm3);
        ymm12 = _mm256_add_pd(ymm14, ymm4);
        ymm13 = _mm256_add_pd(ymm14, ymm5);

        ymm10 = _mm256_mul_pd(ymm10, ymm10);
        ymm11 = _mm256_mul_pd(ymm11, ymm11);
        ymm12 = _mm256_mul_pd(ymm12, ymm12);
        ymm13 = _mm256_mul_pd(ymm13, ymm13);

        ymm10 = _mm256_add_pd(ymm0, ymm10);
        ymm11 = _mm256_add_pd(ymm0, ymm11);
        ymm12 = _mm256_add_pd(ymm0, ymm12);
        ymm13 = _mm256_add_pd(ymm0, ymm13);

        ymm10 = _mm256_div_pd(ymm1, ymm10);
        ymm11 = _mm256_div_pd(ymm1, ymm11);
        ymm12 = _mm256_div_pd(ymm1, ymm12);
        ymm13 = _mm256_div_pd(ymm1, ymm13);

        ymm6 = _mm256_add_pd(ymm6, ymm10);
        ymm7 = _mm256_add_pd(ymm7, ymm11);
        ymm8 = _mm256_add_pd(ymm8, ymm12);
        ymm9 = _mm256_add_pd(ymm9, ymm13);
    }

    double tmp1[4] __attribute__((aligned(32)));
    double tmp2[4] __attribute__((aligned(32)));
    double tmp3[4] __attribute__((aligned(32)));
    double tmp4[4] __attribute__((aligned(32)));

    _mm256_store_pd(tmp1, ymm6);
    _mm256_store_pd(tmp2, ymm7);
    _mm256_store_pd(tmp3, ymm8);
    _mm256_store_pd(tmp4, ymm9);

    pi += tmp1[0] + tmp1[1] + tmp1[2] + tmp1[3] +
          tmp2[0] + tmp2[1] + tmp2[2] + tmp2[3] +
          tmp3[0] + tmp3[1] + tmp3[2] + tmp3[3] +
          tmp4[0] + tmp4[1] + tmp4[2] + tmp4[3];
    return pi * 4.0;
}
void init_opencl(size_t N)
{
    FILE *fp;
    char fileName[] = "./kernel.cl";
    char *source_str;
    size_t source_size;
    global_Worksize = N;

    /* Load the source code containing the kernel*/
    fp = fopen(fileName, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    /* Get Platform and Device Info */
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    /* Create OpenCL context */
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    /* Create Command Queue
     * clCreateCommandQueue() was deprecated as of OpenCL 2.0
     *     ,and replaced with clCreateCommandQueueWithProperties in OpenCl 3.0 */
    command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);

    /* Create Memory Buffer */
    memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(double), NULL, &ret);

    /* Create Kernel Program from the source */
    program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size, &ret);

    /* Build Kernel Program */
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    /* Create OpenCL Kernel */
    kernel = clCreateKernel(program, "compute_pi", &ret);

    /* Set OpenCL Kernel Parameters */
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memobj);
    ret = clSetKernelArg(kernel, 1, sizeof(uint), &N);
}
void free_opencl()
{
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(memobj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

}
double compute_pi_opencl(size_t N)
{
    double *pii = (double *)malloc(sizeof(double)*MEM_SIZE);

    //* Execute OpenCL Kernel */
    ret =  clEnqueueNDRangeKernel (command_queue, kernel, 1, NULL, &global_Worksize, &local_Worksize, 0, NULL, NULL);

    /* Copy results from the memory buffer */
    ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, MEM_SIZE * sizeof(double),pii, 0, NULL, NULL);

    return pii[0];
}
