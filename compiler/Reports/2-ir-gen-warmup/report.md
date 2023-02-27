# lab2 实验报告
学号： PB20061338 
姓名:  柯志伟

## 问题1: getelementptr
请给出 `IR.md` 中提到的两种 getelementptr 用法的区别,并稍加解释:
  - `%2 = getelementptr [10 x i32], [10 x i32]* %1, i32 0, i32 %0`
  - `%2 = getelementptr i32, i32* %1 i32 %0`

```
  两个GEP操作最终都获得一个i32*的指针，GEP操作的第二个参数指定基指针(base)，对于一个GEP指令第二个参数是一个类型为[10 x i32]*的指针,需再指定两个参
  数m,n来确定偏移，最终得到指向地址为base+10m+n的i32*指针，第二个GEP指令第二个参数是一个类型为i32*的指针，再通过一个参数确定偏移即可得到指向所需的
  i32*的指针
```

## 问题2: cpp 与 .ll 的对应
请说明你的 cpp 代码片段和 .ll 的每个 BasicBlock 的对应关系。

### 1. assign部分

- CPP代码

```Cpp

#include "BasicBlock.h"
#include "Constant.h"
#include "Function.h"
#include "IRBuilder.h"
#include "Module.h"
#include "Type.h"

#include <iostream>
#include <memory>

#define CONST_INT(num) ConstantInt::get(num, module)

int main(){
    auto module = new Module("Cminus code");  // module name是什么无关紧要
    auto builder = new IRBuilder(nullptr, module);
    Type *Int32Type = Type::get_int32_type(module);

    // main函数
    auto mainFun = Function::create(FunctionType::get(Int32Type, {}), "main", module);
    auto bb = BasicBlock::create(module, "entry", mainFun);
    builder->set_insert_point(bb);
    auto retAlloca = builder->create_alloca(Int32Type); 
    auto *arrayType = ArrayType::get(Int32Type, 10);
    auto aAlloca =  builder->create_alloca(arrayType);
    builder->create_store(CONST_INT(0), retAlloca);
    auto a0GEP = builder->create_gep(aAlloca, {CONST_INT(0),CONST_INT(0)});  
    builder->create_store(CONST_INT(10), a0GEP);
    auto a0Load = builder->create_load(a0GEP);
    auto mul = builder->create_imul(a0Load, CONST_INT(2));
    auto a1GEP = builder->create_gep(aAlloca, {CONST_INT(0),CONST_INT(1)});
    builder->create_store(mul, a1GEP);
    auto a1Load = builder->create_load(a1GEP);
    builder->create_ret(a1Load);

    std::cout << module->print();
    delete module;
    return 0;
}

```

- llvm IR代码

```ll

define i32 @main() {
label_entry:
  %op0 = alloca i32
  %op1 = alloca [10 x i32]
  store i32 0, i32* %op0
  %op2 = getelementptr [10 x i32], [10 x i32]* %op1, i32 0, i32 0
  store i32 10, i32* %op2
  %op3 = load i32, i32* %op2
  %op4 = mul i32 %op3, 2
  %op5 = getelementptr [10 x i32], [10 x i32]* %op1, i32 0, i32 1
  store i32 %op4, i32* %op5
  %op6 = load i32, i32* %op5
  ret i32 %op6
}

```

- 基本块的对应关系

```
Cpp代码生成1个BasicBlock，代码及对应的ll中标号如下：
auto bb = BasicBlock::create(module, "entry", mainFun);       --> main函数中的label_entry标号

```


### 2. fun部分

- Cpp代码

```cpp
#include "BasicBlock.h"
#include "Constant.h"
#include "Function.h"
#include "IRBuilder.h"
#include "Module.h"
#include "Type.h"

#include <iostream>
#include <memory>

#define CONST_INT(num) ConstantInt::get(num, module)

int main(){
    auto module = new Module("Cminus code");  // module name是什么无关紧要
    auto builder = new IRBuilder(nullptr, module);
    Type *Int32Type = Type::get_int32_type(module);

    std::vector<Type *> Ints(1, Int32Type);
    auto calleeFunTy = FunctionType::get(Int32Type, Ints);
    auto calleeFun = Function::create(calleeFunTy,
                                 "callee", module);
    auto bb = BasicBlock::create(module, "entry", calleeFun);

    builder->set_insert_point(bb);
    auto aAlloca = builder->create_alloca(Int32Type);
    std::vector<Value *> args;  // 获取gcd函数的形参,通过Function中的iterator
    for (auto arg = calleeFun->arg_begin(); arg != calleeFun->arg_end(); arg++) {
        args.push_back(*arg);   // * 号运算符是从迭代器中取出迭代器当前指向的元素
    }
    builder->create_store(args[0], aAlloca);
    auto aLoad = builder->create_load(aAlloca);
    auto mul = builder->create_imul(CONST_INT(2),aLoad);

    builder->create_ret(mul);

    auto mainFun = Function::create(FunctionType::get(Int32Type, {}),
                                  "main", module);

    bb = BasicBlock::create(module, "entry", mainFun);
    builder->set_insert_point(bb);

    auto retAlloca = builder->create_alloca(Int32Type);
    builder->create_store(CONST_INT(0), retAlloca);
    auto call = builder->create_call(calleeFun, {CONST_INT(110)});
    builder->create_ret(call);

    std::cout << module->print();
    delete module;
    return 0;
}

```

- llvm IR代码

```ll
define i32 @callee(i32 %arg0) {
label_entry:
  %op1 = alloca i32
  store i32 %arg0, i32* %op1
  %op2 = load i32, i32* %op1
  %op3 = mul i32 2, %op2
  ret i32 %op3
}
define i32 @main() {
label_entry:
  %op0 = alloca i32
  store i32 0, i32* %op0
  %op1 = call i32 @callee(i32 110)
  ret i32 %op1
}

```

- 对应关系

```
Cpp代码生成2个BasicBlock，代码及对应的ll中标号如下：
auto bb = BasicBlock::create(module, "entry", calleeFun);     -->  callee函数中的label_entry标号

bb = BasicBlock::create(module, "entry", mainFun);            -->   main函数中的label_entry标号

```


### 3. if部分

- Cpp代码

```cpp
#include "BasicBlock.h"
#include "Constant.h"
#include "Function.h"
#include "IRBuilder.h"
#include "Module.h"
#include "Type.h"

#include <iostream>
#include <memory>

#define CONST_INT(num) ConstantInt::get(num, module)

#define CONST_FP(num) ConstantFP::get(num, module) // 得到常数值的表示,方便后面多次用到

int main() {
    auto module = new Module("Cminus code");  // module name是什么无关紧要
    auto builder = new IRBuilder(nullptr, module);
    Type *Int32Type = Type::get_int32_type(module);
    Type *FloatType = Type::get_float_type(module);

    auto mainFun = Function::create(FunctionType::get(Int32Type, {}),
                                  "main", module);
    auto bb = BasicBlock::create(module, "entry", mainFun);

    builder->set_insert_point(bb);
    auto retAlloca = builder->create_alloca(Int32Type);
    auto aAlloca = builder->create_alloca(FloatType);

    builder->create_store(CONST_INT(0), retAlloca);
    builder->create_store(CONST_FP(5.555), aAlloca);
    auto aLoad =  builder->create_load(aAlloca);
    auto fcmp = builder->create_fcmp_gt(aLoad,CONST_FP(1));

    auto trueBB = BasicBlock::create(module, "trueBB", mainFun);    // true分支
    auto falseBB = BasicBlock::create(module, "falseBB", mainFun);
    auto retBB = BasicBlock::create(
        module, "", mainFun);
    auto br = builder->create_cond_br(fcmp, trueBB, falseBB);

    builder->set_insert_point(trueBB);  // if true; 分支的开始需要SetInsertPoint设置
    builder->create_store(CONST_INT(233), retAlloca);
    builder->create_br(retBB);

    builder->set_insert_point(falseBB);
    builder->create_store(CONST_INT(0), retAlloca);
    builder->create_br(retBB); 

    builder->set_insert_point(retBB);  // ret分支
    auto retLoad = builder->create_load(retAlloca);
    builder->create_ret(retLoad);

    std::cout << module->print();
    delete module;
    return 0;

}

```

- llvm IR代码

```ll
define i32 @main() {
label_entry:
  %op0 = alloca i32
  %op1 = alloca float
  store i32 0, i32* %op0
  store float 0x40163851e0000000, float* %op1
  %op2 = load float, float* %op1
  %op3 = fcmp ugt float %op2,0x3ff0000000000000
  br i1 %op3, label %label_trueBB, label %label_falseBB
label_trueBB:                                                ; preds = %label_entry
  store i32 233, i32* %op0
  br label %label4
label_falseBB:                                                ; preds = %label_entry
  store i32 0, i32* %op0
  br label %label4
label4:                                                ; preds = %label_trueBB, %label_falseBB
  %op5 = load i32, i32* %op0
  ret i32 %op5
}

```

- 对应关系

```
Cpp代码生成4个BasicBlock，代码及对应的ll中标号如下：

auto bb = BasicBlock::create(module, "entry", mainFun);         -->  main函数中的label_entry标号
auto trueBB = BasicBlock::create(module, "trueBB", mainFun);    -->  main函数中的label_trueBB标号
auto falseBB = BasicBlock::create(module, "falseBB", mainFun);  -->  main函数中的label_falseBB标号
auto retBB = BasicBlock::create( module, "", mainFun);          -->  main函数中的label_retBB标号                 
                                         
```



### 4. while部分

- Cpp代码

```cpp
#include "BasicBlock.h"
#include "Constant.h"
#include "Function.h"
#include "IRBuilder.h"
#include "Module.h"
#include "Type.h"

#include <iostream>
#include <memory>

#define CONST_INT(num) ConstantInt::get(num, module)

int main() {
    auto module = new Module("Cminus code");  // module name是什么无关紧要
    auto builder = new IRBuilder(nullptr, module);
    Type *Int32Type = Type::get_int32_type(module);

    auto mainFun = Function::create(FunctionType::get(Int32Type, {}),
                                  "main", module);
    auto bb = BasicBlock::create(module, "entry", mainFun);
    // BasicBlock的名字在生成中无所谓,但是可以方便阅读
    builder->set_insert_point(bb);

    auto retAlloca = builder->create_alloca(Int32Type);
    auto aAlloca = builder->create_alloca(Int32Type);
    auto iAlloca = builder->create_alloca(Int32Type);
    
    builder->create_store(CONST_INT(0), retAlloca);
    builder->create_store(CONST_INT(10), aAlloca);
    builder->create_store(CONST_INT(0), iAlloca);

    auto condBB = BasicBlock::create(module, "condBB", mainFun);
    auto loopBB = BasicBlock::create(module, "loopBB", mainFun);
    auto retBB = BasicBlock::create(module, "", mainFun);
    
    builder->create_br(condBB);
    builder->set_insert_point(condBB);

    auto iLoad = builder->create_load(iAlloca);
    auto icmp = builder->create_icmp_lt(iLoad,CONST_INT(10));
    builder->create_cond_br(icmp,loopBB,retBB);

    builder->set_insert_point(loopBB);
    iLoad = builder->create_load(iAlloca);
    auto add = builder->create_iadd(iLoad,CONST_INT(1));
    builder->create_store(add, iAlloca);
    auto aLoad =  builder->create_load(aAlloca);
    iLoad = builder->create_load(iAlloca);
    add = builder->create_iadd(aLoad,iLoad);
    builder->create_store(add, aAlloca);
    builder->create_br(condBB);

    builder->set_insert_point(retBB);
    aLoad =  builder->create_load(aAlloca);
    builder->create_ret(aLoad);

    std::cout << module->print();
    delete module;
    return 0;
}

```

- llvm IR代码

```ll

define i32 @main() {
label_entry:
  %op0 = alloca i32
  %op1 = alloca i32
  %op2 = alloca i32
  store i32 0, i32* %op0
  store i32 10, i32* %op1
  store i32 0, i32* %op2
  br label %label_condBB
label_condBB:                                                ; preds = %label_entry, %label_loopBB
  %op3 = load i32, i32* %op2
  %op4 = icmp slt i32 %op3, 10
  br i1 %op4, label %label_loopBB, label %label10
label_loopBB:                                                ; preds = %label_condBB
  %op5 = load i32, i32* %op2
  %op6 = add i32 %op5, 1
  store i32 %op6, i32* %op2
  %op7 = load i32, i32* %op1
  %op8 = load i32, i32* %op2
  %op9 = add i32 %op7, %op8
  store i32 %op9, i32* %op1
  br label %label_condBB
label10:                                                ; preds = %label_condBB
  %op11 = load i32, i32* %op1
  ret i32 %op11
}
```

- 对应关系

```
Cpp代码生成4个BasicBlock，代码及对应的ll中标号如下：

auto bb = BasicBlock::create(module, "entry", mainFun);           -->  main函数中的label_entry标号
auto condBB = BasicBlock::create(module, "condBB", mainFun);      -->  main函数中的label_condBB标号
auto loopBB = BasicBlock::create(module, "loopBB", mainFun);      -->  main函数中的label_loopBB标号
auto retBB = BasicBlock::create(module, "", mainFun);             -->  main函数中的label10标号


```


## 问题3: Visitor Pattern
分析 `calc` 程序在输入为 `4 * (8 + 4 - 1) / 2` 时的行为：
1. 请画出该表达式对应的抽象语法树（使用 `calc_ast.hpp` 中的 `CalcAST*` 类型和在该类型中存储的值来表示），并给节点使用数字编号。



2. 请指出示例代码在用访问者模式遍历该语法树时的遍历顺序。

序列请按如下格式指明（序号为问题 3.1 中的编号）：  
3->2->5->1


3.1,3.2答案如图：
![](./../../Documentations/common/figs/asttree.jpg)
## 实验难点
```
- 对llvm中间代码GEP指令的理解(易受C语言中指针的用法影响而混淆)
  阅读助教提供的链接网址理解GEP的用法

- 阅读助教提供的LightIR库代码(由于之前基本没用过C++，另外对C++面向对象实现代码的理解出现困难)，并使用库提供的接口
  阅读C++的语法书，以及从网上查阅资料理解LightIR的代码，并学习助教提供的例子熟悉接口的用法

```

## 实验反馈
```
收获：
  通过此次实验熟悉了LLVM IR指令以及LightIR的C++接口使用，同时了解了Cminus-f到LLVM IR的对应关系，了解了访问者模式的使用
反馈：
  在实验过程中由于对Cmakelist语法的不熟悉，对程序具体的编译链接的依赖关系如何解决的有些疑问
```

