#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>

#define SUCCESS 0
#define RING_BUFFER_SIZE 1024
#define DRIVER_NAME "sk_ring_buffer"
#define DRIVER_LICENSE "GPL"
#define DRIVER_AUTHOR "	Shivam Kumar"
#define DRIVER_DESCRIPTION "Character Device Driver, Which Replicates The Kernel Ring Buffer"

static int major_number = 0;
static char device_buffer[RING_BUFFER_SIZE];
static DEFINE_MUTEX (ring_buffer_mutex);

static int __init module_load (void);
static void __exit module_unload (void);
static int device_open (struct inode *, struct file *);
static ssize_t device_read (struct file *, char *, size_t, loff_t *);
static ssize_t device_write (struct file *, const char *, size_t, loff_t *);
static int device_release (struct inode *, struct file *);

static int device_open (struct inode* pinode, struct file* pfile) {
	return SUCCESS;
}
static ssize_t device_read (struct file* pfile, char* buffer, size_t length, loff_t* offset) {
	return SUCCESS;
}
static ssize_t device_write (struct file* pfile, const char* buffer, size_t length, loff_t* offset) {
	return SUCCESS;
}
static int device_release (struct inode* pinode, struct file* pfile) {
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
	printk (KERN_ALERT "%s: Inside %s function\n", DRIVER_NAME, __FUNCTION__);
	return SUCCESS;
}

static void __exit module_unload (void) {
	printk (KERN_ALERT "%s: Inside %s function\n", DRIVER_NAME, __FUNCTION__);	
}

module_init (module_load);
module_exit (module_unload);

MODULE_LICENSE (DRIVER_LICENSE);
MODULE_AUTHOR (DRIVER_AUTHOR);
MODULE_DESCRIPTION (DRIVER_DESCRIPTION);
