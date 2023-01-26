#ifndef __TASK_H__
#define __TASK_H__

    #include "../../userApp/userApp.h"

#define  MaxTaskNum  40  
#define  STACK_SIZE   4096

#define  initTskBody myMain

typedef enum {
    T_UNALLOC,T_READY,T_RUN,T_WAIT
} TSKSTAT;



typedef struct TCB{
    int         tid;
    TSKSTAT     state;
    int*        stack;
    int*        topOfStack;
    struct TCB*        next;
} TCB;

typedef struct taskQueueFIFO {
    TCB* head;
    TCB* tail;
    TCB* idleTsk;
} taskQueueFIFO;


typedef struct TaskCtl{
    TCB  tcbPool[MaxTaskNum];
    unsigned char allocated[MaxTaskNum];
    taskQueueFIFO  fifoQueue;
}TaskCtl;

TaskCtl taskctl;

TCB*     idleTsk;
TCB*     nowTsk;
TCB*     firstFreeTsk;

unsigned long **prevTSK_StackPtrAddr;
unsigned long *nextTSK_StackPtr;

unsigned long BspContextBase[STACK_SIZE];
unsigned long *BspContext;


int createTsk(void (*tskBody)(void));
void tskEnd(void);
void tskStart(TCB *tsk);
void destroyTsk(int tid);


void TaskCtlInit(void);
void stack_init(unsigned long **stk, void (*task)(void)) ;

void startMultiTask(void);
void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk);

void tskEnqueue(TCB* tcb);
void tskDequeue(TCB* tcb);

TCB* nextFCFSTsk(void);

#endif