/** pagequeue.c
 * ===========================================================
 * Name: Logan Johns, 23 April 2026
 * Section: CS483 / M4
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue *p = (PageQueue*) malloc(sizeof(PageQueue*));
    p->head = NULL;
    p->tail = NULL;
    p->size = 0;
    p->maxSize = maxSize;
    return p;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    //
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    PqNode *node = pq->tail;
    for (int d=0; d<pq->size; d++) {
        //check for hit
        if (node->pageNum == pageNum) {
            //hit
            if (pq->tail->pageNum != pageNum) { //check that node is not already the tail
                if (pq->head->pageNum == pageNum) { //check if the node to be moved is the head
                    pq->head = node->next; //update pq->head for new head node
                    pq->head->prev = NULL;

                } else {
                    node->next->prev = node->prev;
                    node->prev->next = node->next;
                    node->prev = pq->tail;
                }
                node->next = NULL;
                node->prev = pq->tail;
                pq->tail->next = node;
                pq->tail = node;
            }
            return d;
        }
        if (d<pq->size-1) { //ensure that we are not accessing the one before the head
            node = node->prev;
        }
    }
    //node not found. Add new node to tail of list.
    PqNode *newNode = (PqNode*) malloc(sizeof(PqNode*));
    newNode->pageNum = pageNum;
    newNode->next = NULL;
    if (pq->size==0) { //nothing in queue. New node is head and tail.
        pq->head = newNode;
        newNode->prev = NULL;
    } else {
        newNode->prev = pq->tail;
        newNode->prev->next = newNode;
    }
    pq->size++;
    if (pq->size > pq->maxSize) { //queue now too big. Remove head node.
        pq->head = pq->head->next;
        free(pq->head->prev);
        pq->size--;
    }
    pq->tail = newNode;

    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    for (int i=pq->size; i>0; i--) {
        PqNode* nodePtr = pq->tail;
        PqNode* prevNode = nodePtr->prev;
        pq->tail = prevNode;
        pq->size--;
        free(nodePtr);
    }
    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
    PqNode* nodePtr = pq->head;
    printf("HEAD -> ");
    for (int i=0; i<pq->size; i++) {
        printf("%ld ",nodePtr->pageNum);
        nodePtr = nodePtr->next;
    }
    printf("<- TAIL\n");
}
