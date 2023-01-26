
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//定义必要的数据结构
typedef struct charNode{
    char ch;
    unsigned int weight;
    struct charNode* next; 
} charNode,*charList;

typedef struct {
    unsigned int weight;
    unsigned int parent,lchild,rchild;
}HTNode,*HuffmanTree;           //动态分配数组存储赫夫曼树

typedef char ** HuffmanCode;



//声明相关函数
int NotIn(charList CL,char ch);
int NumOfCharList(charList CL);
void GetCharsWeights(const char* s,charList * CLptr);

void Select(HuffmanTree HT,int m,int *s1,int *s2);//在HT的1..i-1中选取weight最小的两个节点，序号为s1和s2

//1.首先对提取文本所用的编码字符和出现的权重
//实现相关函数
void GetCharsWeights(const char* s,charList * CLptr){ //s为文件名，chars为字符集，weights为权重
    FILE *fp = fopen(s,"r");
    if(fp == NULL) {
        printf("File Open Error!");
        exit(1);
    } 
    char ch;
    *CLptr = (charNode *) malloc(sizeof(charNode)) ; //带头结点的字符链表
    (*CLptr)->next = NULL;
    charNode * p = *CLptr;
    while((ch = getc(fp))!=EOF){
        if(NotIn(*CLptr,ch)||!NumOfCharList(*CLptr)) {
            charNode* Node =(charNode *) malloc(sizeof(charNode));
            Node->ch = ch;
            Node->next = NULL;
            Node->weight = 0;
            p->next = Node;
            p = p->next;
        }
    }
    p = *CLptr;
    while(p->next){
        rewind(fp);
        while((ch = getc(fp))!=EOF){
            if((p->next->ch)==ch) p->next->weight++;
        }
        p = p->next;
    }
    fclose(fp);
}
int NotIn(charList CL,char ch){
    charNode *p = CL;
    int i;
    while((p->next)&&((p->next->ch)!= ch ))  p = p->next;
    if(!(p->next))  i = 1;
    else  i =  0;
    return i;
}

int NumOfCharList(charList CL){
    charNode *p = CL;
    int m=0;
    while(p->next){
        m++;
        p = p->next;
    }
    return m;
}


//2.建立Huffman树并求出Huffman编码
//定义相关的函数
void HuffmanCoding(HuffmanTree* HTptr,charList CL,HuffmanCode* HCptr){
    int n=NumOfCharList(CL);
    if(n<1) {
        printf("ERROR!The File Contains Nothing!\n");
        exit(1);
    }
    int m =2*n-1;
     *HTptr = (HuffmanTree)malloc((m+1)*sizeof(HTNode));       //0号单元未用,作为头部
    HuffmanTree p = *HTptr+1;
    int i = 1;
    charNode *q = CL->next; 
    for(;i<=n;++i,++p,q = q->next) *p = {q->weight,0,0,0};          //初始化数组数据
    for(;i<=m;++i,++p) *p = {0,0,0,0};
    
    int s1,s2;
    for(i = n+1;i<= m;++i) {            //建赫夫曼树
        Select(*HTptr,i-1,&s1,&s2);         //在HT的1..i-1中选取weight最小的两个节点，序号为s1和s2
        (*HTptr)[s1].parent = i;(*HTptr)[s2].parent = i;
        (*HTptr)[i].lchild = s1;(*HTptr)[i].rchild = s2;
        (*HTptr)[i].weight = (*HTptr)[s1].weight + (*HTptr)[s2].weight;
    }
    //从叶子到根逆向求每个字符的赫夫曼编码
 (*HCptr) = (HuffmanCode)malloc((n+1)*sizeof(char *));  //分配n个字符的赫夫曼编码的头指针向量
    char *cd = (char*)malloc(n*sizeof(char));
    cd[n-1] = '\0';
    for(i=1;i<=n;++i){
        int start = n-1;
        int c = i;
        int f = (*HTptr)[i].parent;
        for(;f!=0;c=f,f = (*HTptr)[f].parent ){
            if((*HTptr)[f].lchild==c) cd[--start] ='0';
            else cd[--start] = '1';
        } 
        (*HCptr)[i] = (char *)malloc((n-start)*sizeof(char));
        strcpy((*HCptr)[i],&cd[start]);
    }
    free(cd);
} 

void Select(HuffmanTree HT,int m,int *s1,int *s2){ //在HT的1..m中选取weight最小的两个节点，序号为s1和s2
    int i;  //不妨设s1最小，s2次之
    int signum=0;
    int a,b;
    for(i=1;i<=m;i++){
        if(HT[i].parent!=0) continue;
        else {
            if(signum == 0)  {
                a = i;
                signum =1;
            }
            else if(signum == 1){
                b = i;
                signum =2;
            }  
            else if(signum == 2) {
                *s1 = (HT[a].weight <= HT[b].weight)?a:b;
                *s2 = (HT[a].weight <= HT[b].weight)?b:a;
                break;
            }
        }
    }  
    for(i = 1;i<= m;i++){
        if((HT[i].parent)!=0)  continue;
        else {
            if(HT[i].weight < HT[*s1].weight) {
                *s2 = *s1;
                *s1 = i;
           }
            else if(HT[i].weight < HT[*s2].weight){
                *s2 = i;
        }

        }
    }
} 

//测试
int main(){
    charList CL;
    HuffmanCode HC;
    HuffmanTree HT;
    GetCharsWeights("test.txt",&CL);
    HuffmanCoding(&HT,CL,&HC);
    int num = NumOfCharList(CL);
    charNode*p = CL->next;
    int i=1;
    while(num){
        printf("%c:%d-----%s\n",p->ch,p->weight,HC[i]);
        i++;num--; 
        p = p->next;
}
}




