#ifndef __TASK_H__
#define __TASK_H__

#define  MaxTaskNum     40+2  
#define  STACK_SIZE     4096

#define     MAX_PRIORITY    5
#define     MAX_EXEC_TIME   20

#define     PRIORITY        0
#define     EXEC_TIME       1
#define     ARRV_TIME       2

#define     FCFS        1
#define     PRIO        2
#define     SJF         3

// 进程状态
typedef enum {
    T_UNALLOC,T_READY,T_RUN,T_WAIT
} TSKSTAT;

// 进程调度器所需的任务参数
typedef struct tskPara{
    int priority;
    int exeTime;
    int arrTime;
} tskPara;

// 程序控制块
typedef struct myTCB{
    int             tid;
    TSKSTAT         state;
    unsigned int    runTime;
    unsigned int    lastScheduledTime;
    tskPara*         para;
    void (*tskBody)(void);
    unsigned long*            stack;
    unsigned long*            topOfStack;  
    struct myTCB*   next;
} myTCB;

// 任务队列节点
typedef struct taskQueueNode {
    myTCB *TCB;
    struct taskQueueNode *next;
} taskQueueNode;

// 先进先出任务队列
typedef struct taskQueueFIFO {
    taskQueueNode *head,*tail;
} taskQueueFIFO;

// 优先任务队列
typedef struct taskQueuePrio {
    taskQueueNode *head,*tail;
    int     capacity;
    int     (*cmp)(const myTCB *a, const myTCB *b);
} taskQueuePrio;

// 调度器
typedef struct scheduler {
    unsigned int type;
    unsigned int preempt;
    myTCB* (*nextTsk)(void);
    void (*enqueueTsk)(myTCB* tsk);
    void (*dequeueTsk)(int tid);
    void (*schedulerInit)(void);
    void (*schedule)(void);
    void (*tick_hook)(void);
} scheduler;

// 任务管理器
typedef struct TaskCtl{
    myTCB  tcbPool[MaxTaskNum];
    scheduler sched;
    taskQueueFIFO *rdyQueueFIFO;
    taskQueuePrio * rdyQueuePRIO;
    taskQueuePrio * arrvQueue;
    myTCB* idleTsk;
    myTCB* nowTsk;  
    myTCB* firstFreeTsk;
    int     taskNum;
}TaskCtl;


/* task.c */

int     createTsk(void (*tskBody)(void));
void    tskEnd(void);
void    tskStart(int tid);
void    destroyTsk(int tid);

void    context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk);


void    TaskCtlInit(void);

void 	stack_init(unsigned long **stk, void (*task)(void)) ;

void    updateCurrentTskRunTime_hook(void);

void    CTX_SW(void);
void    startMultiTask(void);



#define  tskInitBody myMain
void     tskInitBody(void);
void     tskIdleBody(void);

TaskCtl taskctl;

unsigned long  BspContextBase[STACK_SIZE];
unsigned long *BspContext;

unsigned long **prevTSK_StackPtrAddr;
unsigned long * nextTSK_StackPtr;



/* taskArrv.c */
void taskArrvQueueInit(void);
void enableTask(int tid);



/* taskPara.c */

void initTskPara(tskPara **para);
void setTskPara(unsigned int option,unsigned int value,tskPara* buffer);
unsigned int  getTskPara(unsigned int option, tskPara *para);

void showCurrentTskParaInfo_hook(void);

void task_execute(unsigned int wait_time);


/* taskQueue.c */
void    taskQueueFIFOInit(taskQueueFIFO *queue);

int     taskQueueFIFOEmpty(taskQueueFIFO *queue);
void    taskQueueFIFOEnqueue(taskQueueFIFO *queue, myTCB *tsk);
void    taskQueueFIFODequeue(taskQueueFIFO *queue,int tid);

myTCB*  taskQueueFIFONext(taskQueueFIFO *queue);

void 	taskQueueFIFOPrint(taskQueueFIFO *queue);

void    taskQueuePrioInit(taskQueuePrio *queue, int capacity, int (*cmp)(const myTCB *a, const myTCB *b));

int     taskQueuePrioEmpty(taskQueuePrio *queue);
void    taskQueuePrioEnqueue(taskQueuePrio *queue, myTCB *tsk);
void    taskQueuePrioDequeue(taskQueuePrio *queue,int tid);

myTCB*  taskQueuePrioNext(taskQueuePrio *queue);

void 	taskQueuePrioPrint(taskQueuePrio *queue);


/* scheduler.c */

void 	schedulerInit(void);

//FCFS scheduler
myTCB *     nextTskFCFS(void);
void        enqueueTskFCFS(myTCB *tsk);
void        dequeueTskFCFS(int tid);
void        schedulerInitFCFS(void);
void        scheduleFCFS(void);

//Prio scheduler
myTCB *     nextTskPrio(void);
void        enqueueTskPrio(myTCB *tsk);
void        dequeueTskPrio(int tid);
void        schedulerInitPrio(void);
void        schedulePrio(void);

//SJF scheduler
myTCB *     nextTskSJF(void);
void        enqueueTskSJF(myTCB *tsk);
void        dequeueTskSJF(int tid);
void        schedulerInitSJF(void);
void        scheduleSJF(void);

void initScheduler(void);


#endif