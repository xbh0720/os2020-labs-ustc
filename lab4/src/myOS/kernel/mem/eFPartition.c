#include "../../include/myPrintk.h"
// 一个EEB表示一个空闲可用的Block
struct EEB {
	unsigned long next_start;
};

void showEEB(struct EEB *eeb){
	myPrintk(0x7,"EEB(start=0x%x, next=0x%x)\n", eeb, eeb->next_start);
}

//eFPartition是表示整个内存的数据结构
struct eFPartition{
	unsigned long totalN;
	unsigned long perSize;  //unit: byte	
	unsigned long firstFree;
};

void showeFPartition(struct eFPartition *efp){
	myPrintk(0x5,"eFPartition(start=0x%x, totalN=0x%x, perSize=0x%x, firstFree=0x%x)\n", efp, efp->totalN, efp->perSize, efp->firstFree);
}

void eFPartitionWalkByAddr(unsigned long efpHandler){
	/*本函数是为了方便查看和调试的。功能：
	1、打印eFPartiiton结构体的信息，可以调用上面的showeFPartition函数。
	2、遍历每一个EEB，打印出他们的地址以及下一个EEB的地址（可以调用上面的函数showEEB）

	*/
	struct eFPartition *q=(struct eFPartition *)efpHandler;
	struct EEB *p=(struct EEB *)q->firstFree;
	showeFPartition(q);
	while(p){
		showEEB(p);		//遍历链表并showEEB
		p=(struct EEB *)p->next_start;
	}
}


unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n){
	//本函数需要实现！！！
	/*根据参数persize（每个大小）和n个数计算总大小，注意persize的对齐，例如persize是31字节，你想8字节对齐，那么计算大小实际代入的一个块的大小就是32字节。
	最后别忘记加上eFPartition这个数据结构的大小，因为它也占一定的空间
	*/
	if(perSize%4)perSize=perSize/4*4+4;	//按4字节对齐
	return perSize*n+12;		//eFPartition的大小为12字节
}

unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n){
	
	/*初始化内存
	第一步需要创建一个eFPartition结构体，需要注意的是结构体的persize不是直接传入的参数perSize，需要对齐。结构体的nextStart也是需要考虑一下结构体本身的大小。
	第二步就是对每一块的内存创建一个EEB，将他们连起来构成一个链。注意最后一块的EEB的nextstart应该是0
	*/
	struct eFPartition *q=(struct eFPartition *)start;
	struct EEB *p;
	int i;
	if(perSize%4)q->perSize=perSize/4*4+4;	//对齐
	q->totalN=n;
	q->firstFree=start+12;	//+eFPartition的大小为12字节
	p=(struct EEB *)q->firstFree;
	for(i=1;i<n;i++){
		p->next_start=(unsigned long)p+q->perSize;	//建立EEB链表
		p=(struct EEB *)p->next_start;
	}
	p->next_start=0;	//最后一块的EEB的nextstart是0
	return start;
}


unsigned long eFPartitionAlloc(unsigned long EFPHandler){
	
	/*本函数分配一个空闲块的内存并返回相应的地址，EFPHandler表示整个内存的首地址
	*/
	struct eFPartition *q=(struct eFPartition *)EFPHandler;
	struct EEB *p=(struct EEB *)q->firstFree;
	q->firstFree=p->next_start;	//将第一个空闲块分配
	return q->firstFree+4;	//返回空闲块可使用的内存的首地址，+EEBsize

}


unsigned long eFPartitionFree(unsigned long EFPHandler,unsigned long mbStart){
	
	/*初始化内存
	free掉mbstart为首地址的一块内存，也要更新EBB组成的链表。
	*/
	struct eFPartition *q=(struct eFPartition *)EFPHandler;
	struct EEB *p=(struct EEB *)q->firstFree,*m,*r=(struct EEB *)mbStart;
	while(p&&(unsigned long)p<mbStart){m=p;	p=(struct EEB *)p->next_start;}	//寻找r的插入点
	m->next_start=mbStart;
	r->next_start=(unsigned long)p;
	return 1;
}
