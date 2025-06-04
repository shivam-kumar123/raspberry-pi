#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h> /* for character driver support */

static int __init module_load (void);
static void __exit module_unload (void);
int custom_open (struct inode *, struct file *);
ssize_t custom_read (struct file *, char __user *, size_t, loff_t *);
ssize_t custom_write (struct file *, const char __user *, size_t, loff_t *);
int custom_close (struct inode *, struct file *);

/* definitions of these function can be found at the below location:
 * path: /lib/modules/$(uname -r)/build/include/linux/fs.h */
int custom_open (struct inode *pinode, struct file *pfile) {
	printk (KERN_ALERT "inside %s function", __FUNCTION__);
	return 0;
}

ssize_t custom_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset) {
	printk (KERN_ALERT "inside %s function", __FUNCTION__);
	return 0;
}

ssize_t custom_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) {
	printk (KERN_ALERT "inside %s function", __FUNCTION__);
	return length;
}

int custom_close (struct inode *pinode, struct file *pfile) {
	printk (KERN_ALERT "inside %s function", __FUNCTION__);
	return 0;
}

/* To hold file operations performed on this device */
/* This struct file_operations is defined in the <linux/fs.h> */
struct file_operations char_device_driver_file_operations = {
	.owner = THIS_MODULE,
	.open = custom_open,
	.read = custom_read,
	.write = custom_write,
	.release = custom_close,
};

static int __init module_load (void) {
	printk (KERN_ALERT "%s function is initialized in char_device_driver", __FUNCTION__);
	/* Register with the kernel and indicate that we are registering a character device driver */
	register_chrdev (/* Major Number */ 100,
			/* Name */ "simple char driver",
			/* File Operations */ &char_device_driver_file_operations 
			);
	return 0;
}

static void __exit module_unload (void) {
	printk (KERN_ALERT "%s function is initialized in char_device_driver", __FUNCTION__);
	/* unregister character driver with kernel */
	unregister_chrdev (240, "simple char driver");
}

module_init (module_load);
module_exit (module_unload);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Shivam Kumar");
MODULE_DESCRIPTION ("Simple Character Driver");
