#include<stdio.h>
#include<stdlib.h>
//定义需要的数据类型
typedef struct PolynNode{
	float coef;
	int expn;
	struct PolynNode *next;
}PolynNode,*PolynList;

//声明必要的处理函数
void CreatePolyn(PolynList * polynlistptr);
void AddOrSubPolyn(PolynList P1,PolynList P2);

//主函数
int main(){
	PolynList P1,P2;
	CreatePolyn(&P1);
    CreatePolyn(&P2);
    AddOrSubPolyn(P1,P2);
}

//实现处理函数
void CreatePolyn(PolynList * polynlistptr){
     *polynlistptr = (PolynNode *)malloc(sizeof(PolynNode));
	(*polynlistptr)->next = NULL;
    (*polynlistptr)->expn = 0;
	char c;
	PolynNode *NewNode;
	while(true){
	    NewNode = (PolynNode *)malloc(sizeof(PolynNode));  
	    scanf("%f %d",&NewNode->coef,&NewNode->expn);            
	    NewNode->next = NULL;
	    InsertPolyn(*polynlistptr,NewNode);
	    c=getchar();
	    if(c=='\n') break;	    //正常输入时获取为空格，结束输入时为回车符
    }
}
void AddOrSubPolyn(PolynList Pa,PolynList Pb){
    int i;
	scanf("%d",&i);
	if(i==0) AddPolyn(Pa,Pb);
	if(i==1) SubPolyn(Pa,Pb);
}