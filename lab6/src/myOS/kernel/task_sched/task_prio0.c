#include "../../include/task_sched.h"
#include "../../include/tcb.h"

myTCB *rqPRIO;  //ready queue

void rqPRIO0Init(void) {
	rqPRIO = idleTsk;	// head <- idleTsk
	dLinkListInit((dLinkedList *)(&(rqPRIO->thisNode)));
}

myTCB * nextPRIO0Tsk(void) {
	return (myTCB*)dLinkListFirstNode((dLinkedList*)rqPRIO);	
}

/*
 *tskEnqueuePRIO: insert 
 */
void tskEnqueuePRIO0(myTCB *tsk){
    myTCB *p = nextPRIO0Tsk();
    //按优先级顺序插入rqPRIO队列
    while(p->para.priority>tsk->para.priority&&p!=rqPRIO){   //优先级数越大优先级越高
        p=(myTCB *)p->thisNode.next;
    }
	dLinkInsertBefore((dLinkedList*)rqPRIO,(dLink_node*)p,(dLink_node*)tsk);	
    
   
}

/*
 *tskDequeuePRIO: delete the node
 */
void tskDequeuePRIO0(myTCB *tsk){
	dLinkDelete((dLinkedList*)rqPRIO,(dLink_node*)tsk);	
}

/*===========================================
 *======PRIO=================================
 *===========================================*/
void schedulerInit_PRIO0(void){
    rqPRIO0Init();

	/* default for all task except idleTsk*/
	defaultTskPara.schedPolicy = SCHED_PRIO;

	/* special for idleTsk*/
	_setTskPara(idleTsk,&defaultTskPara);
	idleTsk->para.schedPolicy = SCHED_IDLE;
}

struct scheduler scheduler_PRIO0 = {
    .type = SCHEDULER_PRIORITY0,
    .nextTsk_func = nextPRIO0Tsk,
    .enqueueTsk_func = tskEnqueuePRIO0,
    .dequeueTsk_func = tskDequeuePRIO0,
    .schedulerInit_func = schedulerInit_PRIO0,
	.createTsk_hook = NULL,
    .tick_hook = NULL       
};
