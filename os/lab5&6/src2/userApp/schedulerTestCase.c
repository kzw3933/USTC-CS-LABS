#include "../myOS/userInterface.h"


void myTskFCFS2(void) {
    myPrintf(0x07,"myTskFCFS2 Begin,the timer is %d\n",get_time());
    task_execute(4);
    myPrintf(0x07,"myTskFCFS2 End,the timer is %d\n",get_time());
    tskEnd();
}

void myTskFCFS3(void) {
    myPrintf(0x07,"myTskFCFS3 Begin,the timer is %d\n",get_time());
    task_execute(5);
    myPrintf(0x07,"myTskFCFS3 End,the timer is %d\n",get_time());
    tskEnd();
}

void myTskFCFS4(void) {
    myPrintf(0x07,"myTskFCFS4 Begin,the timer is %d\n",get_time());
    task_execute(3);
    myPrintf(0x07,"myTskFCFS4 End,the timer is %d\n",get_time());
    tskEnd();
}

void myTskFCFS5(void) {
    myPrintf(0x07,"myTskFCFS5 Begin,the timer is %d\n",get_time());
    task_execute(2);
    myPrintf(0x07,"myTskFCFS5 End,the timer is %d\n",get_time());
    tskEnd();
}
void initFCFSCases(void) {
    int newTskTid2 = createTsk(myTskFCFS2); // its tid will be 2
    setTskPara(ARRV_TIME, 3, taskctl.tcbPool[newTskTid2].para);
    setTskPara(EXEC_TIME, 4, taskctl.tcbPool[newTskTid2].para); 

    int newTskTid3 = createTsk(myTskFCFS3); // its tid will be 3
    setTskPara(ARRV_TIME, 10, taskctl.tcbPool[newTskTid3].para);
    setTskPara(EXEC_TIME, 5, taskctl.tcbPool[newTskTid3].para); 

    int newTskTid4 = createTsk(myTskFCFS4); // its tid will be 4
    setTskPara(ARRV_TIME, 13, taskctl.tcbPool[newTskTid4].para);
    setTskPara(EXEC_TIME, 3, taskctl.tcbPool[newTskTid4].para); 

    int newTskTid5 = createTsk(myTskFCFS5); // its tid will be 5
    setTskPara(ARRV_TIME, 0, taskctl.tcbPool[newTskTid5].para);
    setTskPara(EXEC_TIME, 2, taskctl.tcbPool[newTskTid5].para); 

    enableTask(newTskTid2);
    
    enableTask(newTskTid3);
  
    enableTask(newTskTid4);

    enableTask(newTskTid5);
}





void myTskPrio2(void) {
    myPrintf(0x08,"myTskPrio2 Begin,the timer is %d\n",get_time());
    task_execute(4);
    myPrintf(0x08,"myTskPrio2 End,the timer is %d\n",get_time());
    tskEnd();
    
}

void myTskPrio3(void) {
    myPrintf(0x08,"myTskPrio3 Begin,the timer is %d\n",get_time());
    task_execute(3);
    myPrintf(0x08,"myTskPrio3 End,the timer is %d\n",get_time());
    tskEnd();
}

void myTskPrio4(void) {
    myPrintf(0x08,"myTskPrio4 Begin,the timer is %d\n",get_time());
    task_execute(3);
    myPrintf(0x08,"myTskPrio4 End,the timer is %d\n",get_time());
    tskEnd();
}

void myTskPrio5(void) {
    myPrintf(0x08,"myTskPrio5 Begin,the timer is %d\n",get_time());
    task_execute(3);
    myPrintf(0x08,"myTskPrio5 End,the timer is %d\n",get_time());
    tskEnd();
}

void myTskPrio6(void) {
    myPrintf(0x08,"myTskPrio6 Begin,the timer is %d\n",get_time());
    task_execute(3);
    myPrintf(0x08,"myTskPrio6 End,the timer is %d\n",get_time());
    tskEnd();
}



void initPrioCases(void) {
    int newTskTid2 = createTsk(myTskPrio2); // its tid will be 2
    setTskPara(PRIORITY, 3, taskctl.tcbPool[newTskTid2].para);
    setTskPara(ARRV_TIME, 0, taskctl.tcbPool[newTskTid2].para);
    setTskPara(EXEC_TIME, 4, taskctl.tcbPool[newTskTid2].para); 

    int newTskTid3 = createTsk(myTskPrio3); // its tid will be 3
    setTskPara(PRIORITY, 1, taskctl.tcbPool[newTskTid3].para);
    setTskPara(ARRV_TIME, 1, taskctl.tcbPool[newTskTid3].para);
    setTskPara(EXEC_TIME, 3, taskctl.tcbPool[newTskTid3].para); 

    int newTskTid4 = createTsk(myTskPrio4); // its tid will be 4
    setTskPara(PRIORITY, 0, taskctl.tcbPool[newTskTid4].para);
    setTskPara(ARRV_TIME, 1, taskctl.tcbPool[newTskTid4].para);
    setTskPara(EXEC_TIME, 3, taskctl.tcbPool[newTskTid4].para); 

    int newTskTid5 = createTsk(myTskPrio5); // its tid will be 5
    setTskPara(PRIORITY, 4, taskctl.tcbPool[newTskTid5].para);
    setTskPara(ARRV_TIME, 1, taskctl.tcbPool[newTskTid5].para);
    setTskPara(EXEC_TIME, 3, taskctl.tcbPool[newTskTid5].para);

    int newTskTid6 = createTsk(myTskPrio4); // its tid will be 6
    setTskPara(PRIORITY, 4, taskctl.tcbPool[newTskTid6].para);
    setTskPara(ARRV_TIME, 4, taskctl.tcbPool[newTskTid6].para);
    setTskPara(EXEC_TIME, 3, taskctl.tcbPool[newTskTid6].para);

    enableTask(newTskTid3);
    enableTask(newTskTid4);
    enableTask(newTskTid6);
    enableTask(newTskTid5);
    enableTask(newTskTid2);
}



void myTskSJF2(void) {
    myPrintf(0x08,"myTskSJF2 Begin,the timer is %d\n",get_time());
    task_execute(2);
    myPrintf(0x08,"myTskSJF2 End,the timer is %d\n",get_time());
    tskEnd();
}

void myTskSJF3(void) {
    myPrintf(0x08,"myTskSJF3 Begin,the timer is %d\n",get_time());
    task_execute(5);
    myPrintf(0x08,"myTskSJF3 End,the timer is %d\n",get_time());
    tskEnd();
}

void myTskSJF4(void) {
    myPrintf(0x08,"myTskSJF4 Begin,the timer is %d\n",get_time());
    task_execute(4);
    myPrintf(0x08,"myTskSJF4 End,the timer is %d\n",get_time());
    tskEnd();
}

void myTskSJF5(void) {
    myPrintf(0x08,"myTskSJF5 Begin,the timer is %d\n",get_time());
    task_execute(3);
    myPrintf(0x08,"myTskSJF5 End,the timer is %d\n",get_time());
    tskEnd();
}

void myTskSJF6(void) {
    myPrintf(0x08,"myTskSJF6 Begin,the timer is %d\n",get_time());
    task_execute(3);
    myPrintf(0x08,"myTskSJF6 End,the timer is %d\n",get_time());
    tskEnd();
}



void initSJFCases(void) {
 

    int newTskTid2 = createTsk(myTskSJF2); // its tid will be 2
    setTskPara(ARRV_TIME, 0, taskctl.tcbPool[newTskTid2].para);
    setTskPara(EXEC_TIME, 2, taskctl.tcbPool[newTskTid2].para); 

    int newTskTid3 = createTsk(myTskSJF3); // its tid will be 3
    setTskPara(ARRV_TIME, 0, taskctl.tcbPool[newTskTid3].para);
    setTskPara(EXEC_TIME, 5, taskctl.tcbPool[newTskTid3].para); 

    int newTskTid4 = createTsk(myTskSJF4); // its tid will be 4
    setTskPara(ARRV_TIME, 0, taskctl.tcbPool[newTskTid4].para);
    setTskPara(EXEC_TIME, 4, taskctl.tcbPool[newTskTid4].para); 

    int newTskTid5 = createTsk(myTskSJF5); // its tid will be 5
    setTskPara(ARRV_TIME, 10, taskctl.tcbPool[newTskTid5].para);
    setTskPara(EXEC_TIME, 3, taskctl.tcbPool[newTskTid5].para);

    int newTskTid6 = createTsk(myTskSJF6); // its tid will be 6
    setTskPara(ARRV_TIME, 12, taskctl.tcbPool[newTskTid6].para);
    setTskPara(EXEC_TIME, 3, taskctl.tcbPool[newTskTid6].para);

    enableTask(newTskTid3);
    enableTask(newTskTid4);
    enableTask(newTskTid6);
    enableTask(newTskTid5);
    enableTask(newTskTid2);
}