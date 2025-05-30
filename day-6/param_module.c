#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>

static int __init load_module(void);
static void __exit unload_module(void);

int __initdata count = 1;

module_param(count, int, 0);

static int __init load_module (void) {
	printk(KERN_ALERT "function: %s", __FUNCTION__);
	for (int i=0;i<count;i++) {
		printk(KERN_ALERT "inside loop: %d", i);
	}
	return 0;
}

static void __exit unload_module (void) {
	printk(KERN_ALERT "function: %s", __FUNCTION__);
}

module_init (load_module);
module_exit (unload_module);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Shivam Kumar");
MODULE_DESCRIPTION ("Passing CLI arguments to kernel modules");
