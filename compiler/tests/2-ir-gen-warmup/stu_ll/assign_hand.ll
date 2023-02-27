; ModuleID = 'assign.c'
source_filename = "assign.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; main Function
define dso_local i32 @main()#0{

0:                                                                          ; 第1个基本块
    %1 = alloca [10 x i32]                                                  ; 分配数组
    %2 = getelementptr inbounds [10 x i32], [10 x i32]* %1, i64 0, i64 0
    store i32 10, i32* %2
    %3 = getelementptr inbounds [10 x i32], [10 x i32]* %1, i64 0, i64 0
    %4 = load i32 ,i32* %3 
    %5 = mul nsw i32 %4,2
    %6 = getelementptr inbounds [10 x i32], [10 x i32]* %1, i64 0, i64 1
    store i32 %5,i32* %6
    %7 = getelementptr inbounds [10 x i32], [10 x i32]* %1, i64 0, i64 1
    %8 = load i32,i32* %7
    ret i32 %8
}