# Lab4.1 实验报告

<center>
学号: PB20061338
姓名: 柯志伟
</center>

## 实验要求

阅读cminus-f代码优化的框架,了解SSA在代码优化中如何实现以及如何发挥作用,熟悉代码优化过程中优化Pass的框架及实现,并为后续实验准备

## 思考题
### Mem2reg
1. 请简述概念：支配性、严格支配性、直接支配性、支配边界。
  - `支配性`: 在入口节点为 b0 的流图中，当且仅当 bi 位于从 b0 到 bj 的每条路径上时，结点 bi 支配结点 bj
  - `严格支配性`: 令Dom(bi) 集合为包含了支配 bi 的所有结点的名字的集合, 当且仅当 bj 属于 Dom(bi)-bi 时, bj 严格支配 bi
  - `直接支配性`: 令Dom(bi) 集合为包含了支配 bi 的所有结点的名字的集合, Dom(bi)-bi 中最接近 bi 的节点称为 bi 的直接支配节点
  - `支配边界`: 对于节点 n, 如果节点 m 满足如下两个条件: (1) n 支配节点 m的一个前驱 (2) n 不严格支配 m ; 则 m 属于 n 的支配边界
2. `phi`节点是SSA的关键特征，请简述`phi`节点的概念，以及引入`phi`节点的理由。
   - `phi节点`: 在SSA中,同一变量可能在不同的控制流路径上以不同的名字被定值, SSA可使用`phi`函数将这些定值合并 
   - `引入phi节点的理由`: 在SSA中,同一变量可能在不同的控制流路径上以不同的名字被定值, 在对这些路径后续的交汇处对该变量的使用需要将这些定值进行合并,因此SSA引入`phi`函数实现这样的功能
3. 观察下面给出的`cminus`程序对应的 LLVM IR，与**开启**`Mem2Reg`生成的LLVM IR对比，每条`load`, `store`指令发生了变化吗？变化或者没变化的原因是什么？请分类解释。

   ```c
   int globVar;
   int func(int x){
       if(x > 0){
           x = 0;
       }
       return x;
   }
   int main(void){
       int arr[10];
       int b;
       globVar = 1;
       arr[5] = 999;
       b = 2333;
       func(b);
       func(globVar);
       return 0;
   }
   ```

   before `Mem2Reg`：

   ```asm
   @globVar = global i32 zeroinitializer
   declare void @neg_idx_except()
   define i32 @func(i32 %arg0) {
   label_entry:
     %op1 = alloca i32
     store i32 %arg0, i32* %op1
     %op2 = load i32, i32* %op1
     %op3 = icmp sgt i32 %op2, 0
     %op4 = zext i1 %op3 to i32
     %op5 = icmp ne i32 %op4, 0
     br i1 %op5, label %label6, label %label7
   label6:                                                ; preds = %label_entry
     store i32 0, i32* %op1
     br label %label7
   label7:                                                ; preds = %label_entry, %label6
     %op8 = load i32, i32* %op1
     ret i32 %op8
   }
   define i32 @main() {
   label_entry:
     %op0 = alloca [10 x i32]
     %op1 = alloca i32
     store i32 1, i32* @globVar
     %op2 = icmp slt i32 5, 0
     br i1 %op2, label %label3, label %label4
   label3:                                                ; preds = %label_entry
     call void @neg_idx_except()
     ret i32 0
   label4:                                                ; preds = %label_entry
     %op5 = getelementptr [10 x i32], [10 x i32]* %op0, i32 0, i32 5
     store i32 999, i32* %op5
     store i32 2333, i32* %op1
     %op6 = load i32, i32* %op1
     %op7 = call i32 @func(i32 %op6)
     %op8 = load i32, i32* @globVar
     %op9 = call i32 @func(i32 %op8)
     ret i32 0
   }
   ```

   After `Mem2Reg`：

   ```asm
   @globVar = global i32 zeroinitializer
   declare void @neg_idx_except()
   define i32 @func(i32 %arg0) {
   label_entry:
     %op3 = icmp sgt i32 %arg0, 0
     %op4 = zext i1 %op3 to i32
     %op5 = icmp ne i32 %op4, 0
     br i1 %op5, label %label6, label %label7
   label6:                                                ; preds = %label_entry
     br label %label7
   label7:                                                ; preds = %label_entry, %label6
     %op9 = phi i32 [ %arg0, %label_entry ], [ 0, %label6 ]
     ret i32 %op9
   }
   define i32 @main() {
   label_entry:
     %op0 = alloca [10 x i32]
     store i32 1, i32* @globVar
     %op2 = icmp slt i32 5, 0
     br i1 %op2, label %label3, label %label4
   label3:                                                ; preds = %label_entry
     call void @neg_idx_except()
     ret i32 0
   label4:                                                ; preds = %label_entry
     %op5 = getelementptr [10 x i32], [10 x i32]* %op0, i32 0, i32 5
     store i32 999, i32* %op5
     %op7 = call i32 @func(i32 2333)
     %op8 = load i32, i32* @globVar
     %op9 = call i32 @func(i32 %op8)
     ret i32 0
   }
   ```

   ```txt
    func:   
        store i32 %arg0, i32* %op1      被删除, 对函数参数使用alloc的局部变量load-store, 可用函数参数直接代替
        %op2 = load i32, i32* %op1      被删除, 对函数参数使用alloc的局部变量load-store, 可用函数参数直接代替
        store i32 0, i32* %op1          被删除, 对不同分支同一变量的定值使用phi指令代替
        %op8 = load i32, i32* %op1      被删除, 对不同分支同一变量的定值使用phi指令代替

    main:
        store i32 1, i32* @globVar      保留, 对全局变量store
        store i32 999, i32* %op5        保留, 由于GEP指令的影响
        store i32 2333, i32* %op1       删除, 常量直接传入函数, 无需store到局部变量后再load使用
        %op6 = load i32, i32* %op1      删除, 常量直接传入函数, 无需store到局部变量后再load使用
        %op8 = load i32, i32* @globVar  保留, 对全局变量store

   ```

4. 指出放置phi节点的代码，并解释是如何使用支配树的信息的。（需要给出代码中的成员变量或成员函数名称）

```c++
// 步骤二：从支配树获取支配边界信息，并在对应位置插入 phi 指令
    std::map<std::pair<BasicBlock *, Value *>, bool> bb_has_var_phi; // bb has phi for var
    for (auto var : global_live_var_name) {
        std::vector<BasicBlock *> work_list;
        work_list.assign(live_var_2blocks[var].begin(), live_var_2blocks[var].end());
        for (int i = 0; i < work_list.size(); i++) {
            auto bb = work_list[i];
            for (auto bb_dominance_frontier_bb : dominators_->get_dominance_frontier(bb)) {
                if (bb_has_var_phi.find({bb_dominance_frontier_bb, var}) == bb_has_var_phi.end()) {
                    // generate phi for bb_dominance_frontier_bb & add bb_dominance_frontier_bb to work list
                    auto phi =
                        PhiInst::create_phi(var->get_type()->get_pointer_element_type(), bb_dominance_frontier_bb);
                    phi->set_lval(var);
                    bb_dominance_frontier_bb->add_instr_begin(phi);
                    work_list.push_back(bb_dominance_frontier_bb);
                    bb_has_var_phi[{bb_dominance_frontier_bb, var}] = true;
                }
            }
        }
    }

```

```txt
算法方面: 
  对于每个跨block活跃的全局变量, 对它的每一个被定值的block, 找出该block的所有支配边界(预先已使用支配树和CFG构建),在这些支配边界中插入该变量的phi指令

具体实现: 
  预先在建立支配树是已经构建了支配边界(借助idom和cfg使用支配边界的构建算法实现), 使用global_live_var_name 维护了所有跨区域活跃的变量, 并维护了这些变量被定值的block(live_var_2block), 后续使用global_live_var_name、live_var_2block以及支配边界实现插入phi指令的算法
```

5. 算法是如何选择`value`(变量最新的值)来替换`load`指令的？（描述清楚对应变量与维护该变量的位置）

```txt
维护所有跨block活跃的变量名: 
  使用一个 map 栈, std::map<Value *, std::vector<Value*> > var_val_stack; 
对于每个跨block活跃的变量名,对于该变量当前最新的定值始终位于栈顶:
  对于每个block, 分析每一条指令, 如果是phi指令或store指令就将该指令的值作为对应跨区域活跃变量的最新定值放入栈顶,如果是load指令就从栈顶或者当前变量最新定值,通过use-def链修改所有使用该load值得指令,在出block时遍历该block的每一条指令, 如果是phi指令或store指令就对对应跨区域活跃变量的栈进行一次pop操作,根据IR的指令形式与控制流的形式可知,这样可以保证每次对load值的使用是对应跨区域活跃变量的最新定值
```



### 代码阅读总结

1. 了解到llvm IR如何转化到 SSA

```
通过llvm IR的例子可知 llvm IR并不满足SSA的指令格式: 在每个block中除去全局变量的使用,GEP指令的使用外, 其他类型(int, float)变量的使用只有load-store指令(具体表现在进入函数后可能使用局部变量store函数参数的值)不满足SSA,可能会对一个变量多次store造成多次定值, 通过本次实验中的Mem2Reg Pass将这一类的load,store指令删除了.

```

2. 数据流分析的具体实现及应用

```
书上提及了针对数据流分析的框架, 在本次试验中具体了解了如何构建SSA的中间代码形式, 在CFG上分析构建相关的数据结构来支持所需实现的数据流分析, 以及如何使用数据流分析进行优化

```

### 实验反馈 （可选 不会评分）

对本次实验的建议
