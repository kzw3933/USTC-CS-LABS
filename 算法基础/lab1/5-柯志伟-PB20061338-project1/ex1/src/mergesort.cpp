#define numscale (1<<18)
#define infile  "../input/input.txt"
#define outfile  "../output/mergesort/result_18.txt"
#define timefile  "../output/mergesort/time.txt"
#define flag      "numsacle(18)"
#define Infinity  (1<<15)

#include <iostream>
#include <vector>
#include <ctime>

using namespace std;


// 结果合并
void merge(vector<int>&A,int p,int q,int r){
    int num1 = q-p+1;
    int num2 = r-q;
    vector<int> L,R;
    for(int i=0;i<num1;i++){
        L.push_back(A[p+i]);
    }
    L.push_back(Infinity);
    for(int i=0;i<num2;i++){
        R.push_back(A[q+1+i]);
    }
    R.push_back(Infinity);
    int i=0;
    int j=0;
    for(int k=p;k<=r;k++){
        if(L[i]<=R[j]){
            A[k] = L[i];
            i++;
        }
        else{
            A[k] = R[j];
            j++;
        }
    }
}


// 归并排序
void mergesort(vector<int>&A,int p,int r){
    if(p==r){
        return ;
    }
    else{
        int mid = (p+r)/2;
        mergesort(A,p,mid);
        mergesort(A,mid+1,r);
        merge(A,p,mid,r);
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
    mergesort(A,0,numscale-1);
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