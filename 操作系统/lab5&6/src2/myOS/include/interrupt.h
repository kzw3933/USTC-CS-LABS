#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

// start or close interrupt
void disable_interrupt(void);   //although defined in i386/irq.S, it's a general interface used by OS kernel
void enable_interrupt(void);

// initialize interrupt
void init8259A(void);

// time interrupt
void init8253(void);


#endif