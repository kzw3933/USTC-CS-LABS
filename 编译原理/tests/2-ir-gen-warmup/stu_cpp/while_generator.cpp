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