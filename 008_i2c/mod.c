#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <asm/io.h>
#include "regmap.h"

void I2C_init(void);
int  I2C_done(void);
void I2C_read(int address,int start, uint8_t *buffer, int size);
void I2C_write_reg(int address,int reg, uint8_t data);

void MPU6050_Reg_setup(void);
void MPU6050_Read_Sensor(void);
short twoByte(uint8_t a, uint8_t b);


static volatile uint32_t *gpio_reg;
static volatile uint32_t *i2c_reg;

//---mpu6050-----
int16_t accel[3]={0,0,0};
int16_t gyro[3]={0,0,0};
int16_t gyro_p[3]={0,0,0};
////////////////////////////////////////////////////////////////////////////////
///             ####   ###   ####    ###
///     GPIO   #       #  #   ##    #   #
///            #  ###  ###    ##    #   #
///             ### #  #     ####    ###
///////////////////////////////////////////////////////////////////////////////
static void pinMode(int gpio, int mode){
        // ## mode -> 0 : input, 1:output, 4:Alt0
        int GPRSELx = gpio / 10;
        int bit = (gpio % 10) * 3;
        unsigned mask = 0b111 << bit;
        gpio_reg[GPRSELx] = (gpio_reg[GPRSELx] & ~mask) | ((mode << bit) & mask);
}


static void digitalWrite(int gpio, bool output){
        if(output)
                gpio_reg[GPSET0 + gpio/32] |= (1 << (gpio % 32));
        else
                gpio_reg[GPCLR0 + gpio/32] |= (1 << (gpio % 32));
}
static int digitalRead(int gpio){
        return (gpio_reg[GPLEV0 + gpio/32] >> (gpio % 32)) & 1;
}

////////////////////////////////////////////////////////////////////////////////
//	%%%%   %%    %%%
//       %%   %  %  %   
//       %%     %   %
//      %%%%  %%%%   %%%
////////////////////////////////////////////////////////////////////////////////
void I2C_init(void){
	pinMode(0, 4);
	pinMode(1, 4);
}  
// Function to wait for the I2C transaction to complete
int I2C_done(void) { 
        int timeout = 50;
        while((!(i2c_reg[BSC0_S] & BSC_S_DONE)) && --timeout) {
            msleep(1);
        }
        if(timeout == 0){
		return -1;
	}
	return 0;
}
void I2C_read(int address,int start, uint8_t *buffer, int size){
        int i = 0;
        i2c_reg[BSC0_A] = address;
        i2c_reg[BSC0_DLEN] = 1;        // one byte
        i2c_reg[BSC0_FIFO] = start;     // value of first register
        i2c_reg[BSC0_S] = CLEAR_STATUS;  // Reset status bits (see #define)
        i2c_reg[BSC0_C] = START_WRITE;     // Start Write (see #define)

        I2C_done();
        i2c_reg[BSC0_DLEN] = size;
        i2c_reg[BSC0_S] = CLEAR_STATUS;  // Reset status bits (see #define)
        i2c_reg[BSC0_C] = START_READ;      // Start Read after clearing FIFO (see #define)

        I2C_done();

        for(i=0; i<size; i++){
		buffer[i] = i2c_reg[BSC0_FIFO];	
	}
}
void I2C_write_reg(int address,int reg, uint8_t data){
    i2c_reg[BSC0_A] = address;
 
    i2c_reg[BSC0_DLEN] = 2;
    i2c_reg[BSC0_FIFO] = (unsigned char)reg;
    i2c_reg[BSC0_FIFO] = (unsigned char)data;
 
    i2c_reg[BSC0_S] = CLEAR_STATUS;  // Reset status bits (see #define)
    i2c_reg[BSC0_C] = START_WRITE;     // Start Write (see #define)
 
    I2C_done();
}
//////////////////////////////////////////////////////////////////////////////////////
//  @@   @@  @@@  @  @  @   @@@ @@@ @@@  
//  @ @ @ @  @  @ @  @  @@@ @ @ @   @ @
//  @  @  @  @@@  @  @  @ @ @ @ @@@ @ @
//  @     @  @     @@   @@@ @@@ @@@ @@@
///////////////////////////////////////////////////////////////////////////////////////
#define MPU6050_I2C_ADDRESS 0x68
#define MPU6050_DATA_REGISTER  0x3B
void MPU6050_Reg_setup(void){
	I2C_write_reg(MPU6050_I2C_ADDRESS, 0x6B, 0x80);   //MPU6050_RA_PWR_MGMT_1 0x6B
  	msleep(5);
  	I2C_write_reg(MPU6050_I2C_ADDRESS, 0x6B, 0x03);   //MPU6050_RA_PWR_MGMT_1 0x6B
  	I2C_write_reg(MPU6050_I2C_ADDRESS, 0x1A, 0x04);   //MPU6050_RA_CONFIG 0x1A     =DLPF 42hz
  	I2C_write_reg(MPU6050_I2C_ADDRESS, 0x1B, 0x18);   //MPU6050_RA_GYRO_CONFIG 0x1B
  	I2C_write_reg(MPU6050_I2C_ADDRESS, 0x1C, 0x10);	//MPU6050_RA_ACCEL_CONFIG 0x1C
}
void MPU6050_Read_Sensor(void){
	uint8_t Gyro_accel_raw[14];  
	I2C_read(MPU6050_I2C_ADDRESS , MPU6050_DATA_REGISTER , (uint8_t *) &Gyro_accel_raw, 14);
	
	accel[0]= twoByte(Gyro_accel_raw[ 0], Gyro_accel_raw[ 1]);
   	accel[1]= twoByte(Gyro_accel_raw[ 2], Gyro_accel_raw[ 3]);
   	accel[2]= twoByte(Gyro_accel_raw[ 4], Gyro_accel_raw[ 5]);
   	gyro[0]=  twoByte(Gyro_accel_raw[ 8], Gyro_accel_raw[ 9]);
   	gyro[1]=  twoByte(Gyro_accel_raw[10], Gyro_accel_raw[11]);
   	gyro[2]=  twoByte(Gyro_accel_raw[12], Gyro_accel_raw[13]);

}
short twoByte(uint8_t a, uint8_t b){
	short temp = a<<8;
	temp+=b;
	return temp;
}


//////////////////////////////////////////////////////////////////
///
///	Real Module!!
///
/////////////////////////////////////////////////////////////////

MODULE_LICENSE("GPL");

static int mod_open (struct inode* inode, struct file *filp){
	printk("open\n");
	return 0;
}

static int mod_release (struct inode* inode, struct file *filp){
	printk("close\n");
	return 0;
}
int mod_read(struct file *filp, char *buf, size_t count, loff_t *f_pos){
	MPU6050_Read_Sensor();
	printk("[%d,%d,%d] [%d,%d,%d]\n",gyro[0],gyro[1],gyro[2],accel[0],accel[1],accel[2]);
	
	return 0;
}
int mod_write(struct file *filp,const char *user_buf,size_t count,loff_t *f_pos){
	
	return 0;
}

int result;
struct file_operations mod_fops={
	open : mod_open,
	release : mod_release,
	write :mod_write,
	read : mod_read
};

static int __init mod_init(void)
{
	printk("hello MOD\n");
	result = register_chrdev(0,"MOD",&mod_fops);	
	printk(" $ sudo mknod /dev/MOD c %d 0\n",result);
	// Get register address

        gpio_reg = (uint32_t*) ioremap(GPIO_BASE, GPIO_LEN);	
	i2c_reg  = (uint32_t*) ioremap(BSC_BASE, BSC_LEN);
	I2C_init();
	
	MPU6050_Reg_setup();

	return 0;

}


static void __exit mod_exit(void){
	unregister_chrdev(result,"MOD");
}

module_init(mod_init);
module_exit(mod_exit);

