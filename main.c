#include <stdio.h>
#include <check.h>
#include "queue.h"
#include "cachedQueue.h"

START_TEST(test_queue_push_and_pop) 
{
    int j, k, h, t;
    /* test normal Queue */
    printf("=========================start test queue_push_and_pop......\n");
    Queue *myQ;
    myQ = Queue_Create();
    k = 1000;
    
    for (j = 0; j < QUEUE_SIZE; j++)
    {
        h = myQ->head;
        myQ->insert(myQ, k);
        printf("inserting %d at position %d, size = %d\n", k--, h, myQ->getSize(myQ));
    }

    printf("Inserted %d elements\n", myQ->getSize(myQ));

    for (j = 0; j < QUEUE_SIZE; j++)
    {
        t = myQ->tail;
        k = myQ->remove(myQ);
        printf("REMOVING %d at position %d, size = %d\n", k, t, myQ->getSize(myQ));
    }

    printf("Last item removed = %d\n", k);
    printf("Current queue size %d\n", myQ->getSize(myQ));
    printf("=========================end test queue_push_and_pop\n");
}
END_TEST

START_TEST(test_cachedQueue_push_and_pop) 
{
    int j, k, h, d, t;
    printf("=========================start test cachedQueue_push_and_pop.....\n");
    CachedQueue *myCachedQ;
    myCachedQ = CachedQueue_Create();
    ck_assert_msg(myCachedQ != NULL, "error, myCachedQ is NULL");
    k = 10000;

    for (j = 0; j < QUEUE_SIZE + 1; j++)
    {
        h = myCachedQ->queue->head;
        d = myCachedQ->numberElementsOnDisk;
        myCachedQ->insert(myCachedQ, k);//9990
        printf("inserting %d at position %d - %d, size %d\n", k--, d, h, myCachedQ->getSize(myCachedQ));
    }

    printf("Inserted %d elements, OnDisk %d\n", myCachedQ->getSize(myCachedQ), myCachedQ->numberElementsOnDisk);

    for (j = 0; j < QUEUE_SIZE + 1; j++)
    {
        k = myCachedQ->remove(myCachedQ);
        printf("REMOVING %d, size = %d\n", k, myCachedQ->getSize(myCachedQ));
    }
    printf("Last item removed = %d\n", k);
    printf("Current cachedQueue size %d\n", myCachedQ->getSize(myCachedQ));
    printf("=========================end test cachedQueue_push_and_pop\n");
}
END_TEST

static Suite *queue_suite(void)
{
    Suite *s;
    TCase *tc_queue;
    TCase *tc_cachedQueue;

    s = suite_create("QueueAndCachedQueue");

    tc_queue = tcase_create("queue");
    tcase_add_test(tc_queue, test_queue_push_and_pop);
    suite_add_tcase(s, tc_queue);

    tc_cachedQueue = tcase_create("cachedQueue");
    tcase_add_test(tc_cachedQueue, test_cachedQueue_push_and_pop);
    suite_add_tcase(s, tc_cachedQueue);

    return s;
}

// int main(int argc, char const *argv[])
// {
//     SRunner *sr;
//     Suite *s;

//     s = queue_suite();
//     sr = srunner_create(s);

//     srunner_run_all(sr, CK_NORMAL);
//     srunner_ntests_failed(sr);
//     srunner_free(sr);

//     return 0;
// }


int main(int argc, char const *argv[])
{
    int j, k, h, d, t;
    printf("=========================start test cachedQueue_push_and_pop.....\n");
    CachedQueue *myCachedQ;
    myCachedQ = CachedQueue_Create();
    k = 10000;

    for (j = 0; j < QUEUE_SIZE * 2 + 1; j++)
    {
        h = myCachedQ->queue->head;
        d = myCachedQ->numberElementsOnDisk;
        myCachedQ->insert(myCachedQ, k);//9990
        printf("inserting %d at position %d - %d, size %d\n", k--, d, h, myCachedQ->getSize(myCachedQ));
    }

    printf("Inserted %d elements, OnDisk %d\n", myCachedQ->getSize(myCachedQ), myCachedQ->numberElementsOnDisk);

    for (t = 0; t < QUEUE_SIZE * 2 + 1; t++)
    {
        k = myCachedQ->remove(myCachedQ);
        printf("REMOVING %d, size = %d\n", k, myCachedQ->getSize(myCachedQ));
    }
    printf("Last item removed = %d\n", k);
    printf("Current cachedQueue size %d\n", myCachedQ->getSize(myCachedQ));
    printf("=========================end test cachedQueue_push_and_pop\n");
    return 0;
}
