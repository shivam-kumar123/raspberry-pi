#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

int ex1_simple_module_init(void) {
  printk(KERN_ALERT "Inside the %s function sk\n", __FUNCTION__);
  return 0;
}


// module_init and module_exit are macros, which help the compiler know which is the init and exit funtion
module_init(ex1_simple_module_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shivam Kumar");
MODULE_DESCRIPTION("First module created by Shivam Kumar !");
