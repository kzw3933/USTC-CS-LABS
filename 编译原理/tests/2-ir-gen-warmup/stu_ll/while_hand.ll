; ModuleID = 'while.c'
source_filename = "while.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; main Function
define dso_local i32 @main()#0{
0:                                              ;第1个基本块
    %1 = alloca i32                                 
    %2 = alloca i32
    store i32 10,i32* %1   
    store i32 0,i32* %2
    br label %3
3:                                              ;第2个基本块
    %4 = load i32,i32* %2
    %5 = icmp slt i32 %4,10
    br i1 %5 ,label %6 ,label %12
6:                                              ;第3个基本块
    %7 = load i32,i32* %2
    %8 = add nsw i32 %7,1
    store i32 %8,i32* %2
    %9 = load i32 ,i32* %1
    %10 = load i32,i32* %2
    %11 = add nsw i32 %9,%10
    store i32 %11,i32* %1
    br label %3
12:                                             ;第4个基本块
    %13 = load i32,i32* %1
    ret i32 %13
}