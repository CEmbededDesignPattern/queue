#ifndef _QUEUE_H_
#define _QUEUE_H_

#define QUEUE_SIZE 10
#define QUEUE_SIZE_REAL (QUEUE_SIZE + 1)
/* class Queue */
typedef struct Queue Queue;
struct Queue {
    int buffer[QUEUE_SIZE];
    int size;
    int head;
    int tail;

    int (*isFull) (const Queue *const me);
    int (*isEmpty) (const Queue *const me);
    int (*getSize) (const Queue *const me);
    void (*insert) (Queue *const me, int k);
    int (*remove) (Queue *const me);
};


/* Constructors and destructors */
void Queue_Init(Queue *const me,
            int (*isFullfunc) (const Queue *const me),
            int (*isEmptyfunc) (const Queue *const me),
            int (*getSizefunc) (const Queue *const me),
            void (*insertfunc) (Queue *const me, int k),
            int (*removefunc) (Queue *const me));
void Queue_Cleanup(Queue *const me);

/* Operations */
int Queue_isFull(const Queue *const me);
int Queue_isEmpty(const Queue *const me);
int Queue_getSize(const Queue *const me);
void Queue_insert(Queue *const me, int k);
int Queue_remove(Queue *const me);

Queue *Queue_Create(void);
void Queue_Destroy(Queue *const me);

#endif
