#include "../../include/task_sched.h"
#include "../../include/tcb.h"
#include "../../include/myPrintk.h"
#include "../../include/task.h"
//#include "watchdog.c"
//如何使运行exeTime后结束任务？
//解决办法：
//可以通过watchdogtimer设定计时器并与时钟中断结合，
extern void context_switch(myTCB *prevTsk, myTCB *nextTsk);

myTCB *rqSJF;  //ready queue

void rqSJFInit(void) {
	rqSJF = idleTsk;	// head <- idleTsk
	dLinkListInit((dLinkedList *)(&(rqSJF->thisNode)));
}

myTCB * nextSJFTsk(void) {
	return (myTCB*)dLinkListFirstNode((dLinkedList*)rqSJF);	
}

/*
 *tskEnqueueSJF: insert into the tail node
 */
void tskEnqueueSJF(myTCB *tsk){
    myTCB *p = nextSJFTsk();
    //按运行时间大小插入队列，运行时间越短越早执行
    while(p->para.exeTime<tsk->para.exeTime&&p!=rqSJF){
        p=(myTCB *)p->thisNode.next;
    }
	dLinkInsertBefore((dLinkedList*)rqSJF,(dLink_node*)p,(dLink_node*)tsk);	
}

/*
 *tskDequeueFIFO: delete the first node
 */
void tskDequeueSJF(myTCB *tsk){
	dLinkDelete((dLinkedList*)rqSJF,(dLink_node*)tsk);	
}

/*===========================================
 *======SJF=================================
 *===========================================*/
void schedulerInit_SJF(void){
    rqSJFInit();

	/* default for all task except idleTsk*/
	defaultTskPara.schedPolicy = SCHED_SJF;

	/* special for idleTsk*/
	_setTskPara(idleTsk,&defaultTskPara);
	idleTsk->para.schedPolicy = SCHED_IDLE;
}

//运行时间结束进行调度
void Timer_tick_hook(void){
    if(--currentTsk->para.exeTime==0){
        tskEnd();
    }    
}

struct scheduler scheduler_SJF = {
    .type = SCHEDULER_SJF,
    .nextTsk_func = nextSJFTsk,
    .enqueueTsk_func = tskEnqueueSJF,
    .dequeueTsk_func = tskDequeueSJF,
    .schedulerInit_func = schedulerInit_SJF,
	.createTsk_hook = NULL,
    .tick_hook = Timer_tick_hook       //
};
