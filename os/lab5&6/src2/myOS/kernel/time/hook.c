#include "../../include/malloc.h"
#include "../../include/tick.h"


void append2HookList(hookList* hooklist,void(*func)(void)){
    hookNode* node = (hookNode*)malloc(sizeof(hookNode));
    node->hook_func = func;
    node->next = 0;
    hooklist->tail->next = node;
    hooklist->tail = node;
    hooklist->num ++;
}


