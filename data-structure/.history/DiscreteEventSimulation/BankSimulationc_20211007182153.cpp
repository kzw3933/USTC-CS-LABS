#include <stdio.h>
#include <stdlib.h>
typedef struct
{
    int Occurtime;
    int type; //0代表客户到达，1-2代表客户要离开的的队号
} Event;      //定义事件类型
typedef struct EventNode
{
    Event event;
    struct EventNode *next;
} EventNode, *EventList; //定义事件表节点
typedef struct
{
    int ArrivalTime; //到达时间
    int Duration;    //办理事务要花的时间
    int money;       //交易量
    int type;        //事务类型，0为存钱，1为取钱
} Customer;          //定义客户类型
typedef struct QNode
{
    Customer customer;
    struct QNode *next;
} QNode, *QuenePtr; //定义队列节点
typedef struct
{
    QuenePtr front;
    QuenePtr rear;
} QueneList; //定义队列类型
typedef struct {
    int durtime;
    int money;
    int type;
    int intertime;
} RandomItem;       //定义随机数类型

//定义必要的全局变量
int TimeNow;   //定义一个计时器
int CloseTime; //银行服务结束时间
int TotalTime;
int CustomerNum;
int TotalMoney = 100000;
QueneList q[3]; //三个队列，0,1为正常处理队，2为等候处理队
EventList ev;
Event en;

//声明功能函数
void BankSimulation();
void OpenForDay();  //初始化
void TimeDrived();  //服务进行
void CloseForDay(); //服务结束

void InitEventList(EventList *ev);
void OrderInsert(EventList ev, Event entmp);
int GetCurEvent(Event *enptr);
void DelFirstEvent();

void InitQuene(QueneList *q);
void DelQuene(QueneList *q, Customer *customerptr);
void EnQuene(QueneList *q, Customer customer);
void GetQHead(QueneList q, Customer *customerptr);
int GetQueneNum(QueneList q);
int GetBestQuene();

void CustomerArrived(Event entmp);
void CustomerDeparture(Event entmp);
void SvMoService(Customer customer,int choice);
void GtMonService(Customer customer,int choice);

RandomItem Random();

int main()
{
    printf("欢迎进入银行服务模拟系统\n");
    BankSimulation();
    return 0;
}

void BankSimulation()
{
    OpenForDay();  //初始化
    TimeDrived();  //服务进行
    CloseForDay(); //服务结束
}

//服务流程函数
void OpenForDay()
{
    printf("------------------银行服务即将开始运行--------------\n");
    printf("请输入模拟系统服务结束时间：\n");
    scanf("%d", &CloseTime);
    TotalTime = 0;
    CustomerNum = 0;
    InitEventList(&ev); //初始化事件表
    TimeNow = 0;
    en.Occurtime = TimeNow;
    en.type = 0; //设置第一个到达的顾客
    OrderInsert(ev, en);
    int i = 0;
    for (; i < 3; i++)
        InitQuene(&(q[i])); //初始化队列
}
void TimeDrived()
{
    while (TimeNow < CloseTime)
    {
        if (GetCurEvent(&en) && (TimeNow == en.Occurtime))
        {
            DelFirstEvent();
            switch (en.type)
            {
            case 0:
                CustomerArrived(en);
                break;
            default:
                CustomerDeparture(en);
                break;
            }
        }

        else
            TimeNow++;
    }
}

void CloseForDay()
{
    printf("本次银行服务模拟结束\n");
    printf("The average time is %f\n", (float)(TotalTime / CustomerNum));
}

//事件表相关函数
void InitEventList(EventList *evltr)
{
    *evltr = (EventNode *)malloc(sizeof(EventNode));
    (*evltr)->next = NULL; //初始化事件表，创建头结点
}
void OrderInsert(EventList ev, Event entmp)
{
    EventList temp1 = ev;
    EventList temp2 = (EventNode *)malloc(sizeof(EventNode));
    temp2->event = entmp;
    temp2->next = NULL;
    if (temp1->next)
    {
        while (temp1->next && (temp1->next->event.Occurtime < entmp.Occurtime))
            temp1 = temp1->next; //找到插入的位置，按时间升序
    }
    temp2->next = temp1->next;
    temp1->next = temp2; //插入新的事件节点
}
int GetCurEvent(Event *enptr)
{
    if (ev->next)
    {
        *enptr = ev->next->event;
        return 1;
    }
    else
        return 0; //获取下一个待处理的事务
}
void DelFirstEvent()
{
    if (ev->next)
    {
        EventNode *temp = ev->next;
        ev->next = temp->next;
        free(temp); //从时间表中删除要处理的事件
    }
}

//队列相关函数
void InitQuene(QueneList *qtr)
{
    (*qtr).front = (QNode *)malloc(sizeof(QNode));
    (*qtr).front->next = NULL;
    (*qtr).rear = (*qtr).front;
    (*qtr).rear->next = NULL;
}
void EnQuene(QueneList *q, Customer customer)
{
    QNode *temp = (QNode *)malloc(sizeof(QNode));
    temp->customer = customer;
    temp->next = NULL;
    (*q).rear->next = temp;
    (*q).rear = temp;
}
void DelQuene(QueneList *q, Customer *customerptr)
{
    QNode *qnode = (*q).front;
    *customerptr = qnode->customer;
    (*q).front = qnode->next;
    free(qnode);
}
int GetQueneNum(QueneList q)
{
    int i = 0;
    while (q.front != q.rear)
    {
        i++;
        q.front = q.front->next;
    }
    return i;
}
void GetQHead(QueneList q, Customer *customerptr)
{
    *customerptr = q.rear->customer;
}
int GetBestQuene(){
    int m=GetQueneNum(q[0]);
    int n=GetQueneNum(q[1]);
    return (m>n)?1:0;
}

//服务函数
void CustomerArrived(Event entmp)
{
    int choice = GetBestQuene(); //从两个队中选出人最少的队
    CustomerNum++;
    Customer customer;
    Event event;
    RandomItem randitem = Random();
    event.Occurtime = entmp.Occurtime + randitem.intertime; //下一个到达事件的时间
    event.type = 0;                                         //下一个到达事件
    if (event.Occurtime < CloseTime)
        OrderInsert(ev, event); //未达到关门时间，插入事件表
        customer.ArrivalTime = entmp.Occurtime;
        customer.Duration = randitem.durtime;
        customer.money = randitem.money;
        customer.type = randitem.type;
        EnQuene(&q[choice], customer); //将当前到达事件的客户入队,先排入1,2队
    if (GetQueneNum(q[choice]) == 1)
        OrderInsert(ev, {customer.ArrivalTime + customer.Duration, choice+1}); //如果插入客户在排头，即刻处理
}
void CustomerDeparture(Event entmp)
{
    Customer customer;
    int choice = entmp.type;
    DelQuene(&q[choice], &customer);
    switch (customer.type)
    {
    case 0:
        SvMoService(customer,choice);
        break;
    case 1:
        GtMonService(customer,choice);
        break;
    }
}

void SvMoService(Customer customer,int choice)
{
    int moneysignal = TotalMoney;
    TotalMoney += customer.money;
    while (GetQueneNum(q[2]))
    {
        Customer customertmp;
        DelQuene(&q[2], &customertmp);
        if (customertmp.money <= TotalMoney)
        {
            TotalMoney -= customertmp.money;
            TotalTime += TimeNow - customertmp.ArrivalTime;
        }
        else
            EnQuene(&q[2], customertmp);
        if (TotalMoney <= moneysignal)
            break;
    }
    if (GetQueneNum(q[choice]))
    {
        Event entmp;
        Customer customertmp;
        GetQHead(q[choice], &customertmp);
        entmp.Occurtime = TimeNow + customertmp.Duration;
        entmp.type = choice+1;
        OrderInsert(ev, entmp);
    }
}
void GtMonService(Customer customer,int choice)
{ //处理借钱的服务
    if (customer.money <= TotalMoney)
    {
        TotalMoney -= customer.money;
        TotalTime += TimeNow - customer.ArrivalTime;
    }
    else
    {
        EnQuene(&q[2], customer);
    }

    if (GetQueneNum(q[choice]))
    {
        Event entmp;
        Customer customertmp;
        GetQHead(q[choice], &customertmp);
        entmp.Occurtime = TimeNow + customertmp.Duration;
        entmp.type = choice+1;
        OrderInsert(ev, entmp);
    }
}

//随机数函数

RandomItem Random()
{
    RandomItem randitem;
    //srand((unsigned)time(NULL));
    randitem.durtime = rand() % 10 + 1;     //默认处理一件事务不超过20分钟,最少5分钟
    randitem.money = rand() % 20000 + 1000; //默认处理事务的钱数不超过2000，最少200;
    randitem.type = rand() % 2;             //
    randitem.intertime = rand() % 20 + 5;   // 两人来的间隔至少5分钟
    printf("%d %d %d %d \n", randitem.durtime, randitem.money, randitem.type, randitem.intertime);
    return randitem;
}