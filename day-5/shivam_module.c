/* simplest kernel module */

#include<linux/module.h> /* Needed by all kernel modules */
#include<linux/kernel.h> /* Needed for specifying log level in printk (KERN_ALERT, KERN_INFO, ...) */

static int __init init_module_sk (void);
static void __exit cleanup_module_sk (void);

static int __init init_module_sk (void) {
	printk(KERN_ALERT "Inside %s function", __FUNCTION__);
	// a non-zero value indicates init_module failed and module cannot be loaded
	return 0;
}

static void __exit cleanup_module_sk (void) {
	printk(KERN_ALERT "Inside %s function", __FUNCTION__);
}

module_init (init_module_sk);
module_exit (cleanup_module_sk);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Shivam Kumar");
MODULE_DESCRIPTION ("optimized simple kernel module");
