#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/gpio.h>


#define LED 4

MODULE_LICENSE("GPL");

int result;
struct file_operations mod_fops;

static int __init mod_init(void)
{
	printk("<1> Hello world from kernel!!\n");
	result = register_chrdev(0,"MOD",&mod_fops);	
	printk("<1> major number = %d\n",result);
	
	if(gpio_request(LED,"sysf")){
		printk("error : LED PIN request faiure\n");
		return 0;
	}
	gpio_direction_output(LED,1);
	return 0;
}


static void __exit mod_exit(void){
	printk("<1> bye bye\n");
	
	gpio_direction_output(LED,0);
	gpio_free(LED);
	unregister_chrdev(result,"MOD");
}

module_init(mod_init);
module_exit(mod_exit);
