#ifndef __TICK_H__
#define __TICK_H__

#define TICK_FREQ 20

// hook函数链结点
typedef struct hookNode{
    void (*hook_func)(void);
    struct hookNode* next;
}hookNode;

// hook函数链
typedef struct hookList {
    hookNode* head;
    hookNode* tail;
    int num;
}hookList;

/* hook.c */

void append2HookList(hookList* hooklist,void(*func)(void));

/* tick.c */
extern int tick_number;

hookList tick_hook_list;

void tick_hook_init(void);
void tick(void);
int  get_tick_number(void);
void oneTickUpdateWallClock(void);    


/* timer.c */
void            init_timer(void);
void            update_timer(void);
int             get_time(void);


/* wallClock.c */
void getWallClock(int *h, int *m, int *s);
void setWallClock(int h, int m, int s);

void setWallClockHook(void (*func)(void));

void (*wallClock_hook)(void);


#endif
