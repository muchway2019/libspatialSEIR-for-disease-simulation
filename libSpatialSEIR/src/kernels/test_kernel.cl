__kernel void test_kernel(__global float* a,
                          __global float* b,
                          __global float* output)
{
    *out = (*a)+(*b);
}
