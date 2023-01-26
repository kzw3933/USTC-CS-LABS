#include "../../include/tick.h"
#include "../../include/myPrintk.h"
#include "../../include/task.h"

int timer;
extern int ss;


void init_timer(void){
    timer = 0;
    append2HookList(&tick_hook_list,update_timer);
}

void update_timer(void){
    if(tick_number % 200 == 0){
        timer ++;
        // myPrintf(0x07,"**********************************\n");
        // myPrintf(0x07,"Timer now is %d\n",timer);
        // myPrintf(0x07,"**********************************\n");
        if(taskctl.nowTsk){
            taskctl.nowTsk->runTime++;
        }
    }  
    return;
}

int get_time(void){
    return timer;
}