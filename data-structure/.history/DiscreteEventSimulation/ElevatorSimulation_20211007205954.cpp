#include<stdio.h>
#include<stdlib.h>
//定义必要的数据类型
typedef enum Floor {Floor0,Floor1,Floor2,Floor3,Floor4} Floor;
typedef enum State {GoingUp,GoingDown,Idle} State;
typedef struct Elevator{
    Floor floor;          //电梯的当前位置
    int D1;                 //值为非0时，有人在进入或离开电梯
    int D2;                 //如果电梯在某层等候300t以上
    int D3;                    //值为非0时，电梯门开着但没人上电梯
    State state;            //电梯的状态
    int CallCar[5];
}Elevator;
typedef struct Passenger{
    Floor infloor;
    Floor outfloor;
    int GiveupTime;
    int InterTime;    //距下一个人到达的时间间隔
}Passenger;
typedef struct Buildings{
    int CallUp[5];          
    int CallDown[5];     //分别对应每层楼的上升按钮和下降按钮是否被按亮
}Buildings;
typedef struct Event{
    int ArrivalTime;
    int type;         //0代表到达事件，1为离开事件
}Event;
typedef struct EventNode{
   Event event;              
    struct EventNode *next;
}EventNode,*EventList;
typedef struct PasserNode{
    Passenger passer;
    struct PasserNode *next;
}PasserNode;
typedef struct PasserQuene{
    PasserNode *front;
    PasserNode *rear;
}PasserQuene;

//定义必要的全局变量
Elevator elevator;
Passenger passer;
Buildings Building;
EventList ev;
Event en;
int TimeNow = 0;
PasserQuene q[5];

 
//声明相关的函数

void InitService();
void ElevatorSimulation();
void CloseService();         //流程函数

void InitEventList(EventList *elptr);
void OrderInsert(Event entmp); 




void InitQuene(PasserQuene *q);

//主函数
int main(){
    printf("-----------------------------电梯模拟服务即将开始----------------------------\n");
    InitService();
    ElevatorSimulation();
    CloseService();
}
//流程相关函数
void InitService(){
    elevator.floor = Floor1;
    elevator.state = Idle;
    elevator.D1 = 0;
    elevator.D2 = 1;
    elevator.D3 = 0;
    InitEventList(&ev);
    for(int i=0;i<5;i++){
        elevator.CallCar[i]=0;
        Building.CallDown[i]=0;
        Building.CallUp[i]=0;
        InitQuene(&q[i]);
        }
    en.ArrivalTime = 0;
    en.type =0;
    OrderInsert(en);
}
void ElevatorSimulation(){
    
}
void CloseService(){
    printf("电梯模拟服务结束\n");
}



//事件表相关函数
void InitEventList(EventList *elptr){
    *elptr = (EventNode*)malloc(sizeof(EventNode));
    (*elptr)->next = NULL;       //初始化事件表，创建头结点
}
void OrderInsert(Event entmp){
    EventNode * enptr = (EventNode*)malloc(sizeof(EventNode));
    enptr->event = entmp;
    enptr->next = NULL;
    EventList elptr = ev;
    while(elptr->next && elptr->next->event.ArrivalTime <entmp.ArrivalTime)  elptr = elptr->next;
    enptr->next = elptr->next;
    elptr->next = enptr;
}













//队列相关函数
void InitQuene(PasserQuene *qtr){
    (*qtr).front = (PasserNode*)malloc(sizeof(PasserNode));
    (*qtr).front->next = NULL;
    (*qtr).rear =  (*qtr).front;
    
}