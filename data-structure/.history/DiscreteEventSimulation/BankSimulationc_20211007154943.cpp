#include <stdio.h>
#include <stdlib.h>
typedef struct
{
    int Occurtime;
    int type; //0代表客户到达，1代表客户离开
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

void OpenForDay()
{
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
void TimeDrived()
{
    printf("bdxf\n");
}
void CloseForDay()
{
    printf("szdgh\n");
}
