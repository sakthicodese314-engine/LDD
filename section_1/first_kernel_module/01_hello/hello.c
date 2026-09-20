#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sakthi");
MODULE_DESCRIPTION("Simple Linux loadable Kernel Module");
MODULE_VERSION("1.0");

static int __init my_init(void)
{
    printk("Hello: Hello, Kernel\n");
    pr_info("My module loaded\n");
    return 0; 
}

static void __exit my_exit(void)
{
    printk("Hello: GoodBye Kernel\n");
    pr_info("My module unloaded\n");
}


module_init(my_init);
module_exit(my_exit);
