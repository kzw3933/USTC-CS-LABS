#include <iostream>
#include <fstream>
#include <random>
#include <ctime>

using namespace std;

/* 
 * 产生[0,2^15-1]的均匀分布的共2^18个随机数
 */

int main(){

    FILE* fout = fopen("../input/input.txt","w");

    std::default_random_engine e;
    std::uniform_int_distribution<int>u(0,(1<<15)-1);
    e.seed(time(0));
    int data;

    for(int i=0;i<(1<<18);i++){
        data = u(e);
        fprintf(fout,"%d\n",data);
    }

    fclose(fout);
    return 0;

}