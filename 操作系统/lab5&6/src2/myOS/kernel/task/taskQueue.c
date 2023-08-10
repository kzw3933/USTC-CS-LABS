#include "../../include/task.h"
#include "../../include/interrupt.h"
#include "../../include/malloc.h"
#include "../../include/myPrintk.h"

// FIFO queue

void taskQueueFIFOPrint(taskQueueFIFO *queue){
    if(!queue){
        myPrintk(0x08,"Error! FIFO queue is null\n");
    }
    else {
        taskQueueNode * node = queue->head;
        while(node){
            myPrintk(0x05,"node tid is %d\n",node->TCB->tid);
            node = node->next;
        }
    }   
}

void taskQueueFIFOInit(taskQueueFIFO *queue){
    queue->head = 0;
    queue->tail = 0;
}

int taskQueueFIFOEmpty(taskQueueFIFO *queue){
    return (queue->head == 0 );
}

myTCB * taskQueueFIFONext(taskQueueFIFO *queue){
    if (taskQueueFIFOEmpty(queue))
		return 0;
	else
    	return queue->head->TCB;
}

void taskQueueFIFOEnqueue(taskQueueFIFO *queue, myTCB *tsk) {
    taskQueueNode *node = (taskQueueNode *)malloc(sizeof(taskQueueNode));
    if (!node)
        return ;

    node->TCB = tsk;
    node->next = 0;

    disable_interrupt();

    if (taskQueueFIFOEmpty(queue))
        queue->head = node;
    else {
        queue->tail->next = node;       
    }
    queue->tail = node;
    enable_interrupt();
}

void  taskQueueFIFODequeue(taskQueueFIFO *queue,int tid) {
    taskQueueNode *node = queue->head;
    if (taskQueueFIFOEmpty(queue))
        return ;

    else {
        disable_interrupt();
        if(queue->head == queue->tail){
            queue->head = 0;
            queue->tail = 0;
        }
        else {
            queue->head = node->next;
        }
        enable_interrupt();
        // free(node);
    }
}


// Prio queue

void taskQueuePrioPrint(taskQueuePrio *queue){

    if(!queue){
        myPrintk(0x08,"Error! Prio queue is null\n");
    }
    else {
        taskQueueNode * node = queue->head;
        while(node){
            myPrintk(0x05,"node tid is %d\n",node->TCB->tid);
            node = node->next;
        }
    }
    
}

void taskQueuePrioInit(taskQueuePrio *queue, int capacity, int (*cmp)(const myTCB *a, const myTCB *b)) {
    queue->head = 0;
    queue->tail = 0;
    queue->capacity = capacity;
    queue->cmp = cmp;

}

int taskQueuePrioEmpty(taskQueuePrio *queue) {
    return (queue->head == 0 && queue->tail == 0);
}

myTCB * taskQueuePrioNext(taskQueuePrio *queue) {
    if (taskQueuePrioEmpty(queue))
		return 0;
	else{
        // myPrintk(0x06,"LOOK:\n");
        // taskQueuePrioPrint(queue);
        return queue->head->TCB;
    }
    	
}

void taskQueuePrioEnqueue(taskQueuePrio *queue, myTCB *tsk) {
    disable_interrupt();
    // myPrintk(0x05,"Enqueue:\n");
    taskQueueNode* node = (taskQueueNode*)malloc(sizeof(taskQueueNode));
    node->TCB = tsk;
    node->next = 0;

    taskQueueNode* p = queue->head;
    taskQueueNode* q;
    if(!queue->head){
        queue->head = node;
        queue->tail = node;
    }
    else if(queue->cmp(queue->head->TCB, tsk)>0){
        node->next = queue->head;
        queue->head = node;
    }
    else {
        q = p->next;
        while(q && queue->cmp(q->TCB, tsk)<=0){
            p = q;
            q = q->next;
        }
        if(!q){
            queue->tail->next = node;
            queue->tail = node;
        }
        else {
            node->next = q;
            p->next = node;
        }
    }
    // taskQueuePrioPrint(queue);
    enable_interrupt();

}


void  taskQueuePrioDequeue(taskQueuePrio *queue,int tid) {
    disable_interrupt();
    // myPrintk(0x05,"Dequeue\n");
    // if (taskQueuePrioEmpty(queue))
    //     return ;

    // else {
    //     taskQueueNode *node = queue->head;

        
    //     if (queue->tail == queue->head){
    //         queue->head = 0;
    //         queue->tail = 0;
    //     }
    //     else {
    //         queue->head = node->next;
    //     }          
        
    //     // free((unsigned long)node);
    // }
    taskQueueNode *p = queue->head;
    taskQueueNode *q;
    if(p->TCB->tid == tid){
        queue->head = p->next;
        if(p == queue->tail){
            queue->tail = 0;
        }
    }
    else {
        q = p->next;
        while(q->TCB->tid != tid){
            p = q;
            q = q->next;
        }
        p->next = q->next;
    }
    enable_interrupt();
}
