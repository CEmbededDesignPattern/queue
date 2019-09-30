#ifndef _CACHEDQUEUE_H_
#define _CACHEDQUEUE_H_
#include "queue.h"
#include "stdio.h"

typedef struct CachedQueue CachedQueue;
struct CachedQueue {
    Queue *queue; /* base class */
    /* new attributes */
    char filename[80];
    FILE *fp;
    int numberElementsOnDisk;
    /* aggregation in subclass */
    Queue *outputQueue;

    /* inherited virtual functions */
    int (*isFull) (const CachedQueue *const me);
    int (*isEmpty) (const CachedQueue *const me);
    int (*getSize) (const CachedQueue *const me);
    void (*insert) (CachedQueue *const me, int k);
    int (*remove) (CachedQueue *const me);
    /* new virtual functions */
    void (*flush) (CachedQueue *const me);
    void (*load) (CachedQueue *const me);
};

/* Constructors and Destructors */
void CachedQueue_Init(CachedQueue *const me, char *fName,
            int (*isFullfunc) (const CachedQueue *const me),
            int (*isEmptyfunc) (const CachedQueue *const me),
            int (*getSiezfunc) (const CachedQueue *const me),
            void (*insertfunc) (CachedQueue *const me, int k),
            int (*removefunc) (CachedQueue *const me),
            void (*flushfunc) (CachedQueue *const me),
            void (*loadfunc) (CachedQueue *const me));
void CachedQueue_Cleanup(CachedQueue *const me);

/* Operatios */
int CachedQueue_isFull(const CachedQueue *const me);
int CachedQueue_isEmpty(const CachedQueue *const me);
int CachedQueue_getSize(const CachedQueue *const me);
void CachedQueue_insert(CachedQueue *const me, int k);
int CachedQueue_remove(CachedQueue *const me);
void CachedQueue_flush(CachedQueue *const me);
void CachedQueue_load(CachedQueue *const me);

CachedQueue *CachedQueue_Create(void);
void CachedQueue_Destroy(CachedQueue *const me);

#endif
