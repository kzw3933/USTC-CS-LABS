#include ".\gemm.h"


void gemm_baseline(float *matrix_A,float *matrix_B,float *matrix_C){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(int k=0;k<N;k++){
                matrix_C[i*N+j] += matrix_A[i*N+k]*matrix_B[k*N+j]; 
            }
        }
    }
}

// use gemm_baseline verify gemm_avx
void gemm_verify(float *matrix_A,float *matrix_B,float *matrix_C){
    float * matrix_D = (float*)malloc(sizeof(float)*N*N);
    gemm_baseline(matrix_A,matrix_B,matrix_D);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(matrix_D[i*N+j]!=matrix_C[i*N+j]){
                printf("Error!!\n");
                return ;
            }
        }
    }
}