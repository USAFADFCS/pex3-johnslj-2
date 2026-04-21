/** pagequeue.c
 * ===========================================================
 * Name: Logan Johns, 15 April 2026
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
            if (node->next != NULL) { //check that node is not already the tail
                if (node->prev == NULL) { //check if the node to be moved is the head
                    pq->head = node->next; //update pq->head for new head node
                }
                node->next->prev = node->prev;
                node->prev->next = node->next;
                node->prev = pq->tail;
                pq->tail->next = node;
                pq->tail = node;
            }
            return d;
        }
        node = node->prev;
    }
    //node not found. Add new node to tail of list.
    PqNode *newNode = (PqNode*) malloc(sizeof(PqNode*));
    newNode->pageNum = pageNum;
    newNode->next = NULL;
    newNode->prev = pq->tail;
    pq->size += 1;
    if (pq->size > pq->maxSize) { //queue now too big. Remove head node.
        pq->head = pq->head->next;
        free(pq->head->prev);
        pq->size -= 1;
    }

    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    PqNode *node = pq->head;
    for (int i=0; i<pq->size-1; i++) {
        node = node->next;
        free(node->prev);
    }
    free(node); //final node not freed in for loop
    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
}
