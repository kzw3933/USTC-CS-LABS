#include <vector>
#include <string>
#include <ctime>
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

void lcs_length(string& X, string& Y,vector<vector<int> >&c, vector<vector<char> >&b){
    int m = X.size();
    int n = Y.size();
    for(int i=1;i<=m;i++)       c[i][0] = 0; 
    for(int j=0;j<=n;j++)       c[0][j] = 0;
    for(int i=1;i<= m;i++){
        for(int j=1;j<= n;j++){
            if (X[i-1] == Y[j-1]){
                c[i][j] = c[i-1][j-1]+1;
                b[i][j] = 'b';
            }
            else if(c[i-1][j] >= c[i][j-1]){
                c[i][j] = c[i-1][j];
                b[i][j] = 'u';
            }
            else {
                c[i][j] = c[i][j-1];
                b[i][j] = 'l';
            }
        }
    }
}
void print_lcs(vector<vector<char> >&b, string& X, int i,int j, string& commonstr){
    if(i==0 || j==0){
        return ;
    }
    if (b[i][j] == 'b'){
        print_lcs(b,X,i-1,j-1, commonstr);
        commonstr.push_back(X[i-1]);
    }
    else if(b[i][j] == 'u'){
        print_lcs(b,X,i-1,j, commonstr);
    }
    else {
        print_lcs(b,X,i,j-1, commonstr);
    }
}

int main(){

    ifstream fin;
    fin.open("E:\\Savefiles\\Algorithm\\lab2\\ex2\\input\\2_2_input.txt", ios::in);

    int count = 0;

    vector<int> lens;
    vector<string> strs;
    vector<string> common_strs;

    LARGE_INTEGER   StartingTime, EndingTime, ElapsedMicroseconds;
    LARGE_INTEGER   Frequency;


    vector<double> costs;

    // 准备数据
    while(! fin.eof()) {
        count ++;
        int tmp_int;
        fin >> tmp_int;
        lens.push_back(tmp_int);
        string tmp_str;
        fin >> tmp_str;
        strs.push_back(tmp_str);
        fin >> tmp_str;
        strs.push_back(tmp_str);    
    }

    // 计算最长公共子序列并衡量时间性能

    vector<vector<int> > c;
    vector<vector<char> > b;

    for(int repeat = 0; repeat < 10000 ;repeat++) {
        for(int i=0; i< count; i++){
            c.clear();
            b.clear();
            int m = lens[i];
            int n = lens[i];
            c.resize(m+1);
            b.resize(m+1);

            for(int j=0;j<=m;j++) {
                c[j].resize(n+1);
                b[j].resize(n+1);
            }
            if (repeat == 0) {
                QueryPerformanceFrequency(&Frequency);
                QueryPerformanceCounter(&StartingTime);
                lcs_length(strs[2*i],strs[2*i+1],c,b);
                QueryPerformanceCounter(&EndingTime);
                ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
                double time_cost = (ElapsedMicroseconds.QuadPart * 1000.0) / Frequency.QuadPart;
                costs.push_back(time_cost);
                string commonstr;
                print_lcs(b,strs[2*i],m,n, commonstr);
                common_strs.push_back(commonstr);
            }
            else {
                QueryPerformanceFrequency(&Frequency);
                QueryPerformanceCounter(&StartingTime);
                lcs_length(strs[2*i],strs[2*i+1],c,b);
                QueryPerformanceCounter(&EndingTime);
                ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
                double time_cost = (ElapsedMicroseconds.QuadPart * 1000.0) / Frequency.QuadPart;
                costs[i] = (costs[i] + time_cost) / 2.0 ;
            }   
        }
    }
    
    string filename_base("E:\\Savefiles\\Algorithm\\lab2\\ex2\\output\\result_");
    string filetime("E:\\Savefiles\\Algorithm\\lab2\\ex2\\output\\time.txt");
    ofstream ftime(filetime, ios::out);

    for(int i=0; i< count; i++) {

        cout << "length of lcs: " << common_strs[i].size() << endl;
        cout << "lcs: " << common_strs[i] << endl;
        cout << "string length: " << lens[i] << " time costs: " << costs[i] << endl; 
    }

    for(int i=0; i< count; i++) {
        string filename = filename_base;
        filename += std::to_string(lens[i]);
        filename += ".txt";
        ofstream fout(filename, ios::out);
        fout << common_strs[i].size() << endl;
        fout << common_strs[i] << endl;
        ftime << costs[i] << endl; 
    }

    return 0;
}