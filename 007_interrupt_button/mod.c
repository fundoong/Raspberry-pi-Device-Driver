#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h> 
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
#define BUTTON 17

MODULE_LICENSE("GPL");


static volatile uint32_t *gpio_reg;

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
static unsigned int irqNumber; 
struct file_operations mod_fops={
	.open=mod_open,
	.release=mod_release,
	.read=mod_read,
	.write=mod_write,
};
static irq_handler_t mod_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){
	printk("interrupt!\n");        
	digitalWrite(LED, state);
        state = state?false:true;

	return (irq_handler_t) IRQ_HANDLED;  
}

static int __init mod_init(void)
{
	printk("<1> Hello world from kernel!!\n");
	result = register_chrdev(0,"MOD",&mod_fops);	
	printk("<1> major number = %d\n",result);
	

        gpio_reg = (uint32_t*) ioremap(GPIO_BASE, GPIO_LEN);
	
	
	pinMode(LED, 1);
	pinMode(BUTTON, 0);
	irqNumber = gpio_to_irq(BUTTON);
	result = request_irq(irqNumber,(irq_handler_t) mod_irq_handler, IRQF_TRIGGER_RISING,
		"mod_gpio_handler",NULL);  

	
	return 0;
}


static void __exit mod_exit(void){
	printk("<1> bye bye\n");
	digitalWrite(LED,false);	
	free_irq(irqNumber, NULL); 
	unregister_chrdev(result,"MOD");
}

module_init(mod_init);
module_exit(mod_exit);
