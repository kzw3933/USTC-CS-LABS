.text
BubbleSort:
	li   a0,0x00000000
	addi a1,zero,1  # 常量1
GetNum: 
	li   s1,20   # 计数器1

	addi s1,s1,1

Loop1:
	addi s1,s1,-1
	beq  s1,a1,Over
	addi s2,zero,0       # 计数器2	
Loop2:	
	addi  t1,s2,0
	add   t2,t1,a0
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
	lw   x11,0(a0)
	lw   x12,1(a0)
	lw   x13,2(a0)
	lw   x14,3(a0)
	lw   x15,4(a0)
	lw   x16,5(a0)
	lw   x17,6(a0)
	lw   x18,7(a0)
	lw   x19,8(a0)
	lw   x20,9(a0)
	lw   x21,10(a0)
	lw   x22,11(a0)
	lw   x23,12(a0)
	lw   x24,13(a0)
	lw   x12,14(a0)
	lw   x12,15(a0)
	lw   x12,16(a0)
	lw   x12,17(a0)
	lw   x12,18(a0)
	lw   x12,19(a0)
	nop
	nop
	nop