#include "../include/task.h"
#include "../include/myPrintk.h"

void schedule(void){
    schedule_fcfs();
}

void schedule_fcfs(void){
    // FCFS schedule
    TCB* preTsk = nowTsk;
    nowTsk = nextFCFSTsk();
    nowTsk->state = T_RUN;
    myPrintk(0x07,"The scheduler tid is %d\n",nowTsk->tid);
    context_switch(&preTsk->topOfStack,nowTsk->topOfStack);
}