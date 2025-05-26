#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

void exported_function (void);

void exported_function () {
  printk(KERN_ALERT "Inside the %s sk", __FUNCTION__);
}

int ex1_simple_module_init(void) {
  printk(KERN_ALERT "Inside the %s function sk\n", __FUNCTION__);
  return 0;
}

void ex1_simple_module_exit(void) {
  printk(KERN_ALERT "Exiting the %s function sk\n", __FUNCTION__);
}

// module_init and module_exit are macros, which help the compiler know which is the init and exit funtion
module_init(ex1_simple_module_init);
module_exit(ex1_simple_module_exit);

// EXPORT_SYMBOL can be used for both exporting function and global variables
EXPORT_SYMBOL(exported_function);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shivam Kumar");
MODULE_DESCRIPTION("First module created by Shivam Kumar !");
