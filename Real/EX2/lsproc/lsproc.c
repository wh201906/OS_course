#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>

static int lsProc_init(void)
{
    struct task_struct *p;
    printk("module lsProc loaded\n");
    printk("----------------------------\n");
    printk("pid    prio    state    name\n");
    printk("----------------------------\n");
    for_each_process(p)
    {
        if (p->mm == NULL) // kernel thread
            printk("%-6d %-7u %-8ld %s\n", p->pid, p->rt_priority, p->state, p->comm);
    }
    printk("----------------------------\n");
    return 0;
}

static void lsProc_exit(void)
{
    printk("module lsProc unloaded\n");
}

module_init(lsProc_init);
module_exit(lsProc_exit);
MODULE_LICENSE("GPL");