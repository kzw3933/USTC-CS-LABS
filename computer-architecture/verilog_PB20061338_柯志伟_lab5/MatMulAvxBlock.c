#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<immintrin.h>

#include ".\gemm.h"

int blocksize = 1024;
int N = (1<<10);


/*      matmulavxblock  */

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
    gemm_avx_block(matrix_A,matrix_B,matrix_C);
    time_end = clock();
    printf("total time:%f\n",time_end-time_start);

    // use gemm_baseline verify gemm_avx
    gemm_verify(matrix_A,matrix_B,matrix_C);
    system("pause");
    
}

static inline void do_block(int n, int si, int sj, int sk, float* matrix_A,float*matrix_B,float* matrix_C){
    
    __m256 vecA ,vecB, vecC;

    for (int j = sj; j < sj + blocksize; j++){
	    for (int i = si; i < si + blocksize; i += 8) {
				vecC = _mm256_loadu_ps(matrix_C+i+j*n);
			for (int k = sk; k < sk + blocksize; k++) {
				vecA = _mm256_broadcast_ss(matrix_A+k+j*n);
                vecB = _mm256_loadu_ps(matrix_B+n*k+i);
                vecC = _mm256_fmadd_ps(vecA,vecB,vecC);
			}
				_mm256_storeu_ps(matrix_C+i+j*n, vecC);
		}
	}
}

void gemm_avx_block(float* matrix_A,float*matrix_B,float* matrix_C)
{
	for (int sj = 0; sj < N; sj += blocksize) {
		for (int si = 0; si < N; si += blocksize) {
			for (int sk = 0; sk < N; sk += blocksize) {
				do_block(N, si, sj, sk,matrix_A,matrix_B, matrix_C);
			}
		}
	}
}