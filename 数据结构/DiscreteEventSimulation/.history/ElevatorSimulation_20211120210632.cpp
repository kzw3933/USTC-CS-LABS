#include<stdio.h>
#include<stdlib.h>
//1.定义必要的数据类型
typedef  enum EventType {EleArivStay,EleBackOne,EleOpen,EleClose,EleOpenOver,EleCloseOver,EleCheck,EleStUp,EleStDown,EleStUpOneFloor,EleStDownOneFloor,EleBackOneDownOneFloor,EleBackOneUpOneFloor,PassAriv,PassGetUpOver,PassGetDownOver,PassGiveUp} EventType;     //事件类型
//注意
//乘客之间互不干扰，电梯与乘客之间有些事件如果同时发生用计算机程序模拟时，涉及判断，需确定处理的程序中顺序 
//也就是说事件具有不同的优先度,对于同时刻发生的事,要保证: 1. 电梯到达停留，乘客到达,先处理电梯到达停留 2.电梯到达停留，乘客放弃,先处理电梯到达停留 3. 电梯检测，乘客到达，先处理乘客到达  4.电梯关闭结束，乘客到达，先处理乘客到达 5.电梯即将返回1楼等待,乘客到达，先处理乘客到达  6.电梯开门结束，乘客到达，先处理乘客到达 7.乘客上电梯结束，另一个乘客到达，先处理乘客到达 

//电梯状态
typedef enum State {GoingUp,GoingDown,Idle,UnKnown} State;    //四种状态，上升，下降，停留，未知
typedef struct Passenger{
    int  infloor;
    int  outfloor;
    int GiveupTime;
}Passenger;
typedef struct PasserNode{
    Passenger passer;
    struct PasserNode * next;
}PasserNode;
typedef struct PasserQuene{
    PasserNode *front;
    PasserNode *rear;
}PasserQuene;            //乘客队列

#define StackInitSize 10
#define StackIncrease  4                                      //栈的相关参数                                     
typedef struct PasserStack{
        Passenger * base;
        Passenger * top;
        int StackSize;                 //当前最大容量
}PasserStack;

typedef struct Elevator{
    int floor;                 //电梯的当前位置
    int D1;                    //值为1时，有人在进入或离开电梯
    int D2;                    //如果电梯在某层等候300t以上
    int D3;                    //值为1时，电梯门开着但没人上电梯
    State statebefore;         //电梯的上一个状态
    State state;               //电梯的状态
    State statenext;           //电梯的上一个状态
    int CallCar[5];             //5个到达楼层信号
    PasserStack passerstack[5];          //5个乘客栈
}Elevator;

typedef struct Buildings{
    int CallUp[5];          
    int CallDown[5];     //分别对应每层楼的上升按钮和下降按钮是否被按亮,楼层共5层，编号0-4
}Buildings;

typedef struct Event{
    int OccurTime;
    EventType type;        //事件类型
    int floor;              //事件发生的楼层,-1,0,1,2,3,4,其中-1代表此处无此标记
}Event;
typedef struct EventNode{
    Event event;              
    struct EventNode *next;
}EventNode,*EventList;         //事件表




//2.定义必要的全局变量,声明相关的函数

Elevator elevator;
Buildings Building;
EventList ev;
Event en;
int TimeOver = 500;         //电梯服务结束时间500-1000
PasserQuene queneUp[5],queneDown[5];

//主流程函数 
void InitService();
void ElevatorSimulation();
void CloseService();        

//事件表相关函数
void InitEventList(EventList *elptr);
void OrderInsert(Event entmp); 
int  GetCurEvent(Event *enptr);
void DelCurEvent(EventList *EListptr);

//栈相关函数
void InitPasserStack(PasserStack * passerstack);
void StackPush(PasserStack * passerstack,Passenger passertmp);
void StackPop(PasserStack * passerstack,Passenger* passertmp);
int EmptyStack(PasserStack  passerstack);        //栈空返回1，否则返回0

//队列相关函数
void InitQuene(PasserQuene *q);
void EnQuene(PasserQuene * q,Passenger passer);
void DeQuene(PasserQuene * q,Passenger *passer);
int  DelQueneNode(PasserQuene *q, int GiveUpTime);
int  EmptyQuene(PasserQuene q);  


//事件处理相关函数
void HandleCurEvent(Event entmp);
void ShowEvent(Event entmp);

//乘客事件处理函数
void HandlePassAriv(Event entmp);     //优先等级更高，在同时刻发生的事件中优先处理   
void HandlePassGiveUp(Event entmp);     //优先等级更低，在同时刻发生的事件中最次处理    
void HandlePassGetUpOver(Event entmp);          
void HandlePassGetDownOver(Event entmp);         

//电梯事件处理函数
void HandleEleCheck(Event entmp); 
void HandleEleArivStay(Event entmp); 
void HandleEleOpen(Event entmp);
void HandleEleClose(Event entmp);
void HandleEleOpenOver(Event entmp);
void HandleEleCloseOver(Event entmp);

void HandleEleStUp(Event entmp); 
void HandleEleBackOne(Event entmp);        
void HandleEleStDown(Event entmp);  
void HandleEleStDownOneFloor(Event entmp);
void HandleEleStUpOneFloor(Event entmp);
void HandleEleBackOneDownOneFloor(Event entmp);
void HandleEleBackOneUpOneFloor(Event entmp) ;       

//判断下一个要到达的位置
int GetNextFloor(Event entmp);

//判断电梯的下一个状态
State DecideNextState(int floor);

//显示当前状态
void ShowStates();

//随机数函数
void RandomItem(int *InFloor,int *OutFloor,int *GiveupTime,int *InterTime);    

//消除返回1楼事件，一旦电梯服务停止，启动时要保证消除返回1楼事件
void RemoveBackOne(EventList *elptr);


//3.函数实现

//主函数
int main(){
    printf("-----------------------------电梯模拟服务即将开始----------------------------\n");
    InitService();               //初始化服务
    ElevatorSimulation();        //电梯模拟服务开始
    CloseService();              //服务停止
}
//流程相关函数
void InitService(){
    elevator.floor = 1;
    elevator.state = Idle;
    elevator.statebefore = UnKnown;
    elevator.statenext = UnKnown;
    elevator.D1 = 0;       //为0，除非人们正在上下电梯
    elevator.D2 = 1;        //为0，如果电梯已经在某层等待300以上
    elevator.D3 = 0;        //为0，除非电梯门在开着，但无人进出
    InitEventList(&ev);

    for(int i=0;i<5;i++){
        elevator.CallCar[i]=0; 
        InitPasserStack(&elevator.passerstack[i]);  
        Building.CallDown[i]=0;
        Building.CallUp[i]=0;
        InitQuene(&queneUp[i]);
        InitQuene(&queneDown[i]);
        
        }      //各层及其队列初始化，电梯初始化

    en.OccurTime= 0;
    en.type = PassAriv;
    en.floor = -1;           //初始无此属性
    OrderInsert(en);       //定义第一个乘客到来事件 
}
void ElevatorSimulation(){
    
    while(true){
        if(GetCurEvent(&en)){
            DelCurEvent(&ev);
            HandleCurEvent(en);
        }
        else break;   
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
    if(entmp.OccurTime< TimeOver){
        
        enptr->event = entmp;
        enptr->next = NULL;
        EventList elptr = ev;
        while((elptr->next) && (elptr->next->event.OccurTime <entmp.OccurTime))
            elptr = elptr->next;
    enptr->next = elptr->next;
    elptr->next = enptr;
    }   
}
int GetCurEvent(Event *enptr){
    if(ev->next){
          *enptr = ev->next->event;
          return 1;
    }
    else return 0;          //获取下一个待处理的事务
}
void DelCurEvent(EventList *EListptr){
          EventNode * evtmp = (*EListptr)->next;
          EventNode *evtmptemp = evtmp;
          (*EListptr)->next = evtmp->next;
          free(evtmptemp);
}   //从事件表中将第一个事件删除，配合获取当前事件使用


void HandleCurEvent(Event entmp){
        switch(entmp.type){
        case PassAriv:HandlePassAriv(entmp);break;
        case PassGetUpOver: HandlePassGetUpOver(entmp);break;
        case PassGetDownOver:HandlePassGetDownOver(entmp);break;
        case PassGiveUp: HandlePassGiveUp(entmp);break;
        
        case EleArivStay:HandleEleArivStay(entmp);break;
        case EleCheck: HandleEleCheck(entmp);break;
        case EleOpen: HandleEleOpen(entmp);break;
        case EleClose: HandleEleClose(entmp);break;
        case EleOpenOver: HandleEleOpenOver(entmp);break;
        case EleCloseOver: HandleEleCloseOver(entmp);break;

        case EleStUp: HandleEleStUp(entmp);break;
        case EleStDown: HandleEleStDown(entmp);break;
        case EleBackOne:HandleEleBackOne(entmp);break;
       
        case EleStDownOneFloor: HandleEleStDownOneFloor(entmp);break;
        case EleStUpOneFloor: HandleEleStUpOneFloor( entmp);break;
        case EleBackOneDownOneFloor: HandleEleBackOneDownOneFloor(entmp);break;
        case EleBackOneUpOneFloor: HandleEleBackOneUpOneFloor(entmp);break;
        default: break;
    }  
}

void ShowEvent(Event entmp){
    printf("当前时间为: %d,",entmp.OccurTime);
    switch(entmp.type){
        case PassAriv: printf("一位乘客从%d楼进入本服务系统\n",entmp.floor);ShowStates();break;
        case PassGetUpOver:printf("一位乘客从%d楼进入电梯\n",entmp.floor);ShowStates();break;   
        case PassGetDownOver:printf("一位乘客从%d楼离开电梯\n",entmp.floor);ShowStates();break;
        case PassGiveUp:printf("一位乘客从%d楼放弃乘坐电梯\n",entmp.floor);ShowStates();break;
       
        case EleArivStay:printf("电梯到达并开始停留在%d楼\n",entmp.floor);ShowStates();break;
        case EleCheck:printf("电梯在%d楼检测是否还有人上下电梯\n",entmp.floor);ShowStates();break;
        case EleOpen: printf("电梯在%d楼开始开门\n",entmp.floor);ShowStates();break;
        case EleClose: printf("电梯在%d楼开始关门\n",entmp.floor);ShowStates();break;
        case EleOpenOver:printf("电梯在%d楼开门结束\n",entmp.floor);ShowStates();break;
        case EleCloseOver:printf("电梯在%d楼关门结束\n",entmp.floor);ShowStates();break;

        case EleStUp:printf("电梯在%d楼开始上行\n",entmp.floor);ShowStates();break;
        case EleStDown:printf("电梯在%d楼开始下行\n",entmp.floor);ShowStates();break;
        case EleBackOne:printf("电梯在%d楼开始返回1层等候\n",entmp.floor);ShowStates();break;
        case EleStDownOneFloor:printf("电梯下降1层，现位于%d层\n",entmp.floor);ShowStates();break;
        case EleStUpOneFloor:printf("电梯上升1层，现位于%d层\n",entmp.floor);ShowStates();break;
        case EleBackOneDownOneFloor: printf("电梯正返回1层，已到%d层\n",entmp.floor);ShowStates();break;
        case EleBackOneUpOneFloor: printf("电梯正返回1层，已到%d层\n",entmp.floor);ShowStates();break;

        default: break;
    }
}

void ShowStates(){
    int i;
    PasserNode * temptr1;
    PasserNode * temptr2;
    for(i=0;i<5;i++){
       temptr1 = queneUp[i].front;
       printf("现在在%d楼等待上楼的队列:",i);
       while(temptr1->next){
           printf("*");
           temptr1 = temptr1->next;
       }
       printf("           ");
       temptr2 = queneDown[i].front;
       printf("现在在%d楼等待下楼的队列:",i);
       while(temptr2->next){
           printf("*");
           temptr2 = temptr2->next;
       }
       printf("\n");   
    }
}

//乘客事件处理
void HandlePassAriv(Event entmp){                                          //出错
    //进入电梯，为下一人的出现做准备
    EventNode* entmpNewptr = ev;
    //确保电梯到达先处理
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == EleArivStay){
          HandleEleArivStay(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
        else    entmpNewptr = entmpNewptr->next;                               //修改，处理后未删除
    }  
    elevator.statenext = DecideNextState(elevator.floor);
    Passenger passer;
    Event entmpNew1,entmpNew2;
    int InterTime;
    int WaitTime;
    RandomItem(&passer.infloor,&passer.outfloor,&WaitTime,&InterTime);
    entmp.floor = passer.infloor;
    passer.GiveupTime = entmp.OccurTime + WaitTime;                        //找了很久的错误,passer.GiveupTime与entmpNew2的不一致
    entmpNew1.OccurTime = entmp.OccurTime + InterTime;
    entmpNew2.OccurTime = passer.GiveupTime;
    entmpNew1.type = PassAriv;
    entmpNew2.type = PassGiveUp;
    entmpNew1.floor = -1;
    entmpNew2.floor = passer.infloor;
    OrderInsert(entmpNew1);
    OrderInsert(entmpNew2);

    //无论何种情况，乘客到达后先入队
    if(passer.outfloor > passer.infloor)  {
         EnQuene(&queneUp[passer.infloor], passer); 
         Building.CallUp[passer.infloor]=1;
    } 
    else {
        EnQuene(&queneDown[passer.infloor], passer);
        Building.CallDown[passer.infloor] =1;
    }
    
    //根据电梯状态，分为两种情况:电梯服务暂时性停止，电梯正在运行中,仅需对电梯服务暂时性停止情况进行处理
    if(elevator.statenext == UnKnown){
        if(elevator.floor==passer.infloor){
            if((elevator.D1==0)&&(elevator.D3==0)) //电梯在关着，此时电梯服务停止
            {
                Event entmpNew3 ;
                entmpNew3.OccurTime = en.OccurTime;    
                entmpNew3.type = EleOpen;
                entmpNew3.floor =elevator.floor;
                OrderInsert(entmpNew3); 
                RemoveBackOne(&ev);
                elevator.statenext = DecideNextState(elevator.floor);
            }
            else if(elevator.D3==1){                      //门开着，无人
                Event entmpNew4 ;
                entmpNew4.OccurTime = en.OccurTime+25;    
                entmpNew4.type = PassGetUpOver;
                entmpNew4.floor =elevator.floor;
                OrderInsert(entmpNew4); 
            }

        }
        else {
            if((elevator.D1==0)&&(elevator.D3==0)){  //电梯在关着，此时电梯服务停止
                RemoveBackOne(&ev);
                if(entmp.floor > elevator.floor){
                    Event entmpNew4 = {entmp.OccurTime,EleStUp,elevator.floor};
                    OrderInsert(entmpNew4);
                    elevator.statenext = GoingUp;
                }
                else {
                Event entmpNew4 = {entmp.OccurTime,EleStDown,elevator.floor};
                OrderInsert(entmpNew4);  
                elevator.statenext = GoingDown;
                }
            }
        }
    }
    else {
        if(elevator.floor==passer.infloor){
            if(elevator.D3==1){       //电梯开着，无人上
                if(elevator.statenext == GoingUp){
                    if(passer.outfloor > passer.infloor){
                        Event entmpNew4 ;
                        entmpNew4.OccurTime = en.OccurTime+25;    
                        entmpNew4.type = PassGetUpOver;
                        entmpNew4.floor =elevator.floor;
                        OrderInsert(entmpNew4); 
                }
            }
                            if(elevator.statenext == GoingDown){
                    if(passer.outfloor < passer.infloor){
                        Event entmpNew4 ;
                        entmpNew4.OccurTime = en.OccurTime+25;    
                        entmpNew4.type = PassGetUpOver;
                        entmpNew4.floor =elevator.floor;
                        OrderInsert(entmpNew4); 
                }
            }
                

            }
        }
        }
    ShowEvent(entmp);              //显示事件处理情况
    }



void HandlePassGiveUp(Event entmp){           //预置事件，需先判断,此处只需队列中有需离开的让其离开即可
    EventNode* entmpNewptr = ev;
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == EleArivStay){
          HandleEleArivStay(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
     else   entmpNewptr = entmpNewptr->next;
    }  
    if(entmp.type == PassGiveUp){
        if(elevator.floor!=entmp.floor){
            if(DelQueneNode(&queneDown[entmp.floor],entmp.OccurTime)) ;
            else  DelQueneNode(&queneUp[entmp.floor],entmp.OccurTime) ;  
            ShowEvent(entmp);  //显示事件处理情况
            }           //放弃时间已到，电梯仍未到达,乘客放弃
        }        
}
void HandlePassGetUpOver(Event entmp){
    EventNode* entmpNewptr = ev;
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == EleArivStay){
          HandleEleArivStay(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
     else   entmpNewptr = entmpNewptr->next;
    }  
    Passenger passertmp;
    elevator.statenext = DecideNextState(elevator.floor);
    if(elevator.statenext == GoingUp){
        if(!EmptyQuene(queneUp[entmp.floor]))  {
            DeQuene(&queneUp[entmp.floor],&passertmp);
             StackPush(&elevator.passerstack[passertmp.outfloor],passertmp);
              elevator.CallCar[passertmp.outfloor] = 1;
        } 
    }
        
    if(elevator.statenext==GoingDown){
      if(!EmptyQuene(queneDown[entmp.floor])){
             DeQuene(&queneDown[entmp.floor],&passertmp);  
             StackPush(&elevator.passerstack[passertmp.outfloor],passertmp);        //访问栈时为空
              elevator.CallCar[passertmp.outfloor] = 1;
        }
        
    }
    ShowEvent(entmp);     //显示事件处理情况
}
void HandlePassGetDownOver(Event entmp){
    Passenger passertmp;
    StackPop(&elevator.passerstack[passertmp.outfloor],&passertmp);
    ShowEvent(entmp);     //显示事件处理情况
}

//电梯事件处理
void HandleEleArivStay(Event entmp){            //确定下一步的状态
    if(entmp.type == EleArivStay){
        elevator.statebefore = elevator.state;
        elevator.state = elevator.statenext; 
        elevator.statenext = DecideNextState(elevator.floor);       
        elevator.floor = entmp.floor;
        Event entmpNew = {entmp.OccurTime,EleOpen,entmp.floor};
        OrderInsert(entmpNew);
        ShowEvent(entmp);
    }
}     
void HandleEleOpen(Event entmp){
    if(entmp.type == EleOpen){
        Event entmpNew = {entmp.OccurTime+20,EleOpenOver,entmp.floor};
        OrderInsert(entmpNew);
        ShowEvent(entmp);
    }
} 
void HandleEleOpenOver(Event entmp){   
    EventNode* entmpNewptr = ev;
    //确保乘客到达先处理
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == PassAriv){
          HandlePassAriv(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
     else    entmpNewptr = entmpNewptr->next;
    }
    if(entmp.type == EleOpenOver){
        //先上后下
        elevator.D1 = 0;
        elevator.D3 = 1;
        int Time = entmp.OccurTime;
        Event entmpNew1 =   {entmp.OccurTime+40,EleCheck,entmp.floor}; 
        OrderInsert(entmpNew1);
        if(elevator.CallCar[elevator.floor] == 1){
        while(!EmptyStack(elevator.passerstack[entmp.floor])){
            elevator.D1 = 1;
            elevator.D3 = 0;
            Event entmpNew2 = {Time+25,PassGetDownOver,entmp.floor};
            OrderInsert(entmpNew2);
            Time+=25;
        }
        elevator.D3 = 1;
        elevator.D1 = 0;
    }
    elevator.statenext = DecideNextState(elevator.floor);
        if(elevator.statenext== GoingUp) {
            if(!EmptyQuene(queneUp[entmp.floor])){
                Event entmpNew3 = {Time+25,PassGetUpOver,entmp.floor};
                OrderInsert(entmpNew3);
                elevator.D3 = 1;
                elevator.D1 = 0;
            }
        }
        if(elevator.statenext== GoingDown) {
            if(!EmptyQuene(queneDown[entmp.floor])){
                Event entmpNew3 = {Time+25,PassGetUpOver,entmp.floor};
                OrderInsert(entmpNew3);
                elevator.D3 = 1;
                elevator.D1 = 0;
            }
        }
        ShowEvent(entmp);

    }


}
void HandleEleCheck(Event entmp){                      //检查中断
    EventNode* entmpNewptr = ev;
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == PassAriv){
          HandlePassAriv(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
     else   entmpNewptr = entmpNewptr->next;
    }
    elevator.statenext = DecideNextState(elevator.floor);
    if(elevator.statenext == GoingUp){
        if(EmptyQuene(queneUp[entmp.floor])){
            Event entmpNew = {entmp.OccurTime,EleClose,entmp.floor};
            OrderInsert(entmpNew);
        }
        else {
        Event entmpNew = {entmp.OccurTime+40,EleCheck,entmp.floor};
        OrderInsert(entmpNew);
    }
    }
    else if(elevator.statenext == GoingDown){
        if(EmptyQuene(queneDown[entmp.floor])){
            Event entmpNew = {entmp.OccurTime,EleClose,entmp.floor};
            OrderInsert(entmpNew);
        }
        else {
        Event entmpNew = {entmp.OccurTime+40,EleCheck,entmp.floor};
        OrderInsert(entmpNew);
    }
    }

    ShowEvent(entmp);
}       
void HandleEleClose(Event entmp){
    elevator.D1 = 0;
    elevator.D3 = 0;
    elevator.D2 = 1;
    Event entmpNew ={entmp.OccurTime+20,EleCloseOver,entmp.floor};
    OrderInsert(entmpNew);
    ShowEvent(entmp);
}
void HandleEleCloseOver(Event entmp){
    EventNode* entmpNewptr = ev;
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == PassAriv){
          HandlePassAriv(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
     else   entmpNewptr = entmpNewptr->next;
    }
    elevator.statenext = DecideNextState(elevator.floor);
    if(elevator.statenext == GoingUp){
        if(!EmptyQuene(queneUp[entmp.floor])){
            Event entmpNew = {entmp.OccurTime,EleOpen,entmp.floor};
            OrderInsert(entmpNew);
        }
        else{
            Event entmpNew = {entmp.OccurTime,EleStUp,entmp.floor};
            OrderInsert(entmpNew);
        } 
    }
    else if(elevator.statenext == GoingDown){
        if(!EmptyQuene(queneDown[entmp.floor])){
            Event entmpNew = {entmp.OccurTime,EleOpen,entmp.floor};
            OrderInsert(entmpNew);
        }
        else {
            Event entmpNew = {entmp.OccurTime,EleStDown,entmp.floor};
            OrderInsert(entmpNew);
        }
    }
    else {
        Event entmpNew = {entmp.OccurTime+300,EleBackOne,entmp.floor};
        OrderInsert(entmpNew);
    }
    ShowEvent(entmp);
}
void HandleEleStUp(Event entmp){  
    elevator.statebefore = elevator.state;
    elevator.state = elevator.statenext; 
    elevator.statenext = DecideNextState(elevator.floor);
    elevator.CallCar[entmp.floor] = 0;
    Building.CallUp[entmp.floor] = 0;
    Event entmpNew = {entmp.OccurTime+51,EleStUpOneFloor,entmp.floor+1};
    OrderInsert(entmpNew);
    ShowEvent(entmp);
}  
void HandleEleStUpOneFloor(Event entmp){
    EventNode* entmpNewptr = ev;
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == PassAriv){
          HandlePassAriv(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
        else entmpNewptr = entmpNewptr->next;
    }
    int floortmp = GetNextFloor(entmp);
    if(floortmp==entmp.floor){
        Event entmpNew = {entmp.OccurTime+14,EleArivStay,floortmp};
        OrderInsert(entmpNew);
    }
    else {
         elevator.floor = entmp.floor;
        ShowEvent(entmp);
        Event entmpNew = {entmp.OccurTime+51,EleStUpOneFloor,entmp.floor+1};
        OrderInsert(entmpNew);
    }

} 
void HandleEleStDown(Event entmp){
    Building.CallDown[entmp.floor] = 0;
    elevator.statebefore = elevator.state;
    elevator.state = elevator.statenext; 
    elevator.statenext = DecideNextState(elevator.floor);
    elevator.CallCar[entmp.floor] = 0;Building.CallDown[entmp.floor] = 0;
    Event entmpNew = {entmp.OccurTime+61,EleStDownOneFloor,entmp.floor+1};
    OrderInsert(entmpNew);
    ShowEvent(entmp);
}   
void HandleEleStDownOneFloor(Event entmp){
   
    EventNode* entmpNewptr = ev;
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == PassAriv){
          HandlePassAriv(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
        else entmpNewptr = entmpNewptr->next;
    }
    int floortmp = GetNextFloor(entmp);
    if(floortmp==entmp.floor){
        Event entmpNew = {entmp.OccurTime+23,EleArivStay,floortmp};
        OrderInsert(entmpNew);
    }
    else {
         elevator.floor = entmp.floor;
            ShowEvent(entmp);
        Event entmpNew = {entmp.OccurTime+61,EleStDownOneFloor,entmp.floor-1};
        OrderInsert(entmpNew);
    }

}  

void HandleEleBackOne(Event entmp){                //此过程要保证此时先将到达事件处理
    EventNode* entmpNewptr = ev;
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
    if(entmpNewptr->next->event.type == PassAriv){
        HandlePassAriv(entmpNewptr->next->event);
        entmpNewptr->next = entmpNewptr->next->next;   
    }
    else   entmpNewptr = entmpNewptr->next;
    }
    elevator.statenext = DecideNextState(elevator.floor);  
    if(elevator.statenext == UnKnown){      //此时电梯处于闲置，且仍然无具体活动，即将返回
            if(entmp.floor > 1) {
                Event entmpNew = {entmp.OccurTime+ 15+61,EleBackOneDownOneFloor,entmp.floor-1};
                OrderInsert(entmpNew); 
    }
            if(entmp.floor < 1) {
                Event entmpNew = {entmp.OccurTime+ 15+51,EleBackOneUpOneFloor,entmp.floor+1};
                OrderInsert(entmpNew); 
    }
    ShowEvent(entmp);
    }    
} 
void HandleEleBackOneDownOneFloor(Event entmp){
    
    EventNode* entmpNewptr = ev;
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == PassAriv){
          HandlePassAriv(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
        else entmpNewptr = entmpNewptr->next;
    }
    int floortmp = GetNextFloor(entmp);
    if(floortmp==entmp.floor){
        Event entmpNew = {entmp.OccurTime+23,EleArivStay,floortmp};
        OrderInsert(entmpNew);
    }
    else if(entmp.floor!=1){
        elevator.floor = entmp.floor;
        ShowEvent(entmp);
        Event entmpNew = {entmp.OccurTime+61,EleBackOneDownOneFloor,entmp.floor-1};
        OrderInsert(entmpNew);
    }

}  
void HandleEleBackOneUpOneFloor(Event entmp){
    
    EventNode* entmpNewptr = ev;
    while((entmpNewptr->next)&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == PassAriv){
          HandlePassAriv(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
        else entmpNewptr = entmpNewptr->next;
    }
    int floortmp = GetNextFloor(entmp);
    if(floortmp==entmp.floor){
        Event entmpNew = {entmp.OccurTime+14,EleArivStay,floortmp};
        OrderInsert(entmpNew);
    }
    else if(entmp.floor!=1){
        elevator.floor = entmp.floor;
        ShowEvent(entmp);
        Event entmpNew = {entmp.OccurTime+51,EleBackOneUpOneFloor,entmp.floor+1};
        OrderInsert(entmpNew);
    }

} 

    


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
    if (temp) free(temp); //

}
void DeQuene(PasserQuene * q,Passenger *passer){
    PasserNode * passernode = (*q).front;
    PasserNode * passernodetmp ;
    if(passernode->next){
    *passer = passernode->next->passer;
    (*q).front = passernode->next;
    passernodetmp  = passernode;
    free(passernodetmp);
    }

}
int DelQueneNode(PasserQuene *q, int GiveUpTime){                             //此处出错
    if(!EmptyQuene(*q)){
        PasserNode * passernodeptr = (*q).front;
        while(passernodeptr->next) {
            if(passernodeptr->next->passer.GiveupTime == GiveUpTime) {
                PasserNode * passernodeptrNew = passernodeptr->next;
               if((*q).rear==passernodeptr->next)  (*q).rear = passernodeptr;    //循环条件出错，尾指针未改
               passernodeptr->next = passernodeptr->next->next;     
               free(passernodeptrNew);
               return 1; 
            }
            passernodeptr = passernodeptr->next;                           
        }
    } 
    return 0;   
}
int EmptyQuene(PasserQuene q){
    if(q.front == q.rear) return 1;
    else return 0;
}

//栈相关函数
void InitPasserStack(PasserStack * passerstack){
    (*passerstack).base = (Passenger*)malloc(StackInitSize*sizeof(Passenger));
    (*passerstack).top = (*passerstack).base;
    (*passerstack).StackSize = StackInitSize;
}
void StackPush(PasserStack * passerstack,Passenger passertmp){
    if((*passerstack).top - (*passerstack).base >= StackInitSize) {
        (*passerstack).base = (Passenger*)realloc((*passerstack).base,((*passerstack).StackSize+10)*sizeof(Passenger));
        (*passerstack).top = (*passerstack).base + (*passerstack).StackSize;
        (*passerstack).StackSize += 10;
    }
    *((*passerstack).top) = passertmp;
    (*passerstack).top+=1;
}
void StackPop(PasserStack * passerstack,Passenger* passertmp){
    if(!EmptyStack(*passerstack)){
            (*passerstack).top-=1;
         *passertmp =  *((*passerstack).top);
    }
    
       
}
int EmptyStack(PasserStack  passerstack){
    if(passerstack.base == passerstack.top)   return 1;
    else return 0;
}

//随机数函数
void RandomItem(int *InFloor,int *OutFloor,int *GiveupTime,int *InterTime){
    *InFloor = rand()%5 ;
    *OutFloor = rand()%5 ;
    while(*InFloor == *OutFloor)   *OutFloor = rand()%5 ;
    *GiveupTime = rand()%40+10;
    *InterTime = rand()%10 + 4;
}

//获取电梯的下一个状态
State DecideNextState(int floor){            //此函数处理电梯处于激活状态，非激活状态特殊考虑 
    switch(elevator.state){
        case GoingUp : return Idle;break;     
        case GoingDown: return Idle;break; 
        case Idle: 
            int i1,i2;
            if(elevator.statebefore==GoingUp){
                for(i1=floor+1;i1<5;i1++){
                    if(elevator.CallCar[i1]==1) return  GoingUp;
                } 
            }
            if(elevator.statebefore==GoingDown){
                for(i2=floor+1;i2>=0;i2--){
                    if(elevator.CallCar[i2]==1)  return GoingDown;
                } 
            }
            if(elevator.statebefore==GoingUp){
                for(i1=floor+1;i1<5;i1++){
                    if(Building.CallDown[i1]||Building.CallUp[i1]) return  GoingUp;
                }
            }
            if(elevator.statebefore==GoingDown){
                for(i2=floor+1;i2>=0;i2--){
                    if(Building.CallDown[i2]||Building.CallUp[i2]) return  GoingDown;
                }
            }
            return UnKnown;break;
            default: return UnKnown;//用不到，防止编译错误 
            }
    
}
   

//获取要去的下一层楼                   //此处逻辑与确定下一个电梯状态略有不同
int GetNextFloor(Event entmp){                                   //此处出错
    int i1,i2,i3;
    int floor;
    switch(elevator.state){
        case GoingDown:
                    for(i1=entmp.floor;i1>=0;i1--){
                        if(elevator.CallCar[i1])  break;
                    }
                    for(i2=entmp.floor;i2>=0;i2--){
                        if(Building.CallDown[i2]) break;
                    }
                    if((i1>=0)&&(i2>=0)){
                      return   (i1>i2? i1:i2);
                    }
                    else if(i1>=0) return  i1;
                    else if(i2>=0) return  i2;
                    else {
                        for(i3=entmp.floor;i3>=0;i3--){
                            if(Building.CallUp[i3]==1) 
                            return i3;
                    }
                    }
                    return -1;           //正常情况下，不会出现
        case GoingUp:
                    for(i1=entmp.floor;i1<5;i1++){
                        if(elevator.CallCar[i1]==1)  break;
                    }
                    for(i2=entmp.floor;i2<5;i2++){
                        if(Building.CallUp[i2]) 
                            break;
                    }
                    if((i1<5)&&(i2<5)){
                      return (i1<i2? i1:i2);
                    }
                    else if(i1<5)   return i1;
                    else if(i2<5)   return i2;
                    else {
                        for(i3=entmp.floor;i3<5;i3++){
                        if(Building.CallDown[i3]) 
                            return i3;
                    }
                    }
                    return  -1;   //正常情况下，不会出现
        default :  return -1;          //用不到，防止编译错误 
                                 
    }
   
}

//消除返回1楼事件，一旦电梯服务停止，启动时要保证消除返回1楼事件
void RemoveBackOne(EventList *EListptr){
        EventNode * evtmp = *EListptr;
        EventNode * evtmpnext;
        while((evtmp->next)&&(evtmp->next->event.type != EleBackOne)){
            evtmp = evtmp->next;
        }
        if((evtmp->next)&&(evtmp->next->event.type == EleBackOne)){
            evtmpnext = evtmp->next;
            evtmp = evtmp->next->next;
        }   
        free(evtmpnext);
}

//1.按愿望思维：初始时，将功能实现进行分离，并假设各个接口足够强大

//2.数据类型：各种数据类型的用途，区别与实现

//3.事件驱动：事件驱动的程序编写

//4.小细节：&&与||的顺序

//5.抽象到深入细节：

//6.头指针、尾指针与next指针的区别

//7.不要free掉应该保存的节点(double free detected in tcache)
