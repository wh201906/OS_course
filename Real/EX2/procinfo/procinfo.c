#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/types.h>

static int pidVal = 1;
module_param(pidVal, int, S_IRUGO);

static int ProcInfo_init(void)
{
    struct pid * pidStruct;
    struct task_struct * taskStruct;
    printk("module ProcInfo loaded\n");
    pidStruct = find_get_pid(pidVal);
    if(pidStruct == NULL)
    {
        printk("Process doesn't exist!");
        return 0;
    }
    taskStruct = pid_task(pidStruct, PIDTYPE_PID);
    if(taskStruct == NULL)
    {
        printk("Process doesn't exist!");
        return 0;
    }
    printk("%-6d %-7u %-8ld %s\n", taskStruct->pid, taskStruct->rt_priority, taskStruct->state, taskStruct->comm);
    printk("----------------------------\n");
    return 0;
}

static void ProcInfo_exit(void)
{
    printk("module ProcInfo unloaded\n");
}

module_init(ProcInfo_init);
module_exit(ProcInfo_exit);
MODULE_LICENSE("GPL");