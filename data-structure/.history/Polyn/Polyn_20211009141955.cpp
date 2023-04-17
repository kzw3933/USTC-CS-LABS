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
void AddOrSubPolyn(PolynList Pa,PolynList Pb);
void InsertPolyn(PolynList P,PolynNode *Node);
void AddPolyn(PolynList Pa,PolynList Pb);
void SubPolyn(PolynList Pa,PolynList Pb );
void PrintPolyn(struct PolynNode* P );
int cmp(int m,int n);
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
    NewNode = (PolynNode *)malloc(sizeof(PolynNode));  
    scanf("%f",&NewNode->coef);
    if(NewNode->coef == 0) {
        NewNode->expn = 0;
        InsertPolyn(*polynlistptr,NewNode);  
    }
    else {
        getchar();
        scanf("%d",&NewNode->expn); 
        InsertPolyn(*polynlistptr,NewNode);
        c = getchar();
        while(c != '\n'){
            NewNode = (PolynNode *)malloc(sizeof(PolynNode));  
	        scanf("%f %d",&NewNode->coef,&NewNode->expn);            
	        NewNode->next = NULL;
	        InsertPolyn(*polynlistptr,NewNode);
	        c=getchar();        //正常输入时获取为空格，结束输入时为回车符
        }
    }
}
void AddOrSubPolyn(PolynList Pa,PolynList Pb){
    int i;
	scanf("%d",&i);                         //输入0代表相加，输入1代表相减
	if(i==0) AddPolyn(Pa,Pb);
	if(i==1) SubPolyn(Pa,Pb);
}
void InsertPolyn(PolynList P,PolynNode *Node){
	PolynNode *Pa = P;        //定义一个前向指针
    while(Pa->next &&Pa->next->expn > Node->expn ) Pa=Pa->next;  //找到指数比插入节点大的最后一个节点,下一个节点相等或更小或为空
    if(Pa->next&&Pa->next->expn == Node->expn) {
          Pa->next->coef+=Node->coef;
          if(Pa->next->coef == 0 && P->expn >1)  {
                PolynNode *Pt = Pa->next; 
                Pa->next = Pt->next;
                free(Pt);
                P->expn-=1;
          } 
        if(Pa->next->coef == 0 && P->expn ==1) {
            Pa->next->expn == 0;
        }
    }  
    else {
        Node->next = Pa->next;
        Pa->next = Node;
        P->expn+=1;           //增加一项
    }
}
void AddPolyn(PolynList Pa,PolynList Pb){   //多项式按指数降序排列
	struct PolynNode *ha=Pa;             //定义前向指针
	struct PolynNode *hat=ha->next;        //定义移动指针,初始指向第一个节点
	struct PolynNode *hbt=Pb->next;         //定义移动指针,初始指向第一个节点

	while(hat&&hbt){
	    switch(cmp(hat->expn,hbt->expn)){
	        case 0: hat->coef += hbt->coef;
		            Pb->next = hbt->next;
		            free(hbt);
		            Pb->expn-=1;
		            hbt=Pb->next;
		            if(hat->coef ==0&&Pa->expn>1){
                        ha->next=hat->next;
                        free(hat);
                        hat=ha->next;
                        Pa->expn-=1;
                        }
		            if(hat->coef ==0&&Pa->expn==1)  Pa->next->expn=0;
		            else {
		                ha=ha->next;
                        hat=ha->next;
                    }
		            break;
	        case 1: ha=ha->next;hat=hat->next;break;
            case -1:Pb->next =hbt->next;hbt->next = hat;ha->next=hbt;hbt = Pb->next;ha=ha->next;Pa->expn+=1;Pb->expn-=1;break;
}
	
}
if(hbt) {ha->next = hbt; Pa->expn+=Pb->expn;Pb->next=NULL;Pb->expn=0;}  
PrintPolyn(Pa);

}

void PrintPolyn(PolynList P ){             //打印多项式
	struct PolynNode *Pa=P;
	while(Pa->next){
        Pa=Pa->next;
		printf("%.1f %d ",Pa->coef,Pa->expn);
	}
}

int cmp(int m,int n){           //比较两个节点的指数
    if(m==n)  return 0;
    else if(m>n)  return 1;
    else return -1;
}

void SubPolyn(PolynList Pa,PolynList Pb ){
	PolynNode *hb =Pb;
	while(hb->next) {
        hb->next->coef = -1*hb->next->coef;
        hb=hb->next;
    }
	AddPolyn(Pa,Pb);
}