#include<stdlib.h>
#include<string.h>
typedef struct {
    unsigned int weight;
    unsigned int parent,lchild,rchild;
}HTNode,*HuffmanTree;           //动态分配数组存储赫夫曼树
typedef char **HuffmanCode;    //动态分配数组存储赫夫曼编码表

void Select(HuffmanTree HT,int m,int s1,int s2);//在HT的1..i-1中选取weight最小的两个节点，序号为s1和s2

void HuffmanCoding(HuffmanTree& HT,HuffmanCode& HC,unsigned int *w,int n){
    if(n<=1) return ;
    int m = 2*n-1;
    HT = (HuffmanTree)malloc((m+1)*sizeof(HTNode));       //0号单元未用
    HuffmanTree p = HT+1;
    int i = 1;
    for(;i<=n;++i,++p,++w) *p = {*w,0,0,0};          //初始化数组数据
    for(;i<=m;++i,++p) *p = {0,0,0,0};
    int s1,s2;
    for(i = n+1;i<= m;++i) {            //建赫夫曼树
        Select(HT,i-1,s1,s2);         //在HT的1..i-1中选取weight最小的两个节点，序号为s1和s2
        HT[s1].parent = i;HT[s2].parent = i;
        HT[i].lchild = s1;HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
    //从叶子到根逆向求每个字符的赫夫曼编码
    HC = (HuffmanCode)malloc((n+1)*sizeof(char *));  //分配n个字符的赫夫曼编码的头指针向量
    char *cd = (char*)malloc(n*sizeof(char));
    cd[n-1] =(char) "\0";
    for(i=1;i<=n;++i){
        int start = n-1;
        int c = i;
        int f = HT[i].parent;
        for(;f!=0;c=f,f = HT[f].parent ){
            if(HT[f].lchild==c) cd[--start] =(char)"0";
            else cd[--start] =(char) "1";
        } 
        HC[i] = (char *)malloc((n-start)*sizeof(char));
        strcpy(HC[i],&cd[start]);
    }
    free(cd);
} //HuffmanCoding
