#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>

#define LED 4

MODULE_LICENSE("GPL");


char data[32]="module";

int mod_open(struct inode *inode, struct file *filp);
int mod_release(struct inode *inode, struct file *filp);
int mod_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);

int mod_open(struct inode *inode, struct file *filp){
	printk("open\n");
	// H/W init
	return 0;
}

int mod_release(struct inode *inode, struct file *filp){
	printk("close\n");
	return 0;
}

int mod_read(struct file *filp, char *buf, size_t count, loff_t *f_pos){
	copy_to_user(buf, data, count);	
	printk("mod_read: %s \n",data);
	return 0;
}
int mod_write(struct file *filp, char *buf, size_t count, loff_t *f_pos){
	copy_from_user(data, buf, count);
	printk("mod_write: %s \n",data);
	return 0;
}

int result;
struct file_operations mod_fops={
	.open=mod_open,
	.release=mod_release,
	.read=mod_read,
	.write=mod_write,
};

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
