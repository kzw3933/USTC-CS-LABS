; ModuleID = 'if.c'
source_filename = "if.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; main Function
define dso_local i32 @main()#0{
0:                                              ;第1个基本块
    %1 = alloca i32                                 
    %2 = alloca float 
    store float 0x40163851E0000000,float* %2    
    %3 = load float ,float* %2
    %4 = fcmp  ugt float %3,1.000000e+00
    br  i1 %4 ,label %5 ,label %6
5:                                              ;第2个基本块
    store i32 233, i32* %1
    br label %7

6:                                              ;第3个基本块
    store i32 0, i32* %1
    br label %7

7:                                              ;第4个基本块
    %8 = load i32 ,i32* %1 
    ret i32 %8
}