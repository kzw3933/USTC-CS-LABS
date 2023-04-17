#include<stdio.h>
#include<stdlib.h>
struct  PolynNode{
	float coef;
	int expn;
	struct PolynNode *next;
};
void InsertPolyn(struct PolynNode *P,struct PolynNode *Node);
void CreatePolyn(struct PolynNode **P);
void PrintPolyn(struct PolynNode* P );
void AddPolyn(struct PolynNode* Pa,struct PolynNode *Pb);
void SubPolyn(struct PolynNode *Pa,struct PolynNode *Pb );
void AddOrSubPolyn(struct PolynNode* P1,struct PolynNode *P2);
int cmp(int m,int n);

int main(){
	struct PolynNode *P1,*P2;
	CreatePolyn(&P1);

	CreatePolyn(&P2);

	AddOrSubPolyn(P1,P2);

}
void CreatePolyn(struct PolynNode **P){
	 *P = (struct PolynNode *)malloc(sizeof(struct PolynNode));
	(*P)->next = NULL;(*P)->expn = 0;
	char c;
	struct PolynNode *NewNode;
	while(true){
	NewNode = (struct PolynNode *)malloc(sizeof(struct PolynNode));  
	scanf("%f %d",&NewNode->coef,&NewNode->expn);            
	NewNode->next = NULL;
	InsertPolyn(*P,NewNode);
	c=getchar();
	if(c=='\n') break;	
}
}
void AddOrSubPolyn(struct PolynNode* Pa,struct PolynNode *Pb){
	int i;
	scanf("%d",&i);
	if(i==0) AddPolyn(Pa,Pb);
	if(i==1) SubPolyn(Pa,Pb);
}
void AddPolyn(struct PolynNode* Pa,struct PolynNode *Pb){
	struct PolynNode *ha=Pa;
	struct PolynNode *hat=ha->next;
	struct PolynNode *hbt=Pb->next;

	while(hat&&hbt){
	switch(cmp(hat->expn,hbt->expn)){
	case 0: hat->coef += hbt->coef;
		Pb->next = hbt->next;
		free(hbt);
		Pb->expn-=1;
		hbt=Pb->next;
		if(hat->coef ==0&&Pa->expn>1) {ha->next=hat->next;free(hat);hat=ha->next;Pa->expn-=1;}
		else if(hat->coef ==0&&Pa->expn==1){Pa->next->expn=0;}
		else {
		
		ha=ha->next;hat=ha->next;}
		break;
	case 1: ha=ha->next;hat=hat->next;break;
	
	case -1:Pb->next =hbt->next;hbt->next = hat;ha->next=hbt;hbt = Pb->next;ha=ha->next;Pa->expn+=1;Pb->expn-=1;break;
}
	
}
if(hbt) {ha->next = hbt; Pa->expn+=Pb->expn;Pb->next=NULL;Pb->expn=0;}  
PrintPolyn(Pa);

}

void SubPolyn(struct PolynNode *Pa,struct PolynNode *Pb ){
	struct PolynNode *hb =Pb;
	while(hb->next) {hb->next->coef = -1*hb->next->coef;hb=hb->next;}
	AddPolyn(Pa,Pb);
}

int cmp(int m,int n){
if(m==n)  return 0;
else if(m>n)  return 1;
else return -1;}

void PrintPolyn(struct PolynNode* P ){
	struct PolynNode *Pa=P;
	while(Pa=Pa->next){
		printf("%.1f %d ",Pa->coef,Pa->expn);
		}}
		
		
		
void InsertPolyn(struct PolynNode *P,struct PolynNode *Node){
	struct PolynNode *Pa = P;
	if(Pa->next){
	while(Pa->next &&Pa->next->expn > Node->expn ) Pa=Pa->next;
	if(Pa->next&&Pa->next->expn == Node->expn) Pa->next->coef+=Node->coef;
	else if(Pa->next&&Pa->next->expn < Node->expn) {Node->next = Pa->next;
		Pa->next = Node;P->expn+=1;}
}
 
	if(!(Pa->next)) {Node->next = Pa->next;Pa->next = Node;P->expn+=1;} 
}

