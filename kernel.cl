__kernel void compute_pi(__global double *pii, const uint N)
{
    double pi = 0.0;
    double dt = 1.0 / N;
    for (size_t i = 0; i < N; i++) {
        double x = (double) i / N;
        pi += dt / (1.0 + x * x);
    }
    pii[0] = 4 * pi;
}
