#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>

#define SUCCESS 0
#define DRIVER_NAME "sk-char-driver"
#define DRIVER_LICENSE "GPL"
#define DRIVER_AUTHOR "Shivam Kumar"
#define DRIVER_DESCRIPTION "Character Device Driver"

static int major_number = 0;
static int __init module_load (void);
static void __exit module_unload (void);
static int device_open (struct inode *, struct file *);
static ssize_t device_read (struct file *, char *, size_t, loff_t *);
static ssize_t device_write (struct file *, const char *, size_t, loff_t *);
static int device_release (struct inode *, struct file *);

static int device_open (struct inode* pinode, struct file* pfile) {
	try_module_get (THIS_MODULE);		
	printk (KERN_ALERT "Inside %s function\n", __FUNCTION__);
	return SUCCESS;
}

static ssize_t device_read (struct file *pfile, char *buffer, size_t length, loff_t *offset) {
	printk (KERN_ALERT "Inside %s function\n", __FUNCTION__);
	return length;
}

static ssize_t device_write (struct file *pfile, const char *buffer, size_t length, loff_t *offset) {
	printk (KERN_ALERT "Inside %s function\n", __FUNCTION__);	
	return length;
}

static int device_release (struct inode *pinode, struct file *pfile) {
	module_put (THIS_MODULE);
	printk (KERN_ALERT "Inside %s function\n", __FUNCTION__);
	return SUCCESS;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_release
};

static int __init module_load (void) {
	printk (KERN_ALERT "custom char driver: inside %s function\n", __FUNCTION__);
	major_number = register_chrdev (0, DRIVER_NAME, &fops);
	if (major_number < 0) {
		printk (KERN_ALERT "Registering %s driver with major number %d failed\n", DRIVER_NAME, major_number);
		return major_number;
	}
	printk (KERN_ALERT "Registering %s driver with major number %d was successful\n", DRIVER_NAME, major_number);
	printk (KERN_ALERT "Run this command: sudo mknod -m 666 /dev/%s c %d 0\n", DRIVER_NAME, major_number);
	return SUCCESS;
}

static void __exit module_unload (void) {
	printk (KERN_ALERT "custom char driver: inside %s function\n", __FUNCTION__);	
	unregister_chrdev (major_number, DRIVER_NAME);
}

module_init (module_load);
module_exit (module_unload);

MODULE_LICENSE (DRIVER_LICENSE);
MODULE_AUTHOR (DRIVER_AUTHOR);
MODULE_DESCRIPTION (DRIVER_DESCRIPTION);
