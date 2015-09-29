#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/gpio.h>

#include <asm/io.h>
#include <asm/uaccess.h>



#define BCM2708_PERI_BASE       0x20000000
#define GPIO_BASE    (BCM2708_PERI_BASE + 0x200000)     // GPIO controller
#define GPIO_LEN        0xB0

#define GPFSEL0 0
#define GPFSEL1 1
#define GPFSEL2 2
#define GPFSEL3 3
#define GPFSEL4 4
#define GPFSEL5 5

#define GPSET0  7
#define GPSET1  8

#define GPCLR0  10
#define GPCLR1  11

#define GPLEV0  13
#define GPLEV1  14

#define GPPUD       (0x94 /4)
#define GPPUDCLK0   (0x98 /4)



#define LED 4


MODULE_LICENSE("GPL");


static volatile uint32_t *gpio_reg;

bool state=true;

static void pinMode(int gpio, int mode){
        // ## mode -> 0 : input, 1:output
        int GPRSELx = gpio / 10;
        int bit = (gpio % 10) * 3;

        unsigned mask = 0b111 << bit;

        gpio_reg[GPRSELx] = (gpio_reg[GPRSELx] & ~mask) | ((mode << bit) & mask);
}


static void digitalWrite(int gpio, bool output){
        if(output){
                gpio_reg[GPSET0 + gpio/32] |= (1 << (gpio % 32));
        }
        else {
                gpio_reg[GPCLR0 + gpio/32] |= (1 << (gpio % 32));
        }
}


int mod_open(struct inode *inode, struct file *filp);
int mod_release(struct inode *inode, struct file *filp);
int mod_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);

int mod_open(struct inode *inode, struct file *filp){
	printk("open\n");
	// H/W init
	pinMode(LED, 1);
	return 0;
}

int mod_release(struct inode *inode, struct file *filp){
	printk("close\n");
	return 0;
}

int mod_read(struct file *filp, char *buf, size_t count, loff_t *f_pos){
	return 0;
}
int mod_write(struct file *filp, char *buf, size_t count, loff_t *f_pos){
	printk("LED :%d\n",state);
	digitalWrite(LED, state);
	state = state?false:true;
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
	

        gpio_reg = (uint32_t*) ioremap(GPIO_BASE, GPIO_LEN);
	
	return 0;
}


static void __exit mod_exit(void){
	printk("<1> bye bye\n");
	digitalWrite(LED,false);	
	unregister_chrdev(result,"MOD");
}

module_init(mod_init);
module_exit(mod_exit);
