#include "../../include/mem.h"

unsigned long malloc(unsigned long size)	{
return dPartitionAlloc(pMemHandler, size);
    //调用实现的dPartition或者是ePartition的alloc
}

unsigned long free(unsigned long start){	
return (unsigned long)dPartitionFree(pMemHandler, start);
    //调用实现的dPartition或者是ePartition的free
}