#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#define DEVICE_NAME "my_dma_device"
static int major_number;
static void *dma_buffer;
static dma_addr_t dma_handle;
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple DMA coherent memory example");

static int my_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}
static int my_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static struct file_operations fops = {
    .open = my_open,
    .release = my_release,
};

static int __init my_init(void) {
    // 注册字符设备
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register device: %d\n", major_number);
        return major_number;
    }
    // 分配DMA一致性内存
    dma_buffer = dma_alloc_coherent(NULL, PAGE_SIZE, &dma_handle, GFP_KERNEL);
    if (!dma_buffer) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to allocate DMA buffer\n");
        return -ENOMEM;
    }
    printk(KERN_INFO "DMA buffer allocated at %p with handle %pad\n", dma_buffer, &dma_handle);
    
    return 0; 
}
static void __exit my_exit(void) {
    // 释放DMA一致性内存
    if (dma_buffer)
        dma_free_coherent(NULL, PAGE_SIZE, dma_buffer, dma_handle);
    // 注销字符设备
    unregister_chrdev(major_number, DEVICE_NAME);
    
    printk(KERN_INFO "Device unregistered and DMA buffer freed\n");
}

module_init(my_init);
module_exit(my_exit);