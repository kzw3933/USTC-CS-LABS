#include "io.h"

void init8253(void){
	//你需要填写这里
	const short int cnt = 11932;
	outb(0x43,0x34);
	outb(0x40,cnt);
	outb(0x40,cnt>>8);
	unsigned char mask1 = inb(0x21);
	outb(0x21,mask1&(0xfe));
	unsigned char mask2 = inb(0xa1);
	outb(0xa1,mask2&(0xfe));
}
