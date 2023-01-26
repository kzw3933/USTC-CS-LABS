#include "include/i8253.h"
#include "include/i8259.h"
#include "include/irq.h"
#include "include/uart.h"
#include "include/vga.h"
#include "include/mem.h"
#include "include/myPrintk.h"

// extern void myMain(void);		//TODO: to be generalized

// void pressAnyKeyToStart(void){
// 	myPrintk(0x5,"Prepare uart device\n");
// 	myPrintk(0x5,"Then, press any key to start ...\n");

// 	uart_get_char();
// }

// void osStart(void){
// 	pressAnyKeyToStart(); // prepare for uart device
// 	init8259A();
// 	init8253();
// 	enable_interrupt();

// 	clear_screen();

// 	pMemInit();  //after this, we can use kmalloc/kfree and malloc/free

// 	{
// 		// myPrintk(0x07,"Mem Initializing...\n");
// 		// myPrintk(0x07,"pMemHandler :0x%x\n",pMemHandler);
// 		dPartitionWalkByAddr(pMemHandler);
// 		// myPrintk(0x07,"\n");
// 		unsigned long tmp = dPartitionAlloc(pMemHandler,100);
// 		// myPrintk(0x07,"tmp : 0x%x\n",tmp);
// 		dPartitionWalkByAddr(pMemHandler);
// 		dPartitionFree(pMemHandler,tmp);
// 		dPartitionWalkByAddr(pMemHandler);
// 	}

// 	// finished kernel init
// 	// NOW, run userApp
// 	myPrintk(0x2,"START RUNNING......\n");	
// 	myMain();
// 	myPrintk(0x2, "STOP RUNNING......ShutDown\n");
// 	while(1);
// }


extern unsigned long __multiboot_start;
extern unsigned long __multiboot_end;
extern unsigned long __text_start;
extern unsigned long __text_end;
extern unsigned long __data_start;
extern unsigned long __data_end;
extern unsigned long __bss_start;
extern unsigned long __bss_end;
/* 此文件无需修改 */

// 用户程序入口
void myMain(void);

void osStart(void) {
    clear_screen();
    myPrintk(0x2, "Starting the OS...\n");
    //myMain();
    myPrintk(0x2,"multiboot address start at: %d\n",(unsigned long)&__multiboot_start);
    myPrintk(0x2,"multiboot address end at: %d\n",(unsigned long)&__multiboot_end);
    myPrintk(0x2,"text address start at: %d\n",(unsigned long)&__text_start);
    myPrintk(0x2,"text address end at: %d\n",(unsigned long)&__text_end);
    myPrintk(0x2,"data address start at: %d\n",(unsigned long)&__data_start);
    myPrintk(0x2,"data address end at: %d\n",(unsigned long)&__data_end);
    
    myPrintk(0x2,"bss address start at: %d\n",(unsigned long)&__bss_start);
    myPrintk(0x2,"bss address end at: %d\n",(unsigned long)&__bss_end);
    
    myPrintk(0x2, "Stop running... shutdown\n");
    while(1); 
}
