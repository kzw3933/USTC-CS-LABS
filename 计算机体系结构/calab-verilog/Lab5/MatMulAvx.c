#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<immintrin.h>

#include ".\gemm.h"

/*      matmulavx  */
int N = (1<<10);

int main(void){

    
    float time_start,time_end;

    // malloc A,B,C
    float *matrix_A =(float*) malloc(sizeof(float)*N*N);
    float *matrix_B =(float*) malloc(sizeof(float)*N*N);
    float *matrix_C =(float*) malloc(sizeof(float)*N*N);

  
    srand(time(0));

    // random initialize A,B and initialize C with 0
    for(int i=0;i<N*N;i++){
        matrix_A[i] = 150+rand()%120;
        matrix_B[i] = 150+rand()%120;
        matrix_C[i] = 0; 
    }
    // measure time
    time_start = clock();
    
    gemm_avx(matrix_A,matrix_B,matrix_C);
    time_end = clock();
    printf("total time:%f\n",time_end-time_start);

    // use gemm_baseline verify gemm_avx
    gemm_verify(matrix_A,matrix_B,matrix_C);
    system("pause");
    
     
}

void gemm_avx(float* matrix_A,float*matrix_B,float* matrix_C) {

	__m256 vecA, vecB, vecC;
    
	for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
           vecA = _mm256_set1_ps(matrix_A[i*N+j]);
           for(int k=0;k<N;k+=8){
               vecB = _mm256_loadu_ps(&(matrix_B[j*N+k]));
               vecC = _mm256_loadu_ps(&(matrix_C[i*N+k]));
               vecC = _mm256_fmadd_ps(vecA, vecB, vecC);
               _mm256_storeu_ps(&(matrix_C[i*N+k]), vecC);
           }
        }
    }
}







