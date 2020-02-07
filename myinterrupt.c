
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/tty.h>
#include <linux/vmalloc.h>

#include "mypcb.h"


extern tPCB task[MAX_TASK_NUM];
extern tPCB* my_current_task;

extern volatile int my_need_sched;

volatile int time_count =0;

void my_timer_handler(void){

    if(time_count %10000 == 0 && my_need_sched != 1){
        printk(KERN_NOTICE  ">>> Timer handler here <<<\n");

        my_need_sched = 1;
    }
    time_count ++;

    return;
}


void my_schedule(void ){

    tPCB* next;
    tPCB* prev;


    if(my_current_task == NULL || my_current_task->next == NULL){
        return;
    }

    printk(KERN_NOTICE "MY_SCHEDULE\n");


    /* schedule */

    next = my_current_task->next;
    prev = my_current_task;


    if(next->state ==0 ){
        my_current_task = next;

        printk (KERN_NOTICE ">>>SWitch %d to %d <<<\n", prev->pid, next->pid);

        asm volatile (
            "pushl %%ebp \n\t"
            "movl %%esp, %0 \n\t"
            "movl %2, %%esp\n\t"
            "movl $1f, %1\n\t"
            "pushl %3\n\t"
            "ret \n\t"
            "1:\t"
            "popl %%ebp \n\t"
            :"=m" (prev->thread.sp),"=m"(next->thread.ip)
            :"m" (next->thread.sp),"m" (next->thread.ip)
            :);

    }
    return;
}