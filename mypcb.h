// #ifndef MYPCB_H
// #define MYPCB_H

#include <linux/types.h>
#define MAX_TASK_NUM 4
#define KERNEL_STACK_SIZE 8*1024

struct Thread
{
    uintptr_t ip;
    uintptr_t sp;

};

typedef struct PCB
{
    int pid;
    volatile long state;
    char stack[KERNEL_STACK_SIZE];
    struct Thread thread;
    unsigned long task_entry;
    struct PCB * next;
    
} tPCB;

void my_schedule(void);




// #endif

