#include "../../include/tick.h"
#include "../../include/malloc.h"
#include "../../include/myPrintk.h"

int tick_number = 0;

void startArrivedTask_hook(void);
void oneTickUpdateWallClock(void);

// 时钟中断函数调用链初始化
void tick_hook_init(void){
	tick_hook_list.head = (hookNode*)malloc(sizeof(hookNode));
	tick_hook_list.head->hook_func = oneTickUpdateWallClock;
	tick_hook_list.head->next = 0;
	tick_hook_list.tail =  tick_hook_list.head;
	tick_hook_list.num = 1;
}

void print_tick_list(void){
	hookNode* node = tick_hook_list.head;
	myPrintk(0x09,"hooklist length %d\n",tick_hook_list.num);
	while(node){
		myPrintk(0x08,"hooklist: %d\n",node->hook_func);
		node = node->next;
	}
}

// 时钟中断处理函数
void tick(void){

	tick_number++;	

	hookNode* hook_ptr = tick_hook_list.head;

	while(hook_ptr){
		hook_ptr->hook_func();
		hook_ptr = hook_ptr->next;		
	} 		
	return ;
}

int get_tick_number(void){
	return tick_number;
}