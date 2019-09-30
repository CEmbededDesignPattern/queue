#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "cachedQueue.h"

/* Constructors and Destructors */
void CachedQueue_Init(CachedQueue *const me, char *fName,
            int (*isFullfunc) (const CachedQueue *const me),
            int (*isEmptyfunc) (const CachedQueue *const me),
            int (*getSizefunc) (const CachedQueue *const me),
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
    if (access(me->filename, F_OK) == 0) //file exist
    {
        remove(me->filename);
    }
    /* initalize aggregates */
    me->outputQueue = Queue_Create();
    /* initalize subclass virtual operatios */
    me->isFull = isFullfunc;
    me->isEmpty = isEmptyfunc;
    me->getSize = getSizefunc;
    me->insert = insertfunc;
    me->remove = removefunc;
    me->flush = flushfunc;
    me->load = loadfunc;
    return;
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
    if (!(me->outputQueue->isEmpty(me->outputQueue)))
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
    FILE *fp = NULL;
    int val;
    char buf[8];
    fp = fopen(me->filename, "a"); //O_WRONLY | O_CREAT | O_APPEND

    if (fp != NULL)
    {
        while (!(me->queue->isEmpty(me->queue)))
        {
            val = me->queue->remove(me->queue);
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%d\n", val);
            fputs(buf, fp);
            ++me->numberElementsOnDisk;
        }

        fclose(fp);
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
    FILE *fp = NULL, *fp_tmp = NULL;
    int val;
    char buf[8];
    fp = fopen(me->filename, "r+"); //O_RDWR
    fp_tmp = fopen("./tmp.dat", "a+");

    if (fp != NULL && fp_tmp != NULL)
    {
        // load data from disk to outputQueue
        while (!(me->outputQueue->isFull(me->outputQueue)) && (me->numberElementsOnDisk > 0))
        {
            memset(buf, 0, sizeof(buf));
            fgets(buf, sizeof(buf), fp);
            --me->numberElementsOnDisk;
            val = atoi(buf);
            me->outputQueue->insert(me->outputQueue, val);
        }

        while (1)
        {
            memset(buf, 0, sizeof(buf));
            if (fgets(buf, sizeof(buf), fp) == NULL)
                break;

            fputs(buf, fp_tmp);
        }

        fclose(fp);
        fclose(fp_tmp);

        //remove file
        remove(me->filename);
        //rename file
        rename("./tmp.dat", me->filename);
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
