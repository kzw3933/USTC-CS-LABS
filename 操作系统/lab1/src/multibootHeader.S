.globl start 
Magic=0x1BADB002
Flags=0x00000000
Checknum=0xE4524FFE
    
.section ".multibootHeader"    
.long Magic
.long Flags
.long Checknum


.section .data
print_data:
.ascii "helloworld,PB20061338_kezhiwei"

.align 4

.text 
.code32 

start: 

movl $0x2f652f68,0xB8A00
movl $0x2f6c2f6c,0xB8A04
movl $0x2f772f6f,0xB8A08
movl $0x2f722f6f,0xB8A0C
movl $0x2f642f6c,0xB8A10

movl $0x2f502f2c,0xB8A14
movl $0x2f322f42,0xB8A18
movl $0x2f302f30,0xB8A1C

movl $0x2f312f36,0xB8A20
movl $0x2f332f33,0xB8A24
movl $0x2f5f2f38,0xB8A28
movl $0x2f652f6b,0xB8A2C
movl $0x2f682f7a,0xB8A30
movl $0x2f772f69,0xB8A34
movl $0x2f692f65,0xB8A38



movw $0x3F8,%dx
movl $0,%ebx 
loop:	
movb print_data(,%ebx,1),%al
outb %al,%dx
addl $1,%ebx
cmp $30,%ebx
jne  loop
nop
nop

hlt
