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
static int write_pos = 0;
static int read_pos = 0;
static int data_count = 0;  // how much data is currently stored


static int __init module_load (void);
static void __exit module_unload (void);
static int device_open (struct inode *, struct file *);
static ssize_t device_read (struct file *, char *, size_t, loff_t *);
static ssize_t device_write (struct file *, const char *, size_t, loff_t *);
static int device_release (struct inode *, struct file *);

static int device_open (struct inode* pinode, struct file* pfile) {
	printk (KERN_ALERT "Inside %s function\n", __FUNCTION__);
	/* if mutex_trylock (&mutex) returns 0, this means some process is already using the mutex */
	if (mutex_trylock (&ring_buffer_mutex) == 0) {
		printk (KERN_ALERT "Driver \"%s\" is busy\n", DRIVER_NAME);
		/* EBUSY is a macro, It stands for Error: BUSY
		 * - is used because Linux kernel functions typically return negative error codes to indicate failure. */
		return -EBUSY;
	}
	try_module_get (THIS_MODULE);
	return SUCCESS;
}

static ssize_t device_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) {
    size_t i;

    printk(KERN_ALERT "Inside %s function\n", __FUNCTION__);

    for (i = 0; i < length; i++) {
        char ch;

        // Copy one byte from user at a time
        if (copy_from_user(&ch, buffer + i, 1)) {
            printk(KERN_ALERT "Failed to copy byte %zu from user space\n", i);
            return -EFAULT;
        }

        // Store in ring buffer
        device_buffer[write_pos] = ch;
        write_pos = (write_pos + 1) % RING_BUFFER_SIZE;

        // If buffer is full, move read_pos too (overwrite behavior)
        if (data_count == RING_BUFFER_SIZE) {
            read_pos = (read_pos + 1) % RING_BUFFER_SIZE;
        } else {
            data_count++;
        }
    }

    printk(KERN_ALERT "Wrote %zu bytes to device\n", length);
    return length;
}

static ssize_t device_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) {
    size_t i;

    printk(KERN_ALERT "Inside %s function\n", __FUNCTION__);

    if (data_count == 0) {
        printk(KERN_ALERT "No data available to read\n");
        return 0;  // nothing to read
    }

    for (i = 0; i < length && data_count > 0; i++) {
        if (copy_to_user(buffer + i, &device_buffer[read_pos], 1)) {
            printk(KERN_ALERT "Failed to copy byte %zu to user space\n", i);
            return -EFAULT;
        }

        read_pos = (read_pos + 1) % RING_BUFFER_SIZE;
        data_count--;
    }

    printk(KERN_ALERT "Read %zu bytes from device\n", i);
    return i;
}

static int device_release (struct inode* pinode, struct file* pfile) {
	printk (KERN_ALERT "Inside %s function\n", __FUNCTION__);
	mutex_unlock (&ring_buffer_mutex);
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
