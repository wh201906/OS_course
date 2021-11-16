#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/types.h>

static int pidVal = 1;
module_param(pidVal, int, S_IRUGO);

static void ProcInfo_print(struct task_struct __rcu * proc, int level)
{
    int i;
    int firstChild = 1;
    struct list_head* pos;
    struct task_struct __rcu * tmp;
    i = level;
    while(i--)
        printk(KERN_CONT "|   ");

    printk(KERN_CONT "|---%-8d %s\n", proc->pid, proc->comm);
    list_for_each(pos, &proc->children)
    {
        // if(firstChild)
        //     printk("+++++\n");
        tmp = list_entry(pos, struct task_struct, sibling);
        ProcInfo_print(tmp, level + 1);
        firstChild = 0;
        //printk("%s\n", tmp->comm);
    }
    // pos = proc->sibling.next;
    // ProcInfo_print(list_entry(list_entry(pos, struct task_struct, sibling), level);
    
}

static int ProcInfo_init(void)
{
    struct pid * pidStruct;
    struct task_struct * taskStruct;
    struct task_struct * taskParent;
    printk("module ProcInfo loaded\n");
    printk("PID: %d\n", pidVal);
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
    taskParent = taskStruct->real_parent;
    if(taskParent != taskStruct)
        ProcInfo_print(taskParent, 0);
    else
        ProcInfo_print(taskParent, 1);
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