#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>



MODULE_LICENSE("GPL");

int result;
struct file_operations mod_fops;

static int __init mod_init(void)
{
	printk("<1> Hello world from kernel!!\n");
	result = register_chrdev(0,"MOD",&mod_fops);	
	printk("<1> major number = %d\n",result);
	
	return 0;
}


static void __exit mod_exit(void){
	printk("<1> bye bye\n");
	unregister_chrdev(result,"MOD");
}

module_init(mod_init);
module_exit(mod_exit);
