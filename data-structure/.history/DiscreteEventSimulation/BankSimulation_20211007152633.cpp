#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//定义必要的数据类型
typedef struct {
    int Occurtime;
    int type;      //0代表客户到达，1代表客户离开       
}Event;    //定义事件类型
typedef struct EventNode{
    Event event;
    struct EventNode *next;    
} EventNode,*EventList;   //定义事件表节点
typedef struct {
    int ArrivalTime;   //到达时间
    int Duration;    //办理事务要花的时间
    int money;         //交易量
    int type;    //事务类型，0为存钱，1为取钱
}Customer;    //定义客户类型
typedef struct QNode{
    Customer customer;
    struct QNode *next;
} QNode,*QuenePtr;   //定义队列节点
typedef struct {
    QuenePtr front;
    QuenePtr rear;
}QueneList;   //定义队列类型



//银行事务的模拟

//1.每天开始初始化服务，默认此时第一个客户到达
    int TimeNow;                   //定义一个计时器
    int CloseTime;                  //银行服务结束时间
    int TotalTime; 
    int CustomerNum; 
    int TotalMoney = 100000;            
    QueneList q[2];
    EventList ev;
    Event en;


void BankSimulation();
void OpenForDay();
void TimeDrived();
void CloseForDay();

void InitEventList(EventList *ev);
void OrderInsert(EventList ev,Event entmp);
int  GetCurEvent(Event *enptr);
void DelFirstEvent();

void InitQuene(QueneList *q);
void DelQuene(QueneList *q,Customer* customerptr);
void EnQuene(QueneList *q,Customer customer);
void GetQHead(QueneList q,Customer *customerptr);
int GetQueneNum(QueneList q);

void CustomerArrived(Event entmp);
void CustomerDeparture(Event entmp);
void SvMoService(Customer customer);
void GtMonService(Customer customer);

void GUIForBankSimulation(Event entmp);
void Random(int* durtime,int* money,int* type,int* intertime);

int main(){

    printf("欢迎进入银行服务模拟系统\n");
    BankSimulation();
    return 0;
}

void BankSimulation(){
    OpenForDay();
    TimeDrived();
    CloseForDay();
    
}
void OpenForDay(){
    printf("------------------银行服务即将开始运行--------------\n");
    printf("请输入模拟系统服务结束时间：\n");
    scanf("%d",&CloseTime);
    TotalTime = 0;
    CustomerNum = 0;
    InitEventList(&ev);          //初始化事件表
    TimeNow = 0;
    en.Occurtime = TimeNow;
    en.type = 0;                //设置第一个到达的顾客
    OrderInsert(ev,en);
    int i=0;
    for(;i<2;i++)    InitQuene(&(q[i]));     //初始化队列
}
void TimeDrived(){
    while(TimeNow < CloseTime){
        if(GetCurEvent(&en)){           //获取事件表中下一个待处理的事件
           
            if(TimeNow == en.Occurtime){
            DelFirstEvent();          //如果到达下一个待处理的事件的时间，取出处理
            switch(en.type){
                case 0:CustomerArrived(en);break;
                case 1:CustomerDeparture(en);break;
                }
            
            }
      }
        GUIForBankSimulation(en); 
      if(!GetCurEvent(&en)||en.Occurtime!=TimeNow) {
          TimeNow++;
      }  
    }
}
void CloseForDay(){
    printf("本次银行服务模拟结束\n");
    printf("The average time is %f\n",(float)(TotalTime / CustomerNum));
    }

void CustomerArrived(Event entmp){
    CustomerNum++;
    Customer customer;
    Event event;
    int durtime;
    int money;
    int type;
    int intertime;
    Random(&durtime,&money,&type,&intertime);
    event.Occurtime=entmp.Occurtime +intertime;    //下一个到达事件的时间
    event.type=0;                     //下一个到达事件
    if(event.Occurtime <CloseTime)  OrderInsert(ev,event);   //未达到关门时间，插入事件表
    customer.ArrivalTime =entmp.Occurtime;   
    customer.Duration = durtime;
    customer.money = money;
    customer.type=type;
    EnQuene(&q[0],customer);        //将当前到达事件的客户入队,都先排入1队
    if(GetQueneNum(q[0])==1) OrderInsert(ev,{customer.ArrivalTime+customer.Duration,1});          //如果插入客户在排头，即刻处理
}
void CustomerDeparture(Event entmp){
    Customer customer;
    DelQuene(&q[0],&customer);
    switch(customer.type){
        case 0 : SvMoService(customer);break;
        case 1 : GtMonService(customer);break;
    }
    

}
void DelQuene(QueneList *q,Customer* customerptr){
    QNode* qnode =(*q).front;
    *customerptr = qnode->customer;
    (*q).front = qnode->next;
    free(qnode);
}
void GUIForBankSimulation(Event entmp){

     printf("当前处理的事件：%d,%d\n",entmp.Occurtime,entmp.type);
     printf("现在时间%d,队列1人数%d,队列2人数%d\n",TimeNow,GetQueneNum(q[0]),GetQueneNum(q[1]));
}
void InitEventList(EventList *evltr){
    *evltr = (EventNode*)malloc(sizeof(EventNode));
    (*evltr)->next = NULL;       //初始化事件表，创建头结点
}
void InitQuene(QueneList *qtr){
    (*qtr).front = (QNode*)malloc(sizeof(QNode));
    (*qtr).front->next = NULL;
    (*qtr).rear =  (*qtr).front;
    (*qtr).rear->next =NULL;
    
}
void OrderInsert(EventList ev,Event entmp){
    EventList temp1= ev;
    EventList temp2 = (EventNode*)malloc(sizeof(EventNode));
    temp2->event = entmp;              
    temp2->next =NULL;
    if(temp1->next){
        while(temp1->next&&(temp1->next->event.Occurtime <entmp.Occurtime)) temp1 = temp1->next;  //找到插入的位置，按时间升序
    }
    temp2->next =temp1->next;
    temp1->next = temp2;                                    //插入新的事件节点  
}
int GetCurEvent(Event *enptr){
    if(ev->next){
          *enptr = ev->next->event;
          return 1;
    }
    else return 0;          //获取下一个待处理的事务
}
void DelFirstEvent(){
    if(ev->next)  {
        EventNode* temp=ev->next; ev->next = temp->next;free(temp);    //从时间表中删除要处理的事件
        }
}
int GetQueneNum(QueneList q){
    int i=0;
    while((q.front != q.rear)) {
        i++; 
        q.front = q.front->next;
        }
    return i;
}
void EnQuene(QueneList* q,Customer customer){
    QNode* temp = (QNode*) malloc(sizeof(QNode));
    temp->customer = customer;
    temp->next = NULL;
    (*q).rear->next = temp;
    (*q).rear = temp;
}

void SvMoService(Customer customer){
    int moneysignal = TotalMoney;
    TotalMoney +=customer.money;
    while(GetQueneNum(q[1])){
        Customer customertmp;
        DelQuene(&q[1],&customertmp);
        if(customertmp.money <=TotalMoney){
            TotalMoney -= customertmp.money;
            TotalTime += TimeNow - customertmp.ArrivalTime;
        }
        else EnQuene(&q[1],customertmp);
        if(TotalMoney <=moneysignal ) break;
    }
    if(GetQueneNum(q[0])) {
        Event entmp;
        Customer customertmp;
        GetQHead(q[0],&customertmp);
        entmp.Occurtime = TimeNow+customertmp.Duration;
        entmp.type = 1;
        OrderInsert(ev,entmp);
    }



}
void GtMonService(Customer customer){      //处理借钱的服务
    if(customer.money <=TotalMoney ){
        TotalMoney -= customer.money;
        TotalTime += TimeNow - customer.ArrivalTime;
    }
    else {
        EnQuene(&q[1],customer);
    }
    
    if(GetQueneNum(q[0])) {
        Event entmp;
        Customer customertmp;
        GetQHead(q[0],&customertmp);
        entmp.Occurtime = TimeNow+customertmp.Duration;
        entmp.type = 1;
        OrderInsert(ev,entmp);
    }


}
void Random(int* durtime,int* money,int* type,int* intertime){
    srand((unsigned)time(NULL));
    *durtime = rand()%10+1;    //默认处理一件事务不超过20分钟,最少5分钟
    *money = rand() %20000 +1000;   //默认处理事务的钱数不超过2000，最少200;
    *type = rand() %2;   //
    *intertime = rand()%20 +5; // 两人来的间隔至少5分钟
    printf("%d %d %d %d \n",*durtime,*money,*type,*intertime);
}
void GetQHead(QueneList q,Customer *customerptr){
    *customerptr = q.rear->customer;
}