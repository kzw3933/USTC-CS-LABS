## Lab05 Riddle

[实验目的] 
    
    基于一段给定的C++代码，用LC3汇编实现

[实验过程]

    思路：给定的一段C++代码是一个函数，函数逻辑并不复杂，用汇编语言实现时，逻辑部分基本相同，仅仅是处理更多的细节(如用加法实现乘法，用减法实现除余)，另外就是涉及到函数调用即返回机制的实现，主要基于JSR,JSRR等指令的使用

    - 原程序
```c++
    int judge(int r0){
        int i=2;
        r1 = 1;
        while(i*i<=r0){
            if(r0%i == 0){
                r1 = 0;
                break;
            }
            i++;
        }
        return r1;
    }
```
    - 汇编
```asssemble
        .ORIG x3000
        JSR JUDGE
        HALT
JUDGE   AND R6,R6,#0
        ADD R6,R6,#2             
        AND R1,R1,#0
        ADD R1,R1,#1             
HEAD    AND R4,R4,#0
        ADD R5,R6,#0
LOOP    ADD R4,R4,R6
        ADD R5,R5,#-1
        BRp LOOP
        NOT R4,R4
        ADD R4,R4,#1            
        ADD R4,R0,R4
        BRn Tail
        NOT R4,R6
        ADD R4,R4,#1
        ADD R3,R0,#0
Calc    ADD R3,R3,R4
        BRp Calc
        BRz BREAK
        ADD R6,R6,#1
        BRnzp HEAD
BREAK   AND R1,R1,#0         
Tail    RET
        .END

```

[实验总结]

本次实验总体来说比较简单，但从中我们可以清楚了解高级语言中函数的调用，返回机制如何用汇编来实现。