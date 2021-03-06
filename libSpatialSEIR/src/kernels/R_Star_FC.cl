#pragma OPENCL EXTENSION cl_khr_fp64 : enable

inline double logFactorial(int n)
{
    if (n != 0)
    {
        return(0.5*log(6.283185*n) + n*log(1.0*n) - n);
    }
    return(0);
}

inline double logChoose(int n, int x)
{
    if ((x !=0) && (n != 0))
    {
        return(logFactorial(n) - logFactorial(x) - logFactorial(n-x)); 
    }
    return(0.0);
}

inline double dbinom(int x, int n, double p)
{
    if (n != 0 && p != 1.0 && p != 0.0)
    {
        return(logChoose(n,x) + x*log(p) + (n-x)*log(1-p));
    }
    else if (n == 0)
    {
        return(x==0 ? 0.0 : -INFINITY);
    }
    else if (p == 0.0)
    {
        return(x == 0 ? 0.0 : -INFINITY);
    }
    else if (p == 1.0)
    {
        return(x==n ? 0.0 : -INFINITY);
    }   
    return(-INFINITY);
}

__kernel void FC_R_Star_Part1(int nTpt,
                              int nLoc,
                              __global int* S_star,
                              __global int* E_star,
                              __global int* R_star,
                              __global int* S,
                              __global int* I,
                              __global int* R,
                              __global double* p_se,
                              __global double* p_rs,
                              __global double* p_ir,
                              __global double* output,
                              __local int* S_star_loc,
                              __local int* E_star_loc,
                              __local int* R_star_loc,
                              __local int* S_loc,
                              __local int* I_loc,
                              __local int* R_loc,
                              __local double* p_se_loc,
                              __local double* p_rs_loc,
                              __local double* p_ir_loc)
{
    size_t globalId = get_global_id(0);
    int i;
    int totalSize = nLoc*nTpt;
    size_t localId = get_local_id(0);
    size_t localSize = get_local_size(0);
    size_t groupId = get_group_id(0); 
    double partialResult = 0.0;

    if (globalId < totalSize)
    {
        S_star_loc[localId] = S_star[globalId];
        E_star_loc[localId] = E_star[globalId];
        R_star_loc[localId] = R_star[globalId];
        S_loc[localId] = S[globalId];
        I_loc[localId] = I[globalId];
        R_loc[localId] = R[globalId];
        p_se_loc[localId] = p_se[globalId];
        p_rs_loc[localId] = p_rs[globalId % nTpt];
        p_ir_loc[localId] = p_ir[globalId % nTpt];


        if ((R_star_loc[localId] >= 0) && (R_star_loc[localId] <= I_loc[localId]) && 
                      (S_star_loc[localId] <= R_loc[localId]))
        {
            partialResult = ((logChoose(I_loc[localId], R_star_loc[localId]) 
                              + R_star_loc[localId]*log(p_ir_loc[localId])
                              + (I_loc[localId] - R_star_loc[localId])*log(1-p_ir_loc[localId])) 
                              + dbinom(S_star_loc[localId], R_loc[localId], p_rs_loc[localId]) 
                              + dbinom(E_star_loc[localId], S_loc[localId], p_se_loc[localId]) 
                             );
        }
        else
        {     
            partialResult = -INFINITY;
        }
    }
    p_rs_loc[localId] = partialResult;

    barrier(CLK_LOCAL_MEM_FENCE);    
    for (i = localSize/2; i > 0; i >>= 1)
    {
        if (localId < i)
        {
            p_rs_loc[localId] += p_rs_loc[localId + i]; 
        } 
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (localId == 0)
    {
        output[groupId] = p_rs_loc[localId];
    }
}
