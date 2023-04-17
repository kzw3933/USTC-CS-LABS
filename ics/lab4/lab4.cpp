#include<stdlib.h>
#include<stdio.h>
int R0,R1,R2,R3,R4,R5,R6,R7;
int target = 0b100100000;
int main()
{
    R1 = target;
    s();
}
void f()
{
    R2 = 1;
    R3 = 0b1000;
    R4 = 0;
    h();
    
}
void h()
{
    R5 = R1&R3;
    if(R5!=0) R4 = R2+R4;
    R2 = R2*2;
    R3 = R3*2;
    if(R3!=0)   h();
}

void s()
{
    f();
    R2 = R1&(0b111);
    R1 = R2+R4;
    R0 = R1-7;
    if(R0>0) s();
    R0 = R1-7;
    if(R0<0) system("pause");
    R1 = R1-7;
    system("pause");
}
