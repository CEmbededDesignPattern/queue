#include "queue.h"
#include <stdlib.h>

/* Constructors and destructors */
void Queue_Init(Queue *const me,
            int (*isFullfunc) (const Queue *const me),
            int (*isEmptyfunc) (const Queue *const me),
            int (*getSizefunc) (const Queue *const me),
            void (*insertfunc) (Queue *const me, int k),
            int (*removefunc) (Queue *const me))
{
    /* initialize attributes */
    me->head = 0;
    me->size = 0;
    me->tail = 0;

    /* initialize member function pointers */
    me->isFull = isFullfunc;
    me->isEmpty = isEmptyfunc;
    me->getSize = getSizefunc;
    me->insert = insertfunc;
    me->remove = removefunc;
}
void Queue_Cleanup(Queue *const me)
{

}

/* Operations */
int Queue_isFull(const Queue *const me)
{
    return (me->head + 1) % QUEUE_SIZE_REAL == me->tail;
}
int Queue_isEmpty(const Queue *const me)
{
    return me->tail == me->head;
}
int Queue_getSize(const Queue *const me)
{
    return me->size;
}
void Queue_insert(Queue *const me, int k)
{
    if (!me->isFull(me))
    {
        me->buffer[me->head] = k;
        me->head = (me->head + 1) % QUEUE_SIZE_REAL;
        ++me->size;
    }
}
int Queue_remove(Queue *const me)
{
    int value = -9999; /* sentinel value */
    if (!me->isEmpty(me))
    {
        value = me->buffer[me->tail];
        me->tail = (me->tail + 1) % QUEUE_SIZE_REAL;
        --me->size;
    }
    return value;
}

Queue *Queue_Create(void)
{
    Queue *me = (Queue *)malloc(sizeof(Queue));
    if (me != NULL)
    {
        Queue_Init(me, Queue_isFull, Queue_isEmpty, Queue_getSize, Queue_insert, Queue_remove);
    }
    return me;
}
void Queue_Destroy(Queue *const me)
{
    if (me != NULL)
    {
        Queue_Cleanup(me);
        free(me);
    }
}
