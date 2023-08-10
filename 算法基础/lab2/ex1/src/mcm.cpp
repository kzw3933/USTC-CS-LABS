#include <vector>
#include <iostream>
#include <fstream>
#include "windows.h"

#define Inifity 9223372036854775807

using namespace std;


// 动态规划求解矩阵链乘问题
void matrix_chain_order(vector<long long>&p,vector<vector<long long> >&m,vector<vector<int> >&s){
    int n = p.size() -1;
    for(int i=0;i<n;i++){
        m[i][i] = 0;
    }
    for(int l=2;l<=n;l++){
        for(int i=0;i<n-l+1;i++){
            int j = i+l-1;
            m[i][j] = Inifity;
            for(int k=i;k<=j-1;k++){
                long long q = m[i][k] + m[k+1][j] +p[i]*p[k+1]*p[j+1];
                if (q<0)
                    cout << " error";
                if (q< m[i][j] ){
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }
}
void print_optimal_parens(vector<vector<int> >&s,int i,int j, string& result){
    if (i==j){
        result.push_back('A');
        result = result + std::to_string(i+1);
    }
    else {
        result.push_back('(');
        print_optimal_parens(s,i,s[i][j], result);
        print_optimal_parens(s,s[i][j]+1,j, result);
        result.push_back(')');
    }
}

int main(){

    ifstream fin("E:\\Savefiles\\Algorithm\\lab2\\ex1\\input\\2_1_input.txt",ios::in);

    LARGE_INTEGER   StartingTime, EndingTime, ElapsedMicroseconds;
    LARGE_INTEGER   Frequency;

    // 准备数据
    vector<int> lens;
    vector<vector<long long> > mats;
    vector<double> costs;
    vector<string> results;
    vector<long long> chain_times;
    int count = 0;

    while(! fin.eof()) {
        int tmp_int1;
        int tmp_int2;
        fin >> tmp_int1;
        count ++;
        lens.push_back(tmp_int1);
        vector<long long> tmp_vec;
        while(tmp_int1 >= 0) {
            fin >> tmp_int2;
            tmp_vec.push_back(tmp_int2);
            tmp_int1-- ;
        }
        mats.push_back(tmp_vec);
    }
    
    // 运行矩阵链乘的动态规划算法并衡量时间性能
    vector<vector<long long> > m;
    vector<vector<int> > s;

    for(int repeat = 0; repeat < 10000; repeat++) {
        for(int i=0; i< count; i++) {
            m.clear();
            s.clear();
            m.resize(lens[i]);
            s.resize(lens[i]);
            for(int j=0;j< lens[i]; j++) {
                m[j].resize(lens[i]);
                s[j].resize(lens[i]);
            }

            if(repeat == 0) {
                QueryPerformanceFrequency(&Frequency);
                QueryPerformanceCounter(&StartingTime);
                matrix_chain_order(mats[i], m, s);
                QueryPerformanceCounter(&EndingTime);
                ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
                double time_cost = (ElapsedMicroseconds.QuadPart * 1000.0) / Frequency.QuadPart;
                costs.push_back(time_cost);
                string result;
                print_optimal_parens(s, 0, lens[i]-1,result);
                results.push_back(result);
                chain_times.push_back(m[0][lens[i]-1]);
            }
            else {
                QueryPerformanceFrequency(&Frequency);
                QueryPerformanceCounter(&StartingTime);
                matrix_chain_order(mats[i], m, s);
                QueryPerformanceCounter(&EndingTime);
                ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
                double time_cost = (ElapsedMicroseconds.QuadPart * 1000.0) / Frequency.QuadPart;
                costs[i] = (costs[i] + time_cost) / 2.0 ;
            }
        }
    }
    
    string filename("E:\\Savefiles\\Algorithm\\lab2\\ex1\\output\\result.txt");
    string filetime("E:\\Savefiles\\Algorithm\\lab2\\ex1\\output\\time.txt");
    ofstream fout(filename, ios::out);
    ofstream ftime(filetime, ios::out);

    for(int i=0; i< count; i++) {
        cout << "Minimum chain times: " << chain_times[i] << endl;
        cout << "Scheme: " << results[i] << endl;
        cout << "matrix scale: " << lens[i] << " time costs: " << costs[i] << endl;
    }

    for(int i=0; i< count; i++) {
        fout << chain_times[i] << endl;
        fout << results[i] << endl;
        ftime << costs[i] << endl;
    }

    return 0;
}