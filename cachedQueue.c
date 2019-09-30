#include <stdlib.h>
#include <string.h>
#include "cachedQueue.h"

/* Constructors and Destructors */
void CachedQueue_Init(CachedQueue *const me, char *fName,
            int (*isFullfunc) (const CachedQueue *const me),
            int (*isEmptyfunc) (const CachedQueue *const me),
            int (*getSiezfunc) (const CachedQueue *const me),
            void (*insertfunc) (CachedQueue *const me, int k),
            int (*removefunc) (CachedQueue *const me),
            void (*flushfunc) (CachedQueue *const me),
            void (*loadfunc) (CachedQueue *const me))
{
    /* initialize base class */
    me->queue = Queue_Create(); /* queue member must use its original functions */

    /* initalize subclass attributes */
    me->numberElementsOnDisk = 0;
    strcpy(me->filename, fName);
    me->fp = fopen(me->filename, "w+");
    if (me->fp != NULL)
    {
        goto err;
    }
    /* initalize aggregates */
    me->outputQueue = Queue_Create();

    /* initalize subclass virtual operatios */
    me->isFull = isFullfunc;
    me->isEmpty = isEmptyfunc;
    me->getSize = getSiezfunc;
    me->insert = insertfunc;
    me->remove = removefunc;
    me->flush = flushfunc;
    me->load = loadfunc;
    return;
err:
    Queue_Destroy(me->queue);
}

void CachedQueue_Cleanup(CachedQueue *const me)
{
    Queue_Cleanup(me->queue);
}

/* Operatios */
int CachedQueue_isFull(const CachedQueue *const me)
{
    return  me->queue->isFull(me->queue) && 
            me->outputQueue->isFull(me->outputQueue);
}
int CachedQueue_isEmpty(const CachedQueue *const me)
{
    return  me->queue->isEmpty(me->queue) && 
            me->outputQueue->isEmpty(me->outputQueue) &&
            (me->numberElementsOnDisk == 0);
}
int CachedQueue_getSize(const CachedQueue *const me)
{
    return  me->queue->getSize(me->queue) + 
            me->outputQueue->getSize(me->outputQueue) + 
            me->numberElementsOnDisk;
}
/**
 * operation insert(int) 
 * Insert Algorithm:
 * if the queue is full,
 *      call flush to write out the queue to disk and reset the queue
 * end if
 * inset the data into queue
 * */
void CachedQueue_insert(CachedQueue *const me, int k)
{
    if (me->queue->isFull(me->queue))
        me->flush(me);
    me->queue->insert(me->queue, k);
}
/**
 * operation remove
 * remove algorithm:
 * if there is data in the outputQueue,
 *      remove it from the outputQueue
 * else if there is data on disk
 *      call load to bring it into the outputQueue
 *      remove it from the outputQueue
 * else if there is data int the queue
 *      remove it from there
 *      (if there is no data to remove the return sentinel value)
 */
int CachedQueue_remove(CachedQueue *const me)
{
    if (!me->outputQueue->isEmpty(me->outputQueue))
    {
        return me->outputQueue->remove(me->outputQueue);
    }
    else if (me->numberElementsOnDisk > 0)
    {
        me->load(me);
        return me->outputQueue->remove(me->outputQueue);
    }
    else
    {
        return me->queue->remove(me->queue);
    }
}


/**
 * operation flush
 * Precondition:this is called only when queue is full and filename is valid
 * 
 * flush algorithm:
 * if file is not open, the open file
 * while not queue->isEmpty()
 *      queue->remove();
 *      write data to disk
 *      numberElementsOnDisk++
 * end while
*/
void CachedQueue_flush(CachedQueue *const me)
{
    int val;
    char buf[8];
    if (me->fp != NULL)
    {
        while (!(me->queue->isEmpty(me->queue)))
        {
            val = me->queue->remove(me->queue);
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%d", val);
            fputs(buf, me->fp);
            ++me->numberElementsOnDisk;
        }
    }
}
/**
 * operation load
 * Precondition:this is called only when outputQueue is empty and filename is valid
 * 
 * load algorithm:
 * while (!outputQueue->isFull && (numberElementsOnDisk > 0))
 *      read from start of file(i.e.,oldest datum)
 *      numberElementsOnDisk--;
 *      outputQueue->insert();
 * end while
*/
void CachedQueue_load(CachedQueue *const me)
{
    int val;
    int i;
    char buf[8];
    if (me->fp != NULL)
    {
        // load data from disk to outputQueue
        while (!(me->outputQueue->isFull(me->outputQueue)) && (me->numberElementsOnDisk > 0))
        {
            memset(buf, 0, sizeof(buf));
            fgets(buf, sizeof(buf), me->fp);
            --me->numberElementsOnDisk;
            val = atoi(buf);
            me->outputQueue->insert(me->outputQueue, val);
        }

        // invert outputQueue data
        for (i = 0; i < me->outputQueue->getSize(me->outputQueue); i++)
        {
            me->outputQueue->insert(me->outputQueue, me->outputQueue->remove(me->outputQueue));
        }
    }
}

CachedQueue *CachedQueue_Create(void)
{
    CachedQueue *me = (CachedQueue *)malloc(sizeof(CachedQueue));
    if (me != NULL)
    {
        CachedQueue_Init(me, "./queuebuffer.dat", 
                            CachedQueue_isFull, CachedQueue_isEmpty,
                            CachedQueue_getSize, CachedQueue_insert,
                            CachedQueue_remove, CachedQueue_flush,
                            CachedQueue_load);
    }
    return me;
}
void CachedQueue_Destroy(CachedQueue *const me)
{
    if (me != NULL)
    {
        CachedQueue_Cleanup(me);
        free(me);
    }
}
