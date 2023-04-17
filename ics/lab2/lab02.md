## Lab02  Lending Your Name

   **程序设计思路**
     
     - 实验要求
     
       >基于函数关系：
       >			F(0) = 1 ，F(1) = 1 ，F(2) = 2 
       >			F(n) = (F(n-1) + 2 * F(n-3)) mod 1024 (3 <= n <= 16384)
       >求出给定n下的函数对应值,其中n范围为： 1 <= n <= 16384,结果置于R7
       >另外将自己学号分为4段，作为n求出对应函数值，存入内存
     
     - 设计思路
     
       > 由于n最小值为1，在用递推公式 F(n) = (F(n-1) + 2 * F(n-3))过程中至少需要保存F(n-1),F(n-2),F(n-3)的值，于是使用三个寄存器R1,R2,R3依次保存F(n-1),F(n-2),F(n-3)，通过对n逐次减1判断是否为0对R1,R2,R3的值进行n-1轮更新，最终R1中存有F(n)的值，并存入R7。其中mod1024通过每次R3与1023(1111111111)按位与实现,1023存于内存，程序开始后载入R4。

	 - 最终代码

```LC3
        .ORIG x3000
        AND R7,R7,#0
        AND R6,R6,#0
        AND R5,R5,#0
        AND R4,R4,#0
        AND R3,R3,#0
        AND R2,R2,#0
        AND R1,R1,#0
        
        ADD R1,R1,#1
        ADD R2,R2,#2
        ADD R3,R3,#4
        LD  R4,MOD
          
DECIDE  ADD R0,R0,#-1
        BRz OVER
        
CALC    ADD R7,R3,#0
        ADD R3,R3,R1
        ADD R3,R3,R1
        AND R3,R3,R4
        ADD R1,R2,#0
        ADD R2,R7,#0
        BRnzp DECIDE
        
OVER    ADD R7,R1,#0
        TRAP x25
MOD     .FILL #1023 
FA      .FILL #930
FB      .FILL #18
FC      .FILL #710
FD      .FILL #370
        .END
```
**最终程序长度**
	除去程序初始化寄存器，总共20行(计入存储学号对应的函数值行数)