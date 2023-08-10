; ModuleID = 'fun.c'
source_filename = "fun.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define dso_local i32 @callee(i32 %0) #0 {
1:
  %2 = alloca i32
  store i32 %0, i32* %2
  %3 = load i32, i32* %2
  %4 = mul nsw i32 2, %3
  ret i32 %4
}

; main function
define dso_local i32 @main() #0 {
0:
  %1 = alloca i32
  store i32 110, i32* %1
  %2 = load i32,i32* %1
  %3 = call i32 @callee(i32 %2)
  ret i32 %3
}

