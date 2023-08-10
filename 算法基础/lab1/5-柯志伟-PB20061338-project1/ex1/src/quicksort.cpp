#define numscale (1<<18)
#define infile   "../input/input.txt"
#define outfile  "../output/quicksort/result_18.txt"
#define timefile  "../output/quicksort/time.txt"
#define flag      "numsacle(18)"

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;


// 分划
int partition(vector<int>&A,int p,int r){
    int s = p-1;
    int key = A[r];
    int temp;
    for(int i=p;i<r;i++){
        if(A[i]<=key){
            s++;
            temp = A[s];
            A[s] = A[i];
            A[i] = temp;
        }
    }
    temp = A[r];
    A[r] = A[s+1];
    A[s+1] = temp;
    return s+1;
}


// 快速排序
void quicksort(vector<int>&A,int p,int r){
    if(p>=r){
        return ;
    }
    else {
        int q = partition(A,p,r);
        quicksort(A,p,q-1);
        quicksort(A,q+1,r);
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
    quicksort(A,0,numscale-1);
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