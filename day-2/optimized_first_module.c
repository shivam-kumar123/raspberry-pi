#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

__init int ex2_simple_module_init(void) {
  printk(KERN_ALERT "Inside the %s function sk\n", __FUNCTION__);
  return 0;
}

void ex1_simple_module_exit(void) {
  printk(KERN_ALERT "Exiting the %s function sk\n", __FUNCTION__);
}

// module_init and module_exit are macros, which help the compiler know which is the init and exit funtion
module_init(ex2_simple_module_init);
module_exit(ex1_simple_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shivam Kumar");
MODULE_DESCRIPTION("First module created by Shivam Kumar !");
