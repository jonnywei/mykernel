#include <linux/printk.h>

#include "mypcb.h"

tPCB task[MAX_TASK_NUM];
tPCB* my_current_task = NULL;

volatile int my_need_sched = 0;

static void my_process(void);

void __init my_start_kernel(void){


    int pid = 0;

    int i;
    // init pid 0
    task[pid].pid = pid;
    task[pid].state = 0;
    task[pid].task_entry = task[pid].thread.ip = (unsigned long) my_process;
    task[pid].thread.sp = (unsigned long) &task[pid].stack[KERNEL_STACK_SIZE-1];
    task[pid].next = &task[pid];

    /*fork more process*/
    for(i = 1; i < MAX_TASK_NUM; i++){
        memcpy(&task[i], &task[0],sizeof(tPCB));
        task[i].pid = i;
        task[i].thread.sp = (unsigned long ) (&task[i].stack[KERNEL_STACK_SIZE-1]);

        task[i].next = task[i-1].next;

        task[i -1].next = &task[i];
    }

    pid =0;

    my_current_task = &task[pid];


    asm volatile (
        "movl %1, %%esp \n\t"
        "pushl %1 \n\t"
        "pushl %0 \n\t"
        "ret \n\t"

        :
        :"c" (task[pid].thread.ip), "d" (task[pid].thread.sp)
        :);

}


int count =0;

void my_process (void ){
    while (1)
    {   
        /* code */

        count ++;

        if(count % 1000000000 == 0){
            printk( KERN_NOTICE "This is proces %d \n", my_current_task->pid);

            if(my_need_sched == 1){

                my_need_sched = 0;

                my_schedule();
            }
            printk(KERN_NOTICE "This is process %d + \n",my_current_task->pid);

        }
    }
    
}