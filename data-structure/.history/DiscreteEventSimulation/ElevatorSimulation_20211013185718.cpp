#include<stdio.h>
#include<stdlib.h>
//1.定义必要的数据类型
typedef  enum EventType {EleArivStay,EleBackOne,EleOpen,EleClose,EleOpenOver,EleCloseOver,EleCheck,EleStUp,EleStDown,PassAriv,PassGetUp,PassGetDown,PassGiveUp} EventType;     //事件类型
//注意
//乘客之间互不干扰，电梯与乘客之间有些事件如果同时发生用计算机程序模拟时，涉及判断，需确定处理的程序中顺序 
//也就是说事件具有不同的优先度,对于同时刻发生的事,要保证: 1. 电梯到达停留，乘客到达,先处理电梯到达停留 2.电梯到达停留，乘客放弃,先处理电梯到达停留 3. 电梯检测，乘客到达，先处理乘客到达  4.电梯关闭结束，乘客到达，先处理乘客到达 5.电梯即将返回1楼等待,乘客到达，先处理乘客到达

//电梯状态
typedef enum State {GoingUp,GoingDown,Idle,UnKnown} State;    //四种状态，上升，下降，停留，未知
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

typedef struct Passenger{
    int  infloor;
    int  outfloor;
    int GiveupTime;          //放弃等待的时刻
}Passenger;
typedef struct PasserNode{
    Passenger passer;
    struct PasserNode *next;
}PasserNode;
typedef struct PasserQuene{
    PasserNode *front;
    PasserNode *rear;
}PasserQuene;            //乘客队列


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
int  GetDelCurEvent(Event *enptr);
void DelCurEvent(EventList *EListptr);

//栈相关函数
void InitPasserStack(PasserStack * passerstack);
void StackPush(PasserStack * passerstack,Passenger passertmp);
void StackPop(PasserStack * passerstack,Passenger* passertmp);
int EmptyStack(PasserStack  passerstack);        //栈空返回1，否则返回0

//队列相关函数
void InitQuene(PasserQuene *q);
void EnQuene(PasserQuene * q,Passenger passer);
void DelQuene(PasserQuene * q,Passenger *passer);
void DelQueneNode(PasserQuene *q, int GiveUpTime);
int  EmptyQuene(PasserQuene q);  


//事件处理相关函数
void HandleCurEvent(Event entmp);
void ShowEvent(Event entmp);

//乘客事件处理函数
void HandlePassAriv(Event entmp);     //优先等级更高，在同时刻发生的事件中优先处理   
void HandlePassGiveUp(Event entmp);     //优先等级更低，在同时刻发生的事件中最次处理    
void HandlePassGetUp(Event entmp);          
void HandlePassGetDown(Event entmp);         

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

//判断下一个要到达的位置
int GetNextFloor(Event entmp);

//判断电梯的下一个状态
State DecideNextState(int floor);

//随机数函数
void RandomItem(int *InFloor,int *OutFloor,int *GiveupTime,int *InterTime);    


//3.函数实现

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
    elevator.statebefore = UnKnown;
    elevator.statenext = UnKnown;
    elevator.D1 = 0;       //为0，除非人们正在上下电梯
    elevator.D2 = 1;        //为0，如果电梯已经在某层等待300以上
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
    enptr->event = entmp;
    enptr->next = NULL;
    EventList elptr = ev;
    while(elptr->next && elptr->next->event.OccurTime <entmp.OccurTime)  elptr = elptr->next;
    enptr->next = elptr->next;
    elptr->next = enptr;
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
          (*EListptr)->next = evtmp->next;
          free(evtmp);
}   //从事件表中将第一个事件删除，配合获取当前事件使用


void HandleCurEvent(Event entmp){
        switch(entmp.type){
         case EleArivStay:HandleEleArivStay(entmp);break;
        case PassGetUp: HandlePassGetUp(entmp);break;
        case PassGetDown:HandlePassGetDown(entmp);break;
        case PassGiveUp: HandlePassGiveUp(entmp);break;
    
        case EleCheck: HandleEleCheck(entmp);break;
        case EleOpen: HandleEleOpen(entmp);break;
        case EleClose: HandleEleClose(entmp);break;
        case EleOpenOver: HandleEleOpenOver(entmp);break;
        case EleCloseOver: HandleEleCloseOver(entmp);break;

        case EleStUp: HandleEleStUp(entmp);break;
        case EleStDown: HandleEleStDown(entmp);break;
        case EleBackOne:HandleEleBackOne(entmp);break;
        
        default: break;
    }  
}

void ShowEvent(Event entmp){
    printf("当前时间为: %d,",entmp.OccurTime);
    switch(entmp.type){
        case PassAriv: printf("一位乘客进入本服务系统\n");break;
        case PassGetUp:printf("一位乘客从%d楼进入电梯\n",entmp.floor+1);break;   
        case PassGetDown:printf("一位乘客从%d楼离开电梯\n",entmp.floor+1);break;
        case PassGiveUp:printf("一位乘客从%d楼放弃乘坐电梯\n",entmp.floor+1);break;
       
        case EleArivStay:printf("电梯到达并开始停留在%d楼\n",entmp.floor+1);break;
        case EleCheck:printf("电梯在%d楼检测是否还有人上下电梯\n",entmp.floor+1);break;
        case EleOpen: printf("电梯在%d楼开始开门\n",entmp.floor+1);break;
        case EleClose: printf("电梯在%d楼开门结束\n",entmp.floor+1);break;
        case EleOpenOver:printf("电梯在%d楼开始关门\n",entmp.floor+1);break;
        case EleCloseOver:printf("电梯在%d楼关门结束\n",entmp.floor+1);break;

        case EleStUp:printf("电梯在%d楼开始上行\n",entmp.floor+1);break;
        case EleStDown:printf("电梯在%d楼开始下行\n",entmp.floor+1);break;
        case EleBackOne:printf("电梯在%d楼返回1层等候\n",entmp.floor+1);break;
        
        default: break;
    }
}

//乘客事件处理
void HandlePassAriv(Event entmp){
    //进入电梯，为下一人的出现做准备
    EventNode* entmpNewptr = ev;
    while(entmpNewptr->next&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == EleArivStay){
          HandleEleArivStay(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
        else entmpNewptr = entmpNewptr->next;
    }  
    Passenger passer;
    Event entmpNew1,entmpNew2;
    int InterTime;
    int GiveUpTime;
    RandomItem(&passer.infloor,&passer.outfloor,&GiveUpTime,&InterTime);
    passer.GiveupTime = entmp.OccurTime + GiveUpTime;
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
            if(elevator.D2 == 0 || elevator.statenext ==UnKnown)   {          //此时电梯服务中止，需要设置一个电梯事件
                elevator.D2 =1;                 
                if(passer.infloor < passer.outfloor)    Building.CallUp[passer.infloor] = 1;
                else Building.CallDown[passer.infloor] = 1;
                Event entmpNew3 ;
                entmpNew3.OccurTime = en.OccurTime;    
                entmpNew3.type = EleOpen;
                entmpNew3.floor =elevator.floor;
                OrderInsert(entmpNew3);
            }
        }
       
    }
    else {
        if(elevator.D2 == 0){
            if(entmp.floor > elevator.floor){
                Event entmpNew4 = {entmp.OccurTime,EleStUp,elevator.floor};
                OrderInsert(entmpNew4);
            }
            else {
                Event entmpNew4 = {entmp.OccurTime,EleStDown,elevator.floor};
                OrderInsert(entmpNew4);  
            }    
        }
        else if((elevator.floor==1)&&(elevator.statenext ==UnKnown)){
             if(entmp.floor > elevator.floor){
                Event entmpNew4 = {entmp.OccurTime,EleStUp,elevator.floor};
                OrderInsert(entmpNew4);
            }
            else {
                Event entmpNew4 = {entmp.OccurTime,EleStDown,elevator.floor};
                OrderInsert(entmpNew4);  
            }            
        }
        if(passer.infloor < passer.outfloor)    Building.CallUp[passer.infloor] = 1;
        else Building.CallDown[passer.infloor] = 1;
    }
    ShowEvent(entmp);

}

void HandlePassGiveUp(Event entmp){           //预置事件，需先判断,此处只需队列中有需离开的让其离开即可
    EventNode* entmpNewptr = ev;
    while(entmpNewptr->next&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == EleArivStay){
          HandleEleArivStay(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
        else entmpNewptr = entmpNewptr->next;
    }  
        if(entmp.type == PassGiveUp) {
            if(elevator.floor!=entmp.floor){
                DelQueneNode(&queneDown[entmp.floor],entmp.OccurTime);
                DelQueneNode(&queneUp[entmp.floor],entmp.OccurTime);
            }    
        }
         
}
void HandlePassGetUp(Event entmp){
    Passenger passertmp;
    if(elevator.statenext == GoingUp)
        DelQuene(&queneUp[entmp.floor],&passertmp);
    else DelQuene(&queneDown[entmp.floor],&passertmp);  
    StackPush(&elevator.passerstack[passertmp.outfloor],passertmp);
    elevator.CallCar[passertmp.outfloor] = 1;
    ShowEvent(entmp);
}
void HandlePassGetDown(Event entmp){
    Passenger passertmp;
    StackPop(&elevator.passerstack[passertmp.outfloor],&passertmp);
    ShowEvent(entmp);
}

//电梯事件处理
void HandleEleArivStay(Event entmp){            //确定下一步的状态
    if(entmp.type == EleArivStay){
        elevator.statebefore = elevator.state;
        elevator.state = elevator.statenext; 
        elevator.statenext = DecideNextState(entmp.floor);       
        elevator.floor = entmp.floor;
        Event entmpNew = {entmp.OccurTime,EleOpen,entmp.floor};
        OrderInsert(entmpNew);
        ShowEvent(entmp);
    }

}     
void HandleEleOpen(Event entmp){
    if(entmp.type == EleOpen){
        Event entmpNew = {entmp.OccurTime,EleOpenOver,entmp.floor};
        OrderInsert(entmpNew);
        ShowEvent(entmp);
    }
} 
void HandleEleOpenOver(Event entmp){
    elevator.statenext = DecideNextState(entmp.floor);
    if(entmp.type == EleOpenOver){
        //先上后下
        int TimeNow = entmp.OccurTime;
        Event entmpNew1 =   {entmp.OccurTime+40,EleCheck,entmp.floor}; 
        OrderInsert(entmpNew1);
        if(elevator.CallCar[elevator.floor] == 1){
        while(!EmptyStack(elevator.passerstack[entmp.floor])){
            Event entmpNew2 = {TimeNow,PassGetDown,entmp.floor};
            OrderInsert(entmpNew2);
            TimeNow+=25;
        }
        elevator.D1 = 1;
    }
        if(elevator.statenext== GoingUp) {
            if(!EmptyQuene(queneUp[entmp.floor])){
                while(!EmptyQuene(queneUp[entmp.floor])){
                    Event entmpNew3 = {TimeNow,PassGetUp,entmp.floor};
                    OrderInsert(entmpNew3);
                    TimeNow+=25;
            }
            }

            else {
                while(!EmptyQuene(queneDown[entmp.floor])){
                    Event entmpNew3 = {TimeNow+25,PassGetUp,entmp.floor};
                    OrderInsert(entmpNew3);
                    TimeNow+=25;
                }

            }
        }
        else {
            while(!EmptyQuene(queneDown[entmp.floor])){
                Event entmpNew3 = {TimeNow+25,PassGetUp,entmp.floor};
                OrderInsert(entmpNew3);
                TimeNow+=25;
            }
        }
        ShowEvent(entmp);

    }


}
void HandleEleCheck(Event entmp){
    elevator.statenext = DecideNextState(entmp.floor);
    EventNode* entmpNewptr = ev;
    while(entmpNewptr->next&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == PassAriv){
          HandlePassAriv(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
        else entmpNewptr = entmpNewptr->next;
    }

    if(elevator.statenext == EleStUp){
        if(EmptyQuene(queneUp[entmp.floor])){
            Event entmpNew = {entmp.OccurTime,EleClose,entmp.floor};
            OrderInsert(entmpNew);
        }
    }
    else if(elevator.statenext == EleStDown){
        if(EmptyQuene(queneDown[entmp.floor])){
            Event entmpNew = {entmp.OccurTime,EleClose,entmp.floor};
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
    elevator.statenext = DecideNextState(entmp.floor);
    EventNode* entmpNewptr = ev;
    while(entmpNewptr->next&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
        if(entmpNewptr->next->event.type == PassAriv){
          HandlePassAriv(entmpNewptr->next->event);
          entmpNewptr->next = entmpNewptr->next->next;   
        }
        else entmpNewptr = entmpNewptr->next;
    }
    elevator.statenext = DecideNextState(entmp.floor);
    if(elevator.statenext == EleStUp){
        if(!EmptyQuene(queneUp[entmp.floor])){
            Event entmpNew = {entmp.OccurTime,EleOpen,entmp.floor};
            OrderInsert(entmpNew);
        }
        else{
            Event entmpNew = {entmp.OccurTime,EleStUp,entmp.floor};
            OrderInsert(entmpNew);
        } 
    }
    else if(elevator.statenext == EleStDown){
        if(!EmptyQuene(queneDown[entmp.floor])){
            Event entmpNew = {entmp.OccurTime,EleOpen,entmp.floor};
            OrderInsert(entmpNew);
        }
        else {
            Event entmpNew = {entmp.OccurTime,EleStDown,entmp.floor};
            OrderInsert(entmpNew);
        }
    }
    ShowEvent(entmp);
}
void HandleEleStUp(Event entmp){
    Building.CallUp[entmp.floor] = 0;
    elevator.statebefore = elevator.state;
    elevator.state = GoingUp;
    elevator.statenext = DecideNextState(entmp.floor);
    elevator.CallCar[entmp.floor] = 0;Building.CallUp[entmp.floor] = 0;
    int floortmp = GetNextFloor(entmp);
    int interval = floortmp - entmp.floor;
    Event entmpNew = {entmp.OccurTime +15+interval*51+14,EleArivStay,floortmp};
    OrderInsert(entmpNew);
    ShowEvent(entmp);
}   
void HandleEleStDown(Event entmp){
    Building.CallDown[entmp.floor] = 0;
    elevator.statebefore = elevator.state;
    elevator.state = GoingDown;
    elevator.statenext = DecideNextState(entmp.floor);
    elevator.CallCar[entmp.floor] = 0;Building.CallDown[entmp.floor] = 0;
    int floortmp = GetNextFloor(entmp);
    int interval = floortmp - entmp.floor;
    Event entmpNew = {entmp.OccurTime +15+interval*61+23,EleArivStay,floortmp};
    OrderInsert(entmpNew);
    ShowEvent(entmp);
}        
void HandleEleBackOne(Event entmp){                //预置事件，需先判断
    EventNode* entmpNewptr = ev;
    while(entmpNewptr->next&&(entmpNewptr->next->event.OccurTime==entmp.OccurTime)){
    if(entmpNewptr->next->event.type == PassAriv){
        HandlePassAriv(entmpNewptr->next->event);
        entmpNewptr->next = entmpNewptr->next->next;   
    }
        else entmpNewptr = entmpNewptr->next;
    }
    elevator.D2 = 1;
    elevator.statenext = DecideNextState(entmp.floor);
    if(elevator.statenext == UnKnown){
            if(entmp.floor > 1) elevator.state = GoingDown;
            else  elevator.state = GoingUp;
            elevator.statenext = DecideNextState(entmp.floor);
            if(entmp.floor > 1) {
                Event entmpNew = {entmp.OccurTime+ 15+(entmp.floor-1)*61+23,EleArivStay,1};
                OrderInsert(entmpNew); 
    }
            if(entmp.floor < 1) {
                Event entmpNew = {entmp.OccurTime+ 15+(1-entmp.floor)*51+14,EleArivStay,1};
                OrderInsert(entmpNew); 
    }
    ShowEvent(entmp);
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
}
void DeQuene(PasserQuene * q,Passenger *passer){
    PasserNode * passernode = (*q).front;
    *passer = passernode->next->passer;
    (*q).front = passernode->next;

    free(passernode);
}
void DelQueneNode(PasserQuene *q, int GiveUpTime){
    if(!EmptyQuene(*q)){
        PasserNode * passernodeptr = (*q).front;
        while(passernodeptr!=(*q).rear) {
            if(passernodeptr->next->passer.GiveupTime == GiveUpTime) {
                PasserNode * passernodeptrNew = passernodeptr->next;
               passernodeptr->next = passernodeptrNew->next;
               free(passernodeptrNew);
               if(passernodeptr->next == (*q).rear)  (*q).rear = passernodeptr;
               break;   
            }
        }
    }    
}
int EmptyQuene(PasserQuene q){
    if(q.front == q.rear) return 1;
    else 0;
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
State DecideNextState(int floor){
    switch(elevator.state){
        case GoingUp : return Idle;break;     //已确保elevator.state不为UnKnown
        case GoingDown: return Idle;break;
        case Idle: 
            int i1,i2;
            for(i1=floor+1;i1<5;i1++){
                if(elevator.CallCar[i1]==1) return GoingUp;
            }
            for(i2=floor-1;i2>0;i2--){
                if(elevator.CallCar[i2]==1) return GoingDown;
            }
            if(elevator.statebefore==GoingUp){
                if(!EmptyQuene(queneUp[floor])) return GoingUp;
                else if(!EmptyQuene(queneDown[floor])) return GoingDown;
                else  return UnKnown;
            }
            if(elevator.statebefore==GoingDown){
                if(!EmptyQuene(queneDown[floor])) return GoingDown;
                else if(!EmptyQuene(queneUp[floor])) return GoingUp;
                else  return UnKnown;
            }
            break;
        default :break; 
    }
}
//获取要去的下一层楼
int GetNextFloor(Event entmp){
    switch(elevator.state){
        case GoingDown:
                    int i1,i2;
                    for(i1=entmp.floor-1;i1>=0;i1--){
                        if(elevator.CallCar[i1]==1)  return i1;
                    }
                    for(i2=entmp.floor-1;i2>=0;i2--){
                        if(!(EmptyQuene(queneUp[i2])) ||!(EmptyQuene(queneDown[i2])) ) 
                            return i2;
                    }
                    break;
        case GoingUp:
                    int i1,i2;
                    for(i1=entmp.floor+1;i1<5;i1++){
                        if(elevator.CallCar[i1]==1)  return i1;
                    }
                    for(i2=entmp.floor+1;i2<5;i2++){
                        if(!(EmptyQuene(queneUp[i2])) ||!(EmptyQuene(queneDown[i2])) ) 
                            return i2;
                    }
                    break;                   
                
    }
}



//1.按愿望思维：初始时，将功能实现进行分离，并假设各个接口足够强大

//2.数据类型：各种数据类型的用途，区别与实现

//3.事件驱动：事件驱动的程序编写