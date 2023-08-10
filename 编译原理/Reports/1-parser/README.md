# lab1 实验报告
学号:   PB20061338
姓名:   柯志伟

## 实验要求
- 词法分析器
```
    根据`Cminus-f`的词法补全`src/parser/lexical_analyzer.l`文件，完成词法分析器。补全
`lexical_analyzer.l`文件中的模式和动作，能够输出识别出的`token`，`text` ,`line(刚出现
的行数)`，`pos_start(该行开始位置)`，`pos_end(结束的位置,不包含)`。
```
- 语法分析器
```
    完成 `src/parser/syntax_analyzer.y`，补全代码中的提示和Cminus-f的文法规则。
```

## 实验难点
- 理解lex与bison的联动使用，特别是两者之间的一些依赖关系(以防出现问题)
- 完成词法分析时，lex中对于一些特殊符号的正则语法描述规则

## 实验设计
- 词法分析器
  ```
  依据助教提供的Basics.md中Cminus-f中的tokens，写出对应lex中识别的正则文法,其中易出错的是关于注释的正则文法，要考虑到注释不能嵌套以及lex识别时采用的最长匹配，如果考虑不周会出现错误，除此之外，针对每个识别的token,完成必要的动作(将token的内容传递给bison，以及token的位置信息的维护)
  ```
- 语法分析器
  ```
  根据助教提供的Basics.md中的Cminus-f的所有语法规则，完成上下文无关文法的描述，并完成每个规则识别后采取的动作(创建语法树的节点并维护父子节点关系，最终生成程序的语法树)
  ```

## 实验结果验证
- 自已的测试样例
```
int main(void a,int b){
    /*** test 
    example///

    */
    a = 4;
    b = 3;
    return a+b;
}

```

- lexer结果:

```
Token         Text      Line    Column (Start,End)
263            int      0       (0,3)
258           main      0       (4,8)
271              (      0       (8,9)
265           void      0       (9,13)
258              a      0       (14,15)
270              ,      0       (15,16)
263            int      0       (16,19)
258              b      0       (20,21)
272              )      0       (21,22)
275              {      0       (22,23)
258              a      5       (5,6)
268              =      5       (7,8)
259              4      5       (9,10)
269              ;      5       (10,11)
258              b      6       (5,6)
268              =      6       (7,8)
259              3      6       (9,10)
269              ;      6       (10,11)
264         return      7       (5,11)
258              a      7       (12,13)
277              +      7       (13,14)
258              b      7       (14,15)
269              ;      7       (15,16)
276              }      8       (1,2)

```

- parser结果:

```
>--+ program
|  >--+ declaration-list
|  |  >--+ declaration
|  |  |  >--+ fun-declaration
|  |  |  |  >--+ type-specifier
|  |  |  |  |  >--* int
|  |  |  |  >--* main
|  |  |  |  >--* (
|  |  |  |  >--+ params
|  |  |  |  |  >--+ param-list
|  |  |  |  |  |  >--+ param-list
|  |  |  |  |  |  |  >--+ param
|  |  |  |  |  |  |  |  >--+ type-specifier
|  |  |  |  |  |  |  |  |  >--* void
|  |  |  |  |  |  |  |  >--* a
|  |  |  |  |  |  >--* ,
|  |  |  |  |  |  >--+ param
|  |  |  |  |  |  |  >--+ type-specifier
|  |  |  |  |  |  |  |  >--* int
|  |  |  |  |  |  |  >--* b
|  |  |  |  >--* )
|  |  |  |  >--+ compound-stmt
|  |  |  |  |  >--* {
|  |  |  |  |  >--+ local-declarations
|  |  |  |  |  |  >--* epsilon
|  |  |  |  |  >--+ statement-list
|  |  |  |  |  |  >--+ statement-list
|  |  |  |  |  |  |  >--+ statement-list
|  |  |  |  |  |  |  |  >--+ statement-list
|  |  |  |  |  |  |  |  |  >--* epsilon
|  |  |  |  |  |  |  |  >--+ statement
|  |  |  |  |  |  |  |  |  >--+ expression-stmt
|  |  |  |  |  |  |  |  |  |  >--+ expression
|  |  |  |  |  |  |  |  |  |  |  >--+ var
|  |  |  |  |  |  |  |  |  |  |  |  >--* a
|  |  |  |  |  |  |  |  |  |  |  >--* =
|  |  |  |  |  |  |  |  |  |  |  >--+ expression
|  |  |  |  |  |  |  |  |  |  |  |  >--+ simple-expression
|  |  |  |  |  |  |  |  |  |  |  |  |  >--+ additive-expression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  >--+ term
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  >--+ factor
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  >--+ integer
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  >--* 4
|  |  |  |  |  |  |  |  |  |  >--* ;
|  |  |  |  |  |  |  >--+ statement
|  |  |  |  |  |  |  |  >--+ expression-stmt
|  |  |  |  |  |  |  |  |  >--+ expression
|  |  |  |  |  |  |  |  |  |  >--+ var
|  |  |  |  |  |  |  |  |  |  |  >--* b
|  |  |  |  |  |  |  |  |  |  >--* =
|  |  |  |  |  |  |  |  |  |  >--+ expression
|  |  |  |  |  |  |  |  |  |  |  >--+ simple-expression
|  |  |  |  |  |  |  |  |  |  |  |  >--+ additive-expression
|  |  |  |  |  |  |  |  |  |  |  |  |  >--+ term
|  |  |  |  |  |  |  |  |  |  |  |  |  |  >--+ factor
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  >--+ integer
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  >--* 3
|  |  |  |  |  |  |  |  |  >--* ;
|  |  |  |  |  |  >--+ statement
|  |  |  |  |  |  |  >--+ return-stmt
|  |  |  |  |  |  |  |  >--* return
|  |  |  |  |  |  |  |  >--+ expression
|  |  |  |  |  |  |  |  |  >--+ simple-expression
|  |  |  |  |  |  |  |  |  |  >--+ additive-expression
|  |  |  |  |  |  |  |  |  |  |  >--+ additive-expression
|  |  |  |  |  |  |  |  |  |  |  |  >--+ term
|  |  |  |  |  |  |  |  |  |  |  |  |  >--+ factor
|  |  |  |  |  |  |  |  |  |  |  |  |  |  >--+ var
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  >--* a
|  |  |  |  |  |  |  |  |  |  |  >--+ addop
|  |  |  |  |  |  |  |  |  |  |  |  >--* +
|  |  |  |  |  |  |  |  |  |  |  >--+ term
|  |  |  |  |  |  |  |  |  |  |  |  >--+ factor
|  |  |  |  |  |  |  |  |  |  |  |  |  >--+ var
|  |  |  |  |  |  |  |  |  |  |  |  |  |  >--* b
|  |  |  |  |  |  |  |  >--* ;
|  |  |  |  |  >--* }

```

## 实验反馈

虽然这次实验只是完成一些规则的填写，但仍然学到很多东西：包括lex和bison的联合使用，以及在这过程中因为出错以及理解如何实施的过程中的思考，比如lex是如何正确识别我们设置的各个token而不出错(特别是有些token间模式有交叉的部分)，结合课上的理论知识理解bison如何实现自下而上的语法分析，语法树是如何建立的等等，收获很大
