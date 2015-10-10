#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/gpio.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <linux/string.h>


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
#define BUTTON 17

MODULE_LICENSE("GPL");


static volatile uint32_t *gpio_reg;
// # timer
struct timer_list timer;
char timer_status = 0;
int timer_ms = 100;
bool state=true;

static void pinMode(int gpio, int mode){
        // ## mode -> 0 : input, 1:output
        int x = gpio / 10;
        int bit = (gpio % 10) * 3;

        unsigned mask = 0b111 << bit;

        gpio_reg[GPFSEL0 + x] = (gpio_reg[GPFSEL0 + x] & ~mask) | ((mode << bit) & mask);
}


static void digitalWrite(int gpio, bool output){
        if(output){
                gpio_reg[GPSET0 + gpio/32] |= (1 << (gpio % 32));
        }
        else {
                gpio_reg[GPCLR0 + gpio/32] |= (1 << (gpio % 32));
        }
}

static int digitalRead(int gpio){
        return (gpio_reg[GPLEV0 + gpio/32] >> (gpio % 32)) & 1;
}

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
	return 0;
}
int mod_write(struct file *filp, char *buf, size_t count, loff_t *f_pos){
	return 0;
}

int result;
struct file_operations mod_fops={
	.open=mod_open,
	.release=mod_release,
	.read=mod_read,
	.write=mod_write,
};

static void my_loop(unsigned long ptr){
        if(digitalRead(BUTTON)){
		digitalWrite(LED, true);
        }
        else {
		digitalWrite(LED, false);
        }
        timer.expires = jiffies + timer_ms;
        add_timer(&timer);
}

static int __init mod_init(void)
{
	printk("<1> Hello world from kernel!!\n");
	result = register_chrdev(0,"MOD",&mod_fops);	
	printk("<1> major number = %d\n",result);
	

        gpio_reg = (uint32_t*) ioremap(GPIO_BASE, GPIO_LEN);
	pinMode(LED, 1);
	pinMode(BUTTON, 0);
	// # timer
	timer_ms = timer_ms*HZ/1000; // 
	init_timer(&timer);
        timer.function = my_loop;
        timer.data = (unsigned long) &timer_status;
        timer.expires = jiffies + timer_ms;
        add_timer(&timer);
	printk("millisec = %d, HZ = %d\n",timer_ms, HZ);

	return 0;
}


static void __exit mod_exit(void){
	printk("<1> bye bye\n");
	// # timer
	del_timer(&timer);
	digitalWrite(LED,false);	
	unregister_chrdev(result,"MOD");
}

module_init(mod_init);
module_exit(mod_exit);
