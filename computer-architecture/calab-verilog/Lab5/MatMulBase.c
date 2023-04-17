#include "./gemm.h"

//matmul scale : N*N

/*      matmulbase  */

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
    gemm_baseline(matrix_A,matrix_B,matrix_C);
    time_end = clock();

    printf("total time:%f\n",time_end-time_start);
    system("pause");
     
}

