//定义需要的数据类型
typedef struct PolynNode{
	float coef;
	int expn;
	struct PolynNode *next;
}PolynNode,*PolynList;

//声明必要的处理函数
void CreatePolyn(struct PolynNode **P);

//主函数
int main(){
	struct PolynNode *P1,*P2;
	CreatePolyn(&P1);

	CreatePolyn(&P2);

	AddOrSubPolyn(P1,P2);

}