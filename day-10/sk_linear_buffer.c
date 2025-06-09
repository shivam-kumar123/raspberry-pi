#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>

#define SUCCESS 0
#define LINEAR_BUFFER_SZ 1024
#define DRIVER_NAME "sk_linear_buffer"
#define DRIVER_LICENSE "GPL"
#define DRIVER_AUTHOR "Shivam Kumar"
#define DRIVER_DESCRIPTION "Character Device Driver For linear buffer"

// static variables
static int major_number = 0;
static int used_buffer_size = 0;
static char device_buffer[LINEAR_BUFFER_SZ];
/* below line creates a usable mutex */
static DEFINE_MUTEX(linear_buffer_mutex);

// static functions
static int __init module_load (void);
static void __exit module_unload (void);
static int device_open (struct inode *, struct file *);
static ssize_t device_read (struct file *, char *, size_t, loff_t *);
static ssize_t device_write (struct file *, const char *, size_t, loff_t *);
static int device_release (struct inode *, struct file *);

static int device_open (struct inode* pinode, struct file* pfile) {
	printk (KERN_ALERT "Inside %s function\n", __FUNCTION__);
	/* if mutex_trylock (&mutex) returns 0, this means some process is already using the mutex */
	if (mutex_trylock (&linear_buffer_mutex) == 0) {
		printk (KERN_ALERT "Driver \"%s\" is busy\n", DRIVER_NAME);
		/* EBUSY is a macro, It stands for Error: BUSY
		 * - is used because Linux kernel functions typically return negative error codes to indicate failure. */
		return -EBUSY;
	}
	try_module_get (THIS_MODULE);
	return SUCCESS;
}

static ssize_t device_read (struct file *pfile, char *buffer, size_t length, loff_t *offset) {
	printk (KERN_ALERT "Inside %s function\n", __FUNCTION__);
	size_t bytes_to_read;
    	size_t remaining_bytes;
	// [H][E][L][L][O]
	//  0  1  2  3  4  5(offset)
	//  in the above case we cant read, as offset pointer is beyound the data
	if (*offset >= used_buffer_size) {
        	printk (KERN_ALERT "Read offset %lld is beyond buffer size %d\n", *offset, used_buffer_size);
        	return 0; // EOF
    	}	
	remaining_bytes = used_buffer_size - *offset;
	bytes_to_read = (length < remaining_bytes) ? length : remaining_bytes;
	// Copy data from kernel space to user space
    	/* ARGUMENTS copy_to_user()
	1. Destination address, in user space.
	2. Source address, in kernel space.
	3. Number of bytes to copy.  */	
	if (copy_to_user (buffer, device_buffer + *offset, bytes_to_read)) {
        	printk (KERN_ALERT "Failed to copy data from kernel space to user space\n");
        	return -EFAULT;
    	}
	*offset += bytes_to_read;
	printk(KERN_ALERT "Read %zu bytes from offset %lld\n", bytes_to_read, *offset - bytes_to_read);
	return bytes_to_read;
}

static ssize_t device_write (struct file *pfile, const char *buffer, size_t length, loff_t *offset) {
	printk (KERN_ALERT "Inside %s function\n", __FUNCTION__);
	size_t space_left = LINEAR_BUFFER_SZ - used_buffer_size;
	if (length > space_left) {
		printk (KERN_ALERT "Not enough space in buffer. Available: %zu bytes, Requested: %zu bytes\n", space_left, length);
		return -ENOSPC;
	}
	/* copy_from_user(next_free_space_in_buffer, user_space_buffer_ length) copy data from user space to kernel space buffer */
	// on success copy_from_user returns 0
	/* ARGUMENTS copy_from_user()
	1. Destination address, in kernel space.
	2. Source address, in user space.
	3. Number of bytes to copy.  */
	if (copy_from_user (device_buffer + used_buffer_size, buffer, length)) {
		printk (KERN_ALERT "Failed to copy data from user space to kernel space\n");
		return -EFAULT;
	}
	used_buffer_size += length;
	return length;
}

static int device_release (struct inode *pinode, struct file *pfile) {
	printk (KERN_ALERT "Inside %s function\n", __FUNCTION__);
	mutex_unlock (&linear_buffer_mutex);
	module_put (THIS_MODULE);
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
	major_number = register_chrdev (0, DRIVER_NAME, &fops);
	if (major_number < 0) {
		printk (KERN_ALERT "Registering %s driver with major number %d failed\n", DRIVER_NAME, major_number);
		return major_number;
	}
	printk (KERN_ALERT "\"%s\" custom driver loaded in kernel\n", DRIVER_NAME);
	printk (KERN_ALERT "Registering %s driver with major number %d was successful\n", DRIVER_NAME, major_number);
	printk (KERN_ALERT "Run this command: sudo mknod -m 666 /dev/%s c %d 0\n", DRIVER_NAME, major_number);
	return SUCCESS;
}

static void __exit module_unload (void) {
	unregister_chrdev (major_number, DRIVER_NAME);
	printk (KERN_ALERT "\"%s\" custom driver unloaded from kernel\n", DRIVER_NAME);	
}

module_init (module_load);
module_exit (module_unload);

MODULE_LICENSE (DRIVER_LICENSE);
MODULE_AUTHOR (DRIVER_AUTHOR);
MODULE_DESCRIPTION (DRIVER_DESCRIPTION);
