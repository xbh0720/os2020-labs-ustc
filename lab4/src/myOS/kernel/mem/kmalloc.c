#include "../../include/mem.h"

unsigned long kmalloc(unsigned long size){
	

return dPartitionAlloc(pMemHandler, size);
    //调用实现的dPartition或者是ePartition的alloc
}

unsigned long kfree(unsigned long start){
	
return dPartitionFree(pMemHandler, start);
    //调用实现的dPartition或者是ePartition的free
}