#include<stdio.h>
#include<stdlib.h>
#include<math.h>
//定义需要的数据类型
typedef struct PolynNode{
	float coef;
	int expn;
	struct PolynNode *next;
}PolynNode,*PolynList;

//定义相关的功能函数
void ShowChoices();

void CreatePolyn(PolynList );
void InsertPolyn(PolynList, PolynNode *);

void AddPolyn(PolynList,PolynList);
void SubPolyn(PolynList,PolynList);

void ValuePolyn(PolynList);
void DerivatPolyn(PolynList); 
void MultiplyPolyn(PolynList,PolynList);

void DeleteZeroPolyn(PolynList);
void PrintPolyn(PolynList);

int Cmp(int ,int);



//主函数
int main(){
	printf("*************欢迎进入多项式计算系统************\n\n");
	printf("现提供以下服务\n");
    ShowChoices();
	unsigned int choice;
	scanf("%d",&choice);
    int i=0;
	PolynList P1=(PolynNode *)malloc(sizeof(struct PolynNode));
    P1->next = NULL;
	P1->expn = 0;
    PolynList P2=(PolynNode *)malloc(sizeof(struct PolynNode));
    P2->next = NULL;
	P2->expn = 0;
    while (true)
    {
       switch(choice)
       {
			case 1:if(i==0){
                    CreatePolyn(P1);i=1;
                    }               //第一个多项式同时作为结果多项式
                    else if(i==1)  CreatePolyn(P2) ;   //第二个多项式同时作为之后的输入存储多项式
				    ShowChoices();
				    scanf("%d",&choice);
				    break;
			case 2:AddPolyn(P1,P2);
				    ShowChoices();
				    scanf("%d",&choice);
				    break;
			case 3:SubPolyn(P1,P2);
				    ShowChoices();
				    scanf("%d",&choice);
				    break;	
            case 4:ValuePolyn(P1);
                    ShowChoices();
                    scanf("%d",&choice);
                    break;
            case 5:DerivatPolyn(P1);
                    ShowChoices();
                    scanf("%d",&choice);
                    break;
            case 6:MultiplyPolyn(P1,P2);
                    ShowChoices();
                    scanf("%d",&choice);
                    break;
            case 7: PrintPolyn(P1);
                    ShowChoices();
                    scanf("%d",&choice);
                    break;
            case 0:	return 0;
        }
    }
}

//功能函数的实现
void ShowChoices()
{
    printf("\n");
	printf("         1:创建多项式         ");printf("            2:多项式加法         \n");
	printf("         3:多项式减法         ");printf("            4:多项式求值         \n");
    printf("         5:多项式求导         ");printf("            6:多项式乘法         \n");
    printf("         7:多项式打印         ");printf("            8:退出此系统         \n");
	printf("请选择:\n");
}

void CreatePolyn(PolynList P)
{
	printf("请输入多项式系数与指数对，键入回车结束输入\n");
	float coef;
	int expn;
    char c;
	PolynNode *NewNodeptr;
	while(true)
    {
        NewNodeptr = (PolynNode *)malloc(sizeof(PolynNode));  
        scanf("%f %d",&NewNodeptr->coef,&NewNodeptr->expn);            
        NewNodeptr->next = NULL;
        InsertPolyn(P,NewNodeptr);
        c=getchar();
        if(c=='\n') break;	
    }
    DeleteZeroPolyn(P);
    printf("创建完成。\n");
}

void InsertPolyn(PolynList P,PolynNode *Node){
	PolynNode *Pa = P;        //定义一个前向指针
    while(Pa->next &&Pa->next->expn > Node->expn ) Pa=Pa->next;  //找到指数比插入节点大的最后一个节点,下一个节点相等或更小或为空
    if(Pa->next&&Pa->next->expn == Node->expn)
          Pa->next->coef+=Node->coef;
    else{
            Node->next = Pa->next;
            Pa->next = Node;
            P->expn+=1; //增加一项
    }
          
    
}

void AddPolyn(PolynList Pa,PolynList Pb){   //多项式按指数降序排列
	struct PolynNode *ha=Pa;             //定义前向指针
	struct PolynNode *hat=ha->next;        //定义移动指针,初始指向第一个节点
	struct PolynNode *hbt=Pb->next;         //定义移动指针,初始指向第一个节点

	while(hat&&hbt){
	    switch(Cmp(hat->expn,hbt->expn)){
	        case 0: hat->coef += hbt->coef;
		            Pb->next = hbt->next;
		            free(hbt);
		            Pb->expn-=1;
		            hbt=Pb->next;
		            ha=ha->next;
                    hat=ha->next;
		            break;
	        case 1: ha=ha->next;hat=hat->next;break;
            case -1:Pb->next =hbt->next;hbt->next = hat;ha->next=hbt;hbt = Pb->next;ha=ha->next;Pa->expn+=1;Pb->expn-=1;break;
}	
}
    if(hbt) {ha->next = hbt; Pa->expn+=Pb->expn;Pb->next=NULL;Pb->expn=0;} 
    DeleteZeroPolyn(Pa); 
    PrintPolyn(Pa);

}

void SubPolyn(PolynList Pa,PolynList Pb ){
	PolynNode *hb =Pb;
	while(hb->next) {
        hb->next->coef = -1*hb->next->coef;
        hb=hb->next;
    }
	AddPolyn(Pa,Pb);
}

void ValuePolyn(struct PolynNode *P)
{
    printf("请输入代入的值：\n");
    float x ;
    scanf("%f",&x);
    printf("\n");
    PrintPolyn(P);
    printf("(x=%f)",x);
    printf(" = ");
    float sum=0;
    struct PolynNode *Pa=P;
	while(Pa->next)
    {
        Pa=Pa->next;
        sum += Pa->coef*pow(x,Pa->expn);
    }
    printf("%f\n",sum);
}

void DerivatPolyn(struct PolynNode *P)
{
    struct PolynNode *Pa=P;
    while(Pa->next)
    {
        if(Pa->next->expn!=0) 
        {
            Pa=Pa->next;Pa->coef*=Pa->expn;Pa->expn--;
        }
        else {
            Pa=Pa->next;Pa->coef*=Pa->expn;
        }
       
    }
    DeleteZeroPolyn(P);
    printf("结果为：\n");
    PrintPolyn(P);
}

void MultiplyPolyn(PolynList Pa,PolynList Pb)
{
    PolynList P=(PolynNode *)malloc(sizeof(struct PolynNode));
    P->expn=0;P->next=NULL;
    PolynNode *ha,*hb;
    ha=Pa;
    PolynNode *NewNodeptr;
    while(ha->next)
    {
        ha=ha->next;
        hb=Pb;
        while(hb->next)
        {
            NewNodeptr = (PolynNode *)malloc(sizeof(struct PolynNode));NewNodeptr->next = NULL;
            hb=hb->next; NewNodeptr->coef=ha->coef * hb->coef;NewNodeptr->expn = ha->expn + hb->expn;
            if(NewNodeptr->coef!=0) InsertPolyn(P,NewNodeptr);
        }
    }
    if(P->expn==0) {
        NewNodeptr = (PolynNode *)malloc(sizeof(struct PolynNode));NewNodeptr->next = NULL;
        NewNodeptr->coef = 0;
        NewNodeptr->expn = 0;
        InsertPolyn(P,NewNodeptr);
        P->expn = 1;
    }
    DeleteZeroPolyn(P);
    printf("(");
    PrintPolyn(Pa);
    printf(")*(");
    PrintPolyn(Pb);
    printf(") = ");
    PrintPolyn(P);
    printf("\n");
}

void PrintPolyn(PolynList P)
{
    PolynNode *Pa=P;
    if(Pa->next) 
    {
        Pa=Pa->next;
        if(Pa->expn!=0)
        {
            printf("%.1fx^%d",Pa->coef,Pa->expn);
        }
        else
            printf("%.1f",Pa->coef);
    }
	while(Pa->next){
        Pa=Pa->next;
        if(Pa->expn!=0)
        {
            if(Pa->coef >0) printf("+%.1fx^%d",Pa->coef,Pa->expn);
            else printf("-%.1fx^%d",-Pa->coef,Pa->expn);
        }
        else {
            if(Pa->coef >0) printf("+%.1f",Pa->coef);
            else printf("-%.1f",-Pa->coef);
        }
            
		}
}
void DeleteZeroPolyn(PolynList P){
    PolynNode * Pa = P;
    while(Pa->next){
        if(Pa->next->coef == 0 && P->expn > 1){
            PolynNode * Pt = Pa->next;
            Pa->next = Pt->next;
            free(Pt);
            P->expn -=1; 
        }
        if(Pa->next->coef == 0 && P->expn == 1){
            Pa->next->expn = 0;
        }
    }
}


int Cmp(int m,int n)
{
    if(m==n)  return 0;
    else if(m>n)  return 1;
    else return -1;
}