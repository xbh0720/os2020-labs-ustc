
#include "../../include/myPrintk.h"
//分割与合并

//dPartition 是整个动态分区内存的数据结构
struct dPartition{
	unsigned long size;
	unsigned long firstFreeStart; 
};

void showdPartition(struct dPartition *dp){
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}


// EMB每一个block的数据结构，userdata可以暂时不用管。
struct EMB{
	unsigned long size;
	union {
		unsigned long nextStart;    // if free: pointer to next block
        		unsigned long userData;		// if allocated, blongs to user
	}tag;	                           
};

void showEMB(struct EMB * emb){
	myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x)\n", emb, emb->size, emb->tag.nextStart);
}

unsigned long dPartitionInit(unsigned long start, unsigned long totalSize){
	//本函数需要实现！！！
	/*
	初始化内存。
	在地址start处，首先是要有dP结构体表示整个数据结构。
	然后，一整块的EMB被分配（以后使用内存会逐渐拆分），在内存中紧紧跟在dP后面，然后dP的firstFreeStart指向EMB。
	返回start首地址。
	注意有两个地方的大小问题，第一个是由于内存肯定要有一个EMB和一个dP，totalSize肯定要比这两个加起来大不能太小。
	第二个注意EMB的可以空间不是totalsize，因为dP要占空间
	*/
	//对dP进行初始化
	struct dPartition *q=(struct dPartition *)start;
	struct EMB *p;
	q->size=totalSize;
	q->firstFreeStart=(unsigned long)(q+1);  //指向dP结构体之后的空闲区

	p=(struct EMB *)q->firstFreeStart;	
	p->size=totalSize-8;		//EMB大小应减去dP的size
	p->tag.nextStart=0;
	return start;

}

void dPartitionWalkByAddr(unsigned long dp){
	//本函数需要实现！！！
	/*本函数遍历输出EMB 方便调试
	先打印dP的信息，可调用上面的showdPartition。
	然后按地址的大小遍历EMB，对于每一个EMB，可以调用上面的showEMB输出其信息
	*/
	struct dPartition *q=(struct dPartition *)dp;
	struct EMB *p=(struct EMB *)q->firstFreeStart;
	showdPartition(q);
	while(p){
	showEMB(p);
	p=(struct EMB *)p->tag.nextStart;	//遍历EMB链表并showEMB
}
}

//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
**/
unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size){
	//本函数需要实现！！！
	/*
	使用firstfit的算法分配空间，当然也可以使用其他fit，不限制。
	最后，成功分配返回首地址，不成功返回0
	注意的地方：
	1、和eFPartition一样，要注意实际分配的block的大小和传入的参数size是不一样的。要注意字节对齐的问题，还有它最小值不是0，因为EMB数据结构有大小。在比较所需要的大小和block的size时候，也用的是实际分配需要的大小而非传入的参数size。
	2、分配就好像是链表的一个块分裂成两个，注意边界情况。
	*/
	struct dPartition *q=(struct dPartition *)dp;
	struct EMB *p=(struct EMB *)q->firstFreeStart,*r,*m;
	if(size%8)size=size/8*8+8;    //8字节对齐
	
	if(p->size>=size+8){	
	//若第一个空闲块p的size比需要的size+EMBsize至少大一个EMBsize，那么
	//将p进行分割并修改dP的firstFreeStart
		if(p->size>size+8+8){	
			r=(struct EMB *)((unsigned long)p+size+8);
			q->firstFreeStart=(unsigned long)r;
			r->size=p->size-size-8;
			p->size=size+8;
			r->tag.nextStart=p->tag.nextStart;
		}
		else q->firstFreeStart=p->tag.nextStart;	//否则直接分配
	}
	else{
		while(p&&p->size<size+8){	//遍历EMB链表，找到足够大的空闲块
			m=p;
			p=(struct EMB*)p->tag.nextStart;
		}
		if(p){	//找到符合大小需求的空闲块p
			if(p->size>size+8+8){	//分割
				r=(struct EMB *)p+size+8;
				r->size=p->size-size-8;
				m->tag.nextStart=(unsigned long)r;
				p->size=size+8;
			}
			else m->tag.nextStart=p->tag.nextStart;	//直接分配
		}
		else return 0;	//无符合需求的空闲块，分配失败
	}
	return (unsigned long)p+8;	//分配成功，返回用户可使用内存的首地址，+EMBsize
}

/*
 *r
 */
unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){
	//本函数需要实现！！！
	/*按照对应的fit的算法释放空间
	注意检查要释放的start~end这个范围是否在dp有效分配范围内
	返回1 没问题
	返回0 error
	*/
	struct dPartition *q=(struct dPartition *)dp;	
	struct EMB *p=(struct EMB *)q->firstFreeStart,*r,*m;
	start=start-8;
	r=(struct EMB *)(start);
	//r指向需要释放的块的首地址，令其指向该块的EMB的首地址
	//是否在有效分配范围内
	if(!(start>=dp+8&&(start+r->size)<(dp+q->size)))return 0;

	if(start+r->size==q->firstFreeStart)	//若需要释放的块r与firstFreeStart相邻，将两块合并
		{q->firstFreeStart=start;
		r->size+=p->size;
		r->tag.nextStart=p->tag.nextStart;}
	else if(start<q->firstFreeStart){		//若r首地址小于firstFreeStart将r作为空闲块插入
		q->firstFreeStart=start;
		r->tag.nextStart=(unsigned long)p;}
	else{
		while(p) 	//寻找与r前相邻的空闲块
			{if(p->size+(unsigned long)p==start)break;
			 p=(struct EMB *)p->tag.nextStart;}
		if(p){   	//若找到与r前相邻的空闲块
			m=(struct EMB *)p->tag.nextStart;
			//判断下一个空闲块是否与r后相邻
			if(p->tag.nextStart==start+r->size){	//将三个相邻块合并
				p->tag.nextStart=m->tag.nextStart;
				p->size=r->size+p->size+m->size;}
			else	{p->tag.nextStart=p->tag.nextStart;
				p->size=r->size+p->size;}
		}
		else {	//若无r的前相邻的空闲块，寻找r的插入点
			p=(struct EMB *)q->firstFreeStart;
			while((unsigned long)p<start&&p)
				{m=p;	p=(struct EMB *)p->tag.nextStart;}
			if(p&&(unsigned long)p==start+r->size)	{	//若r有后相邻的空闲块
				r->tag.nextStart=p->tag.nextStart; 	//将两个空闲块合并
				m->tag.nextStart=start ; r->size=r->size+p->size;}
			else	{r->tag.nextStart=m->tag.nextStart;	//若无后相邻的空闲块
				m->tag.nextStart=start;}		//将r作为空闲块插入
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
