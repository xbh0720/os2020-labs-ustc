#include "../include/dLinkList.h"

#define NULL (void*)0

//链表头节点的prev指向尾结点，尾结点的next指向头节点。

/*for double linked list*/
void dLinkNodeInit(dLink_node *node){
    node->prev = NULL;
    node->next = NULL;
}

/* dLinkListInit
 * list: a new list to be initialized
 */
void dLinkListInit(dLinkedList *list){
    list->prev = list;
    list->next = list;    
}

dLink_node *dLinkListFirstNode(dLinkedList *list){
    return list->next;  //may be the head
}

dLink_node *dLinkListLastNode(dLinkedList *list){
    return list->prev;  //may be the head
}

/* dLinkInsertBefore
 * list: the list
 * old: a node in the list, may the head
 * toBeInserted: a valid node
 * 在某节点之前插入新节点
 */
void dLinkInsertBefore(dLinkedList *list, dLink_node *old, dLink_node *toBeInserted){
    toBeInserted->next = old;
    toBeInserted->prev = old->prev;

    old->prev->next = toBeInserted;
    old->prev = toBeInserted;
}
//在某节点之后插入新节点
void dLinkInsertAfter(dLinkedList *list, dLink_node *old, dLink_node *toBeInserted){
    toBeInserted->next = old->next;
    toBeInserted->prev = old;

    old->next->prev = toBeInserted;
    old->next = toBeInserted;
}
//删除某节点
void dLinkDelete(dLinkedList *list, dLink_node *toBeDeleted){
    dLink_node *before, *after;

    if(toBeDeleted == list) return;  //never delete head

    before = toBeDeleted->prev;
    after = toBeDeleted->next;

    before->next = after;
    after->prev =before;
}

/*==============================================
 *=====FIFO: head<=>node<=>node<=>tail(head)====
 *==============================================*/
void dLinkInsertFifo(dLinkedList *list, dLink_node *toBeInserted){
    /* insert as tail */
    dLinkInsertBefore(list, (dLink_node*)list, toBeInserted);
}

void dLinkDeleteFifo(dLinkedList *list){
    /* delete the first node (not head) */
    dLinkDelete(list, list->next);
}

/*==========================================================
 *=====FIFOWithTail: head<=>node<=>node<=>tail(not head)====
 *==========================================================*/
void dLinkInsertFifoBeforeTail(dLinkedList *list, dLink_node *toBeInserted){
    /* insert before tail */
    dLinkInsertBefore(list, list->prev, toBeInserted);
}