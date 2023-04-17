.text
BubbleSort:
	li   a0,0x00000000
	li   a2,0x00007f14
	li   a4,0x00007f10
	li   a6,0x00007f08
	li   a7,0x00007f0C
	addi a1,zero,1  # 常量1
InTest:
	lw   a3,0(a2)
	blt  a3,a1,InTest  

	lw   s1,0(a4)   # 计数器1
	addi s1,s1,1

Loop1:
	addi s1,s1,-1
	beq  s1,a1,Over
	addi s2,zero,0       # 计数器2	

Loop2:	
	addi  t1,s2,0
	add  t2,t1,a0
	lw   t3,0(t2)
	lw   t4,1(t2)
	blt  t4,t3,Next
	
	addi  t5,t3,0
	addi  t3,t4,0
	addi  t4,t5,0
	sw    t3,0(t2)
	sw    t4,1(t2)  
	beq   zero,zero,Next 
		
Next:	
	addi s2,s2,1
	beq  s2,s1,Loop1
	beq  s1,s1,Loop2  
	 		
Over: 
	addi t1,a0,0
OutTest:
	lw a5,0(a7)
	blt  a5,a1,OutTest
	lw t1,0(a1)
	sw t1,0(a6)
	addi t1,t1,1
	beq zero,zero,OutTest
	nop
	nop