#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "kfifo_test.h"
#include "kfifo.h"

#define BUFFER_LEN 4

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(__typeof__(arr[0])))

#define PRINT_ARR(arr, len) \
({ \
    for(int i = 0;i < (len);i++) \
    { \
        printf("%d ", (arr)[i]); \
    } \
    printf("\n"); \
})

static void KFIFO_Print(struct __kfifo *kfifo)
{
    printf("kfifo info:\n");
    printf("kfifo.in = %d\n", kfifo->in);
    printf("kfifo.in & kfifo.mask = %d\n", kfifo->in & kfifo->mask);
    printf("kfifo.out = %d\n", kfifo->out);
    printf("kfifo.out & kfifo.mask = %d\n", kfifo->out & kfifo->mask);
    printf("kfifo.data = ");
    PRINT_ARR((uint8_t *)kfifo->data, (kfifo->mask + 1) * kfifo->esize);
}

void KFIFO_Test()
{
    kfifo_t kfifo;
    int kfifoBuffer[BUFFER_LEN] = {0};
    int inData[3] = {1, 2, 3};
    int outData[BUFFER_LEN] = {0};
    
    kfifo_init(&kfifo, kfifoBuffer, BUFFER_LEN, sizeof(int));

    printf("input data:");
    PRINT_ARR(inData, ARR_SIZE(inData));
    kfifo_in(&kfifo, inData, ARR_SIZE(inData));
    KFIFO_Print(&kfifo);
    printf("\n");

    kfifo_out(&kfifo, outData, ARR_SIZE(inData));
    printf("out data:");
    PRINT_ARR(outData, ARR_SIZE(inData));
    KFIFO_Print(&kfifo);
    printf("\n");

    printf("input data:");
    PRINT_ARR(inData, ARR_SIZE(inData));
    kfifo_in(&kfifo, inData, ARR_SIZE(inData));
    KFIFO_Print(&kfifo);
    printf("\n");

    kfifo_out(&kfifo, outData, ARR_SIZE(inData));
    printf("out data:");
    PRINT_ARR(outData, ARR_SIZE(inData));
    KFIFO_Print(&kfifo);
    printf("\n");
}
