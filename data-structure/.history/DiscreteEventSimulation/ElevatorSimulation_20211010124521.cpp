#include<stdio.h>
#include<stdlib.h>
//定义必要的数据类型
typedef  enum EventType {EleCheck,EleAriv,EleStUp,EleStDown,EleStay,EleBackOne,EleOpen,EleClose,EleOpenOver,EleCloseOver,PassAriv,PassGetUpOver,PassGetDownOver,PassGiveUp} EventType;
typedef enum State {GoingUp,GoingDown,Idle,UnKnown} State;
typedef struct PasserStack{
        Passenger * base;
        Passenger * top;
        int StackSize;                 //当前最大容量
}PasserStack;
typedef struct Elevator{
   int  floor;          //电梯的当前位置
    int D1;                 //值为非0时，有人在进入或离开电梯
    int D2;                 //如果电梯在某层等候300t以上
    int D3;                    //值为非0时，电梯门开着但没人上电梯
    State statebefore;           //电梯的上一个状态
    State state;            //电梯的状态
    State statenext;        //电梯的下一个状态
    int CallCar[5];
    PasserStack passerstack[5];
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
    int floor;              //事件发生的楼层,-1,0,1,2,3,4 ;-1代表此处无此标记
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
int TimeOver = 500;         //电梯服务结束时间500-1000
PasserQuene queneUp[5],queneDown[5];

 
//声明相关的函数

void InitService();
void ElevatorSimulation();
void CloseService();         //流程函数

void InitEventList(EventList *elptr);
void OrderInsert(Event entmp); 
int GetDelCurEvent(Event *enptr);
void RegularEventList(EventList *elptr);          //将时间按合理的处理顺序排列

void HandleCurEvent(Event entmp);
void ShowEvent(Event entmp);

//乘客事件
void HandlePassAriv(Event entmp);     //优先等级更高，在同时刻发生的事件中优先处理   
void HandlePassGiveUp(Event entmp);     //优先等级更低，在同时刻发生的事件中最次处理    
void HandlePassGetUpOver(Event entmp);          
void HandlePassGetDownOver(Event entmp);         

//电梯事件
void HandleEleAriv(Event entmp);        
void HandleEleStUp(Event entmp);           
void HandleEleBackOne(Event entmp);        
void HandleEleStDown(Event entmp);         
void HandleEleStay(Event entmp);
void HandleEleCheck(Event entmp);       
void HandleEleOpen(Event entmp);
void HandleEleClose(Event entmp);
void HandleEleOpenOver(Event entmp);
void HandleEleCloseOver(Event entmp);


//队列相关函数
void InitQuene(PasserQuene *q);
void EnQuene(PasserQuene * q,Passenger passer);
void DelQuene(PasserQuene * q,Passenger *passer);
void DelQueneNode(PasserQuene *q, int GiveUpTime);



//栈相关函数
void InitPasserStack(PasserStack * passerstack);
void StackPush(PasserStack * passerstack,Passenger passertmp);
void StackPop(PasserStack * passerstack,Passenger* passertmp);
int EmptyStack(PasserStack  passerstack);

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
    elevator.state = UnKnown;
    elevator.D1 = 0;       //为0，除非人们正在上下电梯
    elevator.D2 = 1;        //为0，如果电梯已经在某层等待300t以上
    elevator.D3 = 0;        //为0，除非电梯门在开着，但无人进出
    InitEventList(&ev);
    //各层及其队列初始化，电梯初始化
    for(int i=0;i<5;i++){
        elevator.CallCar[i]=0;   
        Building.CallDown[i]=0;
        Building.CallUp[i]=0;
        InitQuene(&queneUp[i]);
        InitQuene(&queneDown[i]);
        InitPasserStack(&elevator.passerstack[i]);
        }
    //定义第一个乘客到来事件    
    en.OccurTime= 0;
    en.type = PassAriv;
    en.floor = -1;
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
        case PassAriv: HandlePassAriv(entmp);break;
        case PassGetUpOver: HandlePassGetUpOver(entmp);break;
        case PassGetDownOver:HandlePassGetDownOver(entmp);break;
        case PassGiveUp: HandlePassGiveUp(entmp);break;
        case EleAriv:HandleEleAriv(entmp);break;
        case EleStUp: HandleEleStUp(entmp);break;
        case EleStDown: HandleEleStDown(entmp);break;
        case EleCheck: HandleEleCheck(entmp);break;
        case EleBackOne:HandleEleBackOne(entmp);break;
        case EleOpen: HandleEleOpen(entmp);break;
        case EleClose: HandleEleClose(entmp);break;
        case EleOpenOver: HandleEleOpenOver(entmp);break;
        case EleCloseOver: HandleEleCloseOver(entmp);break;
        default: break;
    }  
}

//乘客事件处理
void HandlePassAriv(Event entmp){
    //进入电梯，为下一人的出现做准备
    Passenger passer;
    Event entmpNew1,entmpNew2;
    int InterTime;
    RandomItem(&passer.infloor,&passer.outfloor,&passer.GiveupTime,&InterTime);
    entmpNew1.OccurTime = en.OccurTime + InterTime;
    entmpNew2.OccurTime = en.OccurTime+passer.GiveupTime;
    entmpNew1.type = PassAriv;
    entmpNew2.type = PassGiveUp;
    entmpNew1.floor = -1;
    entmpNew2.floor = passer.infloor;
    OrderInsert(entmpNew1);
    OrderInsert(entmpNew2);
    if(passer.outfloor > passer.infloor)
        EnQuene(&queneUp[passer.infloor], passer);     //无论何种情况，乘客到达后先入队
    else EnQuene(&queneDown[passer.infloor], passer);
    if(passer.infloor == elevator.floor){
        //电梯在本层,此时分三种情况讨论
        if(elevator.state == Idle){
            if(elevator.D2 == 0)   {          //此时电梯服务中止，需要设置一个电梯事件
                elevator.D2 =1;                 
                if(passer.infloor < passer.outfloor)    Building.CallUp[passer.infloor] = 1;
                else Building.CallDown[passer.infloor] = 1;
            Event entmpNew3 ;
            entmpNew3.OccurTime = en.OccurTime;    
            entmpNew3.type = EleOpen;
            entmpNew3.floor =elevator.floor;
            OrderInsert(entmpNew3);
            }
            if(elevator.D3==1)  ;                            //电梯正开着，但无人上,插入对列即可
            if(elevator.D1 ==1)  ;                          //有人正上下电梯，插入队列即可
        }
       
    }
    else {
        if(passer.infloor < passer.outfloor)    Building.CallUp[passer.infloor] = 1;
        else Building.CallDown[passer.infloor] = 1;
    }

}

void HandlePassGiveUp(Event entmp){           //优先级最低,此时无其他事件同时发生
        if(entmp.type == PassGiveUp) {
            DelQueneNode(&queneDown[entmp.floor],entmp.OccurTime);
            DelQueneNode(&queneUp[entmp.floor],entmp.OccurTime);
        } 
}
void HandlePassGetUpOver(Event entmp){
    Passenger passertmp;
    if(elevator.state == GoingUp)
        DelQuene(&q[entmp.floor],&passertmp);
    StackPush(&elevator.passerstack[passertmp.outfloor],passertmp);
}
void HandlePassGetDownOver(Event entmp){
    Passenger passertmp;
    StackPop(&elevator.passerstack[passertmp.outfloor],&passertmp);
    
}

//电梯事件处理
void HandleEleAriv(Event entmp){
    if(entmp.type == EleAriv){
        elevator.state = elevator.statenext;
        elevator.statenext = DecideNextState(entmp.floor);
        elevator.floor = entmp.floor;
        Event entmpNew = {entmp.OccurTime,EleOpen,entmp.floor};
        OrderInsert(entmpNew);
        elevator.statenext = Idle;
    }

}     
void HandleEleOpen(Event entmp){
    if(entmp.type == EleOpen){
        Event entmpNew = {entmp.OccurTime,EleOpenOver,entmp.floor};
        OrderInsert(entmpNew);
    }
} 
void HandleEleOpenOver(Event entmp){
    
    if(entmp.type == EleOpenOver){
        //先上后下
        int TimeNow = entmp.OccurTime;
        Event entmpNew1 =   {entmp.OccurTime+40,EleCheck,entmp.floor}; 
        OrderInsert(entmpNew1);
        if(elevator.CallCar[elevator.floor] == 1){
        while(!EmptyStack(elevator.passerstack[entmp.floor])){
            Event entmpNew2 = {TimeNow+25,PassGetDownOver,entmp.floor};
            OrderInsert(entmpNew2);
            TimeNow+=25;
        }
        elevator.D1 = 1;
    }
        if(elevator.statenext != GoingUp ) 
    }


}
void HandleEleStUp(Event entmp);           
void HandleEleBackOne(Event entmp);        
void HandleEleStDown(Event entmp);         

void HandleEleCheck(Event entmp);       

void HandleEleClose(Event entmp);
void HandleEleOpenOver(Event entmp);
void HandleEleCloseOver(Event entmp);






















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
void DelQuene(PasserQuene * q,Passenger *passer){
    PasserNode * passernode = (*q).front;
    *passer = passernode->next->passer;
    (*q).front = passernode->next;
    free(passernode);
}

//随机数函数

void RandomItem(int *InFloor,int *OutFloor,int *GiveupTime,int *InterTime){
    *InFloor = rand()%5 ;
    *OutFloor = rand()%5 ;
    while(*InFloor == *OutFloor)   *OutFloor = rand()%5 ;
    *GiveupTime = rand()%40+10;
    *InterTime = rand()%10 + 4;
}

