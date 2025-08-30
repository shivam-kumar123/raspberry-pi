#include <linux/init.h>       // For module init/exit macros
#include <linux/module.h>     // Core header for modules
#include <linux/kernel.h>     // printk
#include <linux/sched.h>      // For current, task_struct

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shivam");
MODULE_DESCRIPTION("Demo module using current pointer");

static int __init hello_init(void)
{
    printk(KERN_INFO "Loaded: The process is \"%s\" (pid %i)\n",
           current->comm, current->pid);
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye from process \"%s\" (pid %i)\n",
           current->comm, current->pid);
}

module_init(hello_init);
module_exit(hello_exit);

