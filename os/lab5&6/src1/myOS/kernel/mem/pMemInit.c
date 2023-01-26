#include "../../include/myPrintk.h"
#include "../../include/mem.h"
#include "../../include/io.h"
unsigned long pMemStart;//可用的内存的起始地址
unsigned long pMemSize;//可用的大小

void memTest(unsigned long start, unsigned long grainSize){
	/*本函数需要实现！！！*/
	/*功能：检测算法
		这一个函数对应实验讲解ppt中的第一大功能-内存检测。
		本函数的功能是检测从某一个地址开始的内存是否可用，具体算法就可以用ppt上说的写了读看是否一致。
	注意点两个：
	1、开始的地址要大于1M，需要做一个if判断。
	2、grainsize不能太小，也要做一个if判断
	*/
	const int min_grain_size = 0x1000;
    
	//内存起始地址要大于1M
	if(start < 0x100000){
		myPrintk(0x07,"The start address require >= 1M!\n");
	}
	else {
		grainSize = grainSize > min_grain_size ? grainSize : min_grain_size;
        
		char find_start = 0;
		unsigned char old[2];
		unsigned long i = start;
		
		while(1){
			// 一个内存块的头两个字节,两次读写校对
			old[0] = *((unsigned char*)i);
			old[1] = *((unsigned char*)(i+1));
			*((unsigned char*)i) = 0x55;
			*((unsigned char*)(i+1)) = 0xAA;
			if(*((unsigned char*)i) != 0x55 || *((unsigned char*)(i+1)) != 0xAA){
				break;
			}				
			*((unsigned char*)i) = 0xAA;
			*((unsigned char*)(i+1)) = 0x55;
			if(*((unsigned char*)i) != 0xAA || *((unsigned char*)(i+1)) != 0x55){
				break;
			}
			*((unsigned char*)i) = old[0];
			*((unsigned char*)(i+1)) = old[1];
			
            // 一个内存块的尾两个字节,两次读写校对
			old[0] = *((unsigned char*)(i+grainSize-2));
			old[1] = *((unsigned char*)(i+1+grainSize-2));
			*((unsigned char*)(i+grainSize-2)) = 0x55;
			*((unsigned char*)(i+1+grainSize-2)) = 0xAA;
			if(*((unsigned char*)(i+grainSize-2)) != 0x55 || *((unsigned char*)(i+1+grainSize-2)) != 0xAA){
				break;
			}				
			*((unsigned char*)(i+grainSize-2)) = 0xAA;
			*((unsigned char*)(i+1+grainSize-2)) = 0x55;
			if(*((unsigned char*)(i+grainSize-2)) != 0xAA || *((unsigned char*)(i+1+grainSize-2)) != 0x55){
				break;
			}
			*((unsigned char*)(i+grainSize-2)) = old[0];
			*((unsigned char*)(i+1+grainSize-2)) = old[1];
			//找到有效内存起始地址
			if(find_start == 0){
				pMemStart = i;
				find_start = 1;
			}
			i += grainSize;
		}
		pMemSize = i-pMemStart;
		myPrintk(0x7,"MemStart: 0x%x  \n",pMemStart);
		myPrintk(0x7,"MemSize:  0x%x  \n",pMemSize);
	}	
}



extern unsigned long _end;
void pMemInit(void){
	unsigned long _end_addr = (unsigned long) &_end;
	memTest(0x100000,0x1000);
	myPrintk(0x7,"_end:  0x%x  \n", _end_addr);
	if (pMemStart <= _end_addr) {
		pMemSize -= _end_addr - pMemStart;
		pMemStart = _end_addr;
	}

	pMemHandler = dPartitionInit(pMemStart,pMemSize);	
}
