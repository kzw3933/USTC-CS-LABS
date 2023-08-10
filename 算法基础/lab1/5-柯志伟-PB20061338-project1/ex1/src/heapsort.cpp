#define numscale (1<<18)
#define infile   "../input/input.txt"
#define outfile  "../output/heapsort/result_18.txt"
#define timefile  "../output/heapsort/time.txt"
#define flag      "numsacle(18)"


#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;


// 调整大根堆
void max_heapify(vector<int>&A,int i,int nums){
    int l=2*i+1;
    int r=2*i+2;
    int largest;
    int temp;
    if(l<=nums-1 && A[l]>A[i]){
        largest = l;
    } 
    else {
        largest = i;
    }
    if(r<=nums-1 && A[r]>A[largest]){
        largest = r;
    }
    if(largest != i){
        temp = A[largest];
        A[largest] = A[i];
        A[i] = temp;
        max_heapify(A,largest,nums);
    }
}

// 建大根堆
void build_max_heap(vector<int>&A,int nums){
    int mid = nums/2;
    for(int i=mid;i>=0;i--){
        max_heapify(A,i,nums);
    }
}

// 堆排序
void heapsort(vector<int>&A,int nums){
    build_max_heap(A,nums);
    int temp;
    for(int i=nums-1;i>=1;i--){
        temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        max_heapify(A,0,i);
    }
}

int main(){
    FILE* fin = fopen(infile,"r");
    FILE* fout= fopen(outfile,"w");
    FILE* ftime = fopen(timefile,"a");

    vector<int> A;
    int data;
    double time_start,time_end,costs;
    
    // 读入数据
    for(int i=0;i<numscale;i++){
        fscanf(fin,"%d\n",&data);
        A.push_back(data);
    }

    time_start = clock();

    // 排序
    heapsort(A,numscale);
    time_end = clock();
    costs = time_end - time_start;

    // 输出结果到文件中
    for(int i=0;i<numscale;i++){
        fprintf(fout,"%d\n",A[i]);
    }

    // 输出运行时间到文件中
    fprintf(ftime,"%s time costs: %f s\n",flag,costs/CLOCKS_PER_SEC);
    fclose(fin);
    fclose(fout);
}