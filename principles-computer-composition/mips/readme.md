#### 变量命名说明


###### 1
1. `xx_ready_go`

```
xx流水段数据已经准备好(有效或无效)

```

2. `xx_to_xx_valid`
```
xx输入到xx的数据有效

```

3. `xx_allowin`
```

xx流水段的数据待更新(数据无效或数据被读取)

```


4. `xx_valid`

```
xx流水段的数据是有效的

```

5. `xx_ex`
```
xx流水段发生异常

```
6. `xx_excode`
```
xx流水段发生异常的异常码
```

7. `inst_xxx`
```
xx指令
```

8. `s_is_xx`,`l_is_xx`
```
访存指令xx
```
9. xx_is_xx

```
xx的结果来自xx
```
###### 2

1.  `epc`,`eret_taken`,`b_or_j`
```
epc：来自cp0寄存器堆中的EPC寄存器
eret_taken：eret指令引起的异常，若eret为1，表示异常处理返回，nextpc应选择epc
b_or_j:来自ds，记录id段指令是否是branch指令或jump指令，如果b_or_j为1，意味着当前if段指令是延迟槽指令
bd:判断当前if段指令是否是延迟槽指令，这与异常处理有关
```
2. `fwd1`,`fwd2`,`inst_undef`,`op`,`rs`,`rt`,`sa`,`rd`,`jidx`,`sel`,`rs_lt_zero`,`has_int`,`cp0_addr`

3. 