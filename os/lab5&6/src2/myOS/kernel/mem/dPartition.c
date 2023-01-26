#include "../../include/myPrintk.h"


//dPartition 是整个动态分区内存的数据结构
typedef struct dPartition{
	unsigned long size;
	unsigned long firstFreeStart; 
} dPartition;	//共占8个字节

#define dPartition_size ((unsigned long)0x8)

void showdPartition(struct dPartition *dp){
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}


// EMB每一个block的数据结构，userdata可以暂时不用管。
typedef struct EMB{
	unsigned long size;
	union {
		unsigned long nextStart;    // if free: pointer to next block
        unsigned long userData;		// if allocated, belongs to user
	};	                           
} EMB;	//共占8个字节

#define EMB_size ((unsigned long)0x8)

void showEMB(struct EMB * emb){
	myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x)\n", emb, emb->size, emb->nextStart);
}

unsigned long dPartitionInit(unsigned long start, unsigned long totalSize){
	//本函数需要实现！！！
	/*功能：初始化内存。
	1.在地址start处，首先是要有dPartition结构体表示整个数据结构(也即句柄)。
	2.然后，一整块的EMB被分配（以后使用内存会逐渐拆分），在内存中紧紧跟在dP后面，然后dP的firstFreeStart指向EMB。
	3.返回start首地址(也即句柄)。
	注意有两个地方的大小问题：
		第一个是由于内存肯定要有一个EMB和一个dPartition，totalSize肯定要比这两个加起来大。
		第二个注意EMB的size属性不是totalsize，因为dPartition和EMB自身都需要要占空间。
	
	*/
    // 确保待初始化的块满足大小要求
	if(totalSize <= EMB_size+dPartition_size){
		myPrintk(0x07,"Fail! The Init Block Size Must Be Larger than %d Bytes\n",EMB_size+dPartition_size);
		return 0;
	}
	// 为待管理内存建立dPartition与EMB结构，同时建立初始的空闲静态链表结构
	dPartition *dp = (dPartition *)start;
	dp->size = totalSize - dPartition_size-EMB_size;
	EMB		   *emb = (EMB*)(start+dPartition_size);
	dp->firstFreeStart = (unsigned long)emb;
	emb->size = totalSize - dPartition_size-EMB_size;
	emb->nextStart = 0;

	return start;	
}

void dPartitionWalkByAddr(unsigned long dp){
	//本函数需要实现！！！
	/*功能：本函数遍历输出EMB 方便调试
	1.先打印dP的信息，可调用上面的showdPartition。
	2.然后按地址的大小遍历EMB，对于每一个EMB，可以调用上面的showEMB输出其信息

	*/
	dPartition* dp_ptr = (dPartition*)dp;

	showdPartition(dp_ptr);
	
    //依据内存块自身的空闲链表便利各个空闲块
	unsigned long emb = dp_ptr->firstFreeStart;
	while(1){
		showEMB((EMB*)emb);
		emb = ((EMB*)emb)->nextStart;
		if(emb==0)
			break;
	}
}

//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
**/

unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size){
	//本函数需要实现！！！
	/*功能：分配一个空间
	1.使用firstfit的算法分配空间，当然也可以使用其他fit，不限制。
	2.成功分配返回首地址，不成功返回0
	3.从空闲内存块组成的链表中拿出一块供我们来分配空间(如果提供给分配空间的内存块空间大于size，我们还将把剩余部分放回链表中)，并维护相应的空闲链表以及句柄
	注意的地方：
		1.EMB类型的数据的存在本身就占用了一定的空间。

	*/
    
	dPartition* dp_ptr = (dPartition*)dp;
   
	unsigned long emb_p = dp_ptr->firstFreeStart;
	unsigned long emb_q = dp_ptr->firstFreeStart;
    
    /**--------------------------------通过前后双指针法找到第一个符合用户使用大小要求的空闲块，
    										并维护分配块与前后块的空闲链表关系------------------------------------**/
   
	while(emb_q){
		if(((EMB*)emb_q)->size+4 > size){
            //分配后的剩余空间足够建立EMB结构
			if(((EMB*)emb_q)->size +4- size >= EMB_size ){
				EMB* new_emb = (EMB*) (emb_q+4 + size);
				new_emb->nextStart = ((EMB*)emb_q)->nextStart;
				new_emb->size   = ((EMB*)emb_q)->size +4- size - EMB_size;
                //如果分配的是第一个块
				if(emb_p == dp_ptr->firstFreeStart){
					dp_ptr->firstFreeStart = (unsigned long)new_emb;
				}
				else {
					((EMB*)emb_p)->nextStart = new_emb;
				}
				dp_ptr->size = dp_ptr->size -size - EMB_size+4;
				((EMB*)emb_q)->size = size;
			}
            //分配后的剩余空间不够建立EMB结构，直接全部分配给用户
			else {
				if(emb_p == dp_ptr->firstFreeStart){
					dp_ptr->firstFreeStart = ((EMB*)emb_q)->nextStart;
				}
				else {
					((EMB*)emb_p)->nextStart = ((EMB*)emb_q)->nextStart;
				}
				dp_ptr->size = dp_ptr->size -((EMB*)emb_q)->size;
				((EMB*)emb_q)->size += 4;
			}
			// 返回有效空闲空间的地址
			return emb_q+4;
		}
		else {        //寻找下一块
			emb_p = emb_q;
			emb_q = ((EMB*)emb_q)->nextStart;
		}
	}
    // 没有空闲空间或没有符合大小要求的空闲空间可以分配
	return 0;
}

/*
 *r
 */
unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){
	//本函数需要实现！！！
	/*功能：释放一个空间
	1.按照对应的fit的算法释放空间
	2.注意检查要释放的start~end这个范围是否在dp有效分配范围内
		返回1 没问题
		返回0 error
	3.需要考虑两个空闲且相邻的内存块的合并
	*/
	dPartition* dp_ptr = (dPartition*)dp;
	unsigned long emb_p = dp_ptr->firstFreeStart;
	unsigned long emb_q = dp_ptr->firstFreeStart;
	unsigned long size = ((EMB*)(start-4))->size;
    
	/** -------------------------------使用双指针法根据待释放块地址找到其在空闲链表相邻块，
									并维护待释放块与前后块的空闲链表关系 -------------------------------**/
    
    // 释放位置非法(不符合内存分配后的地址要求)
	if(start < dp + dPartition_size + 4){
		return 0;
	}
    
    // 释放的位置在所有空闲块之前，要维护dPartition信息
	if(start < emb_p){
        // 释放位置非法(不符合内存分配后的地址要求)
		if(size +start > emb_p){
			return 0;
		}
		else {			
			dp_ptr->firstFreeStart = start -4;
			if(size +start == emb_p){
				// 与空闲链表中其后空间块相接，可以合并
				dp_ptr->size +=  ((EMB*)(start-4))->size-4 + EMB_size;
				((EMB*)(start-4))->nextStart = ((EMB*)emb_p)->nextStart;
				((EMB*)(start-4))->size = ((EMB*)(start-4))->size + ((EMB*)emb_p)->size + EMB_size-4;
			}
			else {
				((EMB*)(start-4))->nextStart = emb_p;
				dp_ptr->size +=  ((EMB*)(start-4))->size -4 ;
			}
		}			
	}	
    // 找到待释放块在空闲链表中的前后相邻块
	else {
		while(emb_q){
			if(emb_q < start){
				emb_p = emb_q;
				emb_q = ((EMB*)emb_q)->nextStart;
			}
			else {
                // 释放位置非法(不符合内存分配后的地址要求)
				if(size+ 4 > emb_q -emb_p-EMB_size-((EMB*)emb_p)->size){
				return 0;
				}
				else {
                    // 待释放块与前后块地址都相接
					if(emb_p+EMB_size+((EMB*)emb_p)->size + 4 == start && start + size == emb_q){
						((EMB*)emb_p)->nextStart = ((EMB*)emb_q)->nextStart;
						((EMB*)emb_p)->size = ((EMB*)emb_p)->size + 4+ size + EMB_size + ((EMB*)emb_q)->size;
						dp_ptr->size +=  ((EMB*)(start-4))->size +4+EMB_size;
					}
                    // 待释放块只与前块地址相接
					else if(emb_p+EMB_size+((EMB*)emb_p)->size + EMB_size == start ){
						((EMB*)emb_p)->size = ((EMB*)emb_p)->size + 4+ size ;
						dp_ptr->size +=  ((EMB*)(start-4))->size +4;
					}
                    // 待释放块只与后块地址相接
					else if(start + size == emb_q){
						((EMB*)(start-4))->size = size + EMB_size + ((EMB*)emb_q)->size-4;
						((EMB*)(start-4))->nextStart = ((EMB*)emb_q)->nextStart;
						((EMB*)emb_p)->nextStart = (EMB*)(start-4);
						dp_ptr->size +=  ((EMB*)(start-4))->size +4;
					}
                     // 待释放块与前后块地址都不相接
					else {
						((EMB*)(start-4))->size = size;
						((EMB*)(start-4))->nextStart = emb_q;
						((EMB*)emb_p)->nextStart = (EMB*)(start-4);
						dp_ptr->size +=  ((EMB*)(start-4))->size -4;
					}
					break;
				}
			}
		}
		// 待释放块地址在所有空闲块之后
		if(emb_q==0){
            // 待释放块地址与前块地址相接
			if(emb_p+EMB_size+((EMB*)emb_p)->size + 4 == start){
				((EMB*)emb_p)->size = ((EMB*)emb_p)->size + 4+ size;
				dp_ptr->size +=  ((EMB*)(start-4))->size +4;
			}
			else {
				((EMB*)emb_p)->nextStart = start -4;
				((EMB*)(start-4))->nextStart = 0;
				dp_ptr->size +=  ((EMB*)(start-4))->size -4;
			}
		}
	}
	return 1;	
}

//wrap: we select firstFit, you can select another one
//user need not know this
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long	 dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}
