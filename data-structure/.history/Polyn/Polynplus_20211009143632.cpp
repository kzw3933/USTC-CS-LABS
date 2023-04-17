#include<stdio.h>
#include<stdlib.h>
//定义需要的数据类型
typedef struct PolynNode{
	float coef;
	int expn;
	struct PolynNode *next;
}PolynNode,*PolynList;

//定义相关的功能函数



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
			case 0:	return 0;
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
        }
    }
}