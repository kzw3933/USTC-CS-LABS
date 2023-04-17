#include<stdio.h>
#include<stdlib.h>
//定义必要的数据类型
typedef  enum EventType {EleStOff,EleAriv,PassAriv,PassDepart} EventType;
typedef enum State {GoingUp,GoingDown,Idle} State;
typedef struct Elevator{
   int  floor;          //电梯的当前位置
    int D1;                 //值为非0时，有人在进入或离开电梯
    int D2;                 //如果电梯在某层等候300t以上
    int D3;                    //值为非0时，电梯门开着但没人上电梯
    State state;            //电梯的状态
    int CallCar[5];
}Elevator;
typedef struct Passenger{
    int  infloor;
   int  outfloor;
    int GiveupTime;   
}Passenger;
typedef struct Buildings{
    int CallUp[5];          
    int CallDown[5];     //分别对应每层楼的上升按钮和下降按钮是否被按亮
}Buildings;
typedef struct Event{
    int OccurTime;
    EventType type;        //事件类型
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
Buildings Building;
EventList ev;
Event en;
int CloseTime = 720;
PasserQuene q[5];

 
//声明相关的函数

void InitService();
void ElevatorSimulation();
void CloseService();         //流程函数

void InitEventList(EventList *elptr);
void OrderInsert(Event entmp); 
int GetDelCurEvent(Event *enptr);
void HandleCurEvent(Event entmp);
void HandlePassAriv();
void HandlePassDepart();
void HandleEleAriv();
void HandleEleStartOff();


void PasserArrival(Event entmp);
void PasserDeparture(Event entmp);
void ElevatorService();


void InitQuene(PasserQuene *q);
void EnQuene(PasserQuene * q,Passenger passer);

void RandomItem(int *InFloor,int *OutFloor,int *GiveupTime,int *InterTime);     //随机数函数

//主函数
int main(){
    printf("-----------------------------电梯模拟服务即将开始----------------------------\n");
    InitService();
    ElevatorSimulation();
    CloseService();
}
//流程相关函数
void InitService(){
    elevator.floor = 1;
    elevator.state = Idle;
    elevator.D1 = 0;       //为0，除非人们正在上下电梯
    elevator.D2 = 1;        //为0，如果电梯已经在某层等待300t以上
    elevator.D3 = 0;        //为0，除非电梯门在开着，但无人进出
    InitEventList(&ev);
    //各层及其队列初始化，电梯初始化
    for(int i=0;i<5;i++){
        elevator.CallCar[i]=0;   
        Building.CallDown[i]=0;
        Building.CallUp[i]=0;
        InitQuene(&q[i]);
        }
    //定义第一个乘客到来事件    
    en.OccurTime= 0;
    en.type = PassAriv;
    OrderInsert(en);
}
void ElevatorSimulation(){
    while(true){
        GetDelCurEvent(&en);
        HandleCurEvent(en);
    }
}
void CloseService(){
    printf("电梯模拟服务结束\n");
}



//事件表及事件处理相关函数
void InitEventList(EventList *elptr){
    *elptr = (EventNode*)malloc(sizeof(EventNode));
    (*elptr)->next = NULL;       //初始化事件表，创建头结点
}
void OrderInsert(Event entmp){
    EventNode * enptr = (EventNode*)malloc(sizeof(EventNode));
    enptr->event = entmp;
    enptr->next = NULL;
    EventList elptr = ev;
    while(elptr->next && elptr->next->event.OccurTime <entmp.OccurTime)  elptr = elptr->next;
    enptr->next = elptr->next;
    elptr->next = enptr;
}
int GetDelCurEvent(Event *enptr){
    if(ev->next){
          *enptr = ev->next->event;
          EventNode * evtmp = ev->next;
          ev->next = evtmp->next;
          free(evtmp);
          return 1;

    }
    else return 0;          //获取下一个待处理的事务,并从事件表中将其删除
}


void HandleCurEvent(Event entmp){
        switch(entmp.type){
        case PassAriv: HandlePassAriv();break;
        case PassDepart: HandlePassDepart();break;
        case EleAriv:HandleEleAriv();break;
        case EleStOff: HandleEleStartOff();break;
        default: break;
    }  
}

void HandlePassAriv(){
    //进入电梯，为下一人的出现做准备
    Passenger passer;
    Event entmp;
    int InterTime;
    RandomItem(&passer.infloor,&passer.outfloor,&passer.GiveupTime,&InterTime);
    entmp.OccurTime = en.OccurTime + InterTime;
    entmp.type = PassAriv;
    OrderInsert(entmp);
    EnQuene(&q[passer.infloor], passer);
    //按电梯并等候,此时分三种情况讨论
    if(passer.infloor == elevator.floor){
        if(elevator.D2 == 0)   
    }

}



void HandlePassDepart();
void HandleEleAriv();
void HandleEleStartOff();





















//队列相关函数
void InitQuene(PasserQuene *qtr){
    (*qtr).front = (PasserNode*)malloc(sizeof(PasserNode));
    (*qtr).front->next = NULL;
    (*qtr).rear =  (*qtr).front;
    
}
void EnQuene(PasserQuene * q,Passenger passer){
    PasserNode* temp = (PasserNode*) malloc(sizeof(PasserNode));
    temp->passer = passer;
    temp->next = NULL;
    (*q).rear->next = temp;
    (*q).rear = temp;
}


//随机数函数

void RandomItem(int *InFloor,int *OutFloor,int *GiveupTime,int *InterTime){
    *InFloor = rand()%5 ;
    *OutFloor = rand()%5 ;
    while(*InFloor == *OutFloor)   *OutFloor = rand()%5 ;
    *GiveupTime = rand()%40+10;
    *InterTime = rand()%10 + 4;
}

