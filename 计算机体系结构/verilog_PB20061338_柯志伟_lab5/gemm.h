#ifndef     __H__
#define     __H__

#include<stdlib.h>
#include<stdio.h>
#include<time.h>


extern int N;

void gemm_baseline(float *A,float *B,float *C);
void gemm_verify(float *A,float *B,float *C);

void gemm_avx(float* A,float*B,float* C);

void gemm_avx_block(float *A,float* B,float* C);

#endif



