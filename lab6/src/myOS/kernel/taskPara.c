#include "../include/tcb.h"
#include "../include/taskPara.h"
#include "../include/task_sched.h"

//extern void initLeftExeTime_sjf(myTCB* tsk);//作用？

// may modified during scheduler_init
tskPara defaultTskPara = {
    .priority = MAX_PRIORITY_NUM,
    .exeTime = MAX_EXETIME,
    .arrTime = 0,
    .schedPolicy = SCHED_UNDEF      //用于多级队列
}; //task设计调度的一些参数的默认值

void _setTskPara(myTCB *task, tskPara *para){

    //本函数需要实现！！！
    //将para的中的参数值赋值给task的对应参数
    //注意para为null的处理
    //注意task_sched.h中schedule policy和scheme的定义有区别 所以para中的schedPolicy不能直接赋值给task

    if(para==NULL){
        task->para.arrTime=0;
        task->para.exeTime=MAX_EXETIME;
        task->para.priority=MAX_PRIORITY_NUM;
        task->para.schedPolicy=SCHED_UNDEF; //未使用
    }
    //initLeftExeTime_sjf(tsk);
    else {
        task->para.arrTime=para->arrTime;
        task->para.exeTime=para->exeTime;
        task->para.priority=para->priority;
        task->para.schedPolicy=SCHED_UNDEF; //未使用
    }
}

void initTskPara(tskPara *buffer){
    //本函数需要实现！！！
    //功能是将任务buffer的参数设置为默认值
    buffer->arrTime=0;
    buffer->exeTime=MAX_EXETIME;
    buffer->priority=MAX_PRIORITY_NUM;
    buffer->schedPolicy=SCHED_UNDEF; //未使用
}

void setTskPara(unsigned int option, unsigned int value, tskPara *buffer){
    //
    //设置task的设计调度的四个参数
    //option控制buffer的哪个参数要被赋值，value是具体的数值
    switch(option){
    case ARRTIME:buffer->arrTime=value;break;
    case PRIORITY:buffer->priority=value;break;
    case EXETIME:buffer->exeTime=value;break;
    case SCHED_POLICY:buffer->schedPolicy=value;break;
    default:;
    }

}

void getTskPara(unsigned option, unsigned int *para){
    //本函数需要实现！！！
    //查看task的设计调度的四个参数
    //option控制buffer的哪个参数要查看 赋值给para
    switch(option){
    case ARRTIME:*para= currentTsk->para.arrTime;break;//
    case PRIORITY:*para=currentTsk->para.priority;break;
    case EXETIME:*para=currentTsk->para.exeTime;break;
    case SCHED_POLICY:*para=currentTsk->para.schedPolicy;break;
    default:;
    }

}
