﻿#include <stdlib.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <time.h>
#include <stdio.h>

#define N (1 << 10)
#define blocksize 128
#define gridsize N/blocksize


__global__ void matrixs_GPU(float* matrix_A_dev, float* matrix_B_dev, float* matrix_C_dev);
void print(float* matrix_A);


int main() {

   float* matrix_A = (float*)malloc(sizeof(float) * N * N);
   float* matrix_B = (float*)malloc(sizeof(float) * N * N);
   float* matrix_C = (float*)malloc(sizeof(float) * N * N);

   srand(time(0));

   // random initialize A,B and initialize C with 0
   for (int i = 0; i < N * N; i++) {
       matrix_A[i] = 150 + rand() % 120;
       matrix_B[i] = 150 + rand() % 120;
       matrix_C[i] = 0;
   }

   float* matrix_A_dev;
   float* matrix_B_dev;
   float* matrix_C_dev;

   cudaMalloc((void**)&matrix_A_dev, N * N * sizeof(float));
   cudaMalloc((void**)&matrix_B_dev, N * N * sizeof(float));
   cudaMalloc((void**)&matrix_C_dev, N * N * sizeof(float));
   cudaMemcpy(matrix_A_dev, matrix_A, N * N * sizeof(float), cudaMemcpyHostToDevice);
   cudaMemcpy(matrix_B_dev, matrix_B, N * N * sizeof(float), cudaMemcpyHostToDevice);


   dim3 block(blocksize, blocksize);
   dim3 grid((N + block.x - 1) / block.x, (N + block.y - 1) / block.y);

   matrixs_GPU << < grid, block >> > (matrix_A_dev, matrix_B_dev, matrix_C_dev);
   cudaMemcpy(matrix_C, matrix_C_dev, N * N * sizeof(float), cudaMemcpyDeviceToHost);

    /*print(matrix_C);
   gemm_verify(matrix_A, matrix_B, matrix_C);
   print(matrix_C); */

   free(matrix_A);
   free(matrix_B);
   free(matrix_C);
   cudaFree(matrix_A_dev);
   cudaFree(matrix_B_dev);
   cudaFree(matrix_C_dev);
}

__global__ void matrixs_GPU(float* matrix_A_dev, float* matrix_B_dev, float* matrix_C_dev) {
   int row = threadIdx.y + blockDim.y * blockIdx.y;
   int col = threadIdx.x + blockDim.x * blockIdx.x;

   if (row < N && col < N) {
       matrix_C_dev[row * N + col] = 0;
       for (int k = 0; k < N; k++) {
           matrix_C_dev[row * N + col] += matrix_A_dev[row * N + k] * matrix_B_dev[k * N + col];
       }
   }
}


void print(float* matrix_A) {
   int i, j;
   for (i = 0; i < N; i++) {
       for (j = 0; j < N; j++) {
           printf("%f ", matrix_A[i * N + j]);
       }
       printf("\n");
   }
}


