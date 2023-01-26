#define numscale (1<<18)
#define infile   "../input/input.txt"
#define outfile  "../output/countingsort/result_18.txt"
#define timefile  "../output/countingsort/time.txt"
#define flag      "numsacle(18)"
#define range    ((1<<15)-1)

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

void countingsort(vector<int>&A,vector<int>&B,int nums){
    vector<int> counts(range+1,0);

    // 计数
    for(int i=0;i<nums;i++){
        counts[A[i]] ++;
    }

    // 累加
    for(int i=1;i<range+1;i++){
        counts[i] = counts[i] + counts[i-1];
    }

    // 排序
    for(int i=nums-1;i>=0;i--){
        B[counts[A[i]]-1] = A[i];
        counts[A[i]]--;
    } 
}

int main(){
    FILE* fin = fopen(infile,"r");
    FILE* fout = fopen(outfile,"w");
    FILE* ftime = fopen(timefile,"a");
    vector<int> A;
    int data;
    double time_start,time_end,costs;
    
    // 读入数据
    for(int i=0;i<numscale;i++){
        fscanf(fin,"%d\n",&data);
        A.push_back(data);
    }

    vector<int>B(numscale,0);
    time_start = clock();

    // 排序
    countingsort(A,B,numscale);


    time_end = clock();
    costs = time_end - time_start;

     // 输出结果到文件中
    for(int i=0;i<numscale;i++){
        fprintf(fout,"%d\n",B[i]);
    }

    // 输出运行时间到文件中
    fprintf(ftime,"%s time costs: %f s\n",flag,costs/CLOCKS_PER_SEC);
    fclose(fin);
    fclose(fout);
}

