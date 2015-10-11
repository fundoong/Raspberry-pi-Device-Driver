
 
#define BCM2708_PERI_BASE       0x20000000
#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000)	// GPIO controller 
#define GPIO_LEN	0xB0
#define BSC_BASE     		(BCM2708_PERI_BASE + 0x804000)  // I2C controller 
#define BSC_LEN	0x22
 /////////////////////////////////////////////////
// GPIO

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

////////////////////////////////////////
// I2C Address Map

#define BSC0_C 0
#define BSC0_S 1
#define BSC0_DLEN 2
#define BSC0_A 3
#define BSC0_FIFO 4
#define BSC0_DIV 5
#define BSC0_DEL 6



#define BSC_C_I2CEN     (1 << 15)
#define BSC_C_INTR      (1 << 10)
#define BSC_C_INTT      (1 << 9)
#define BSC_C_INTD      (1 << 8)
#define BSC_C_ST        (1 << 7)
#define BSC_C_CLEAR     (1 << 4)
#define BSC_C_READ      1
 
#define START_READ      BSC_C_I2CEN|BSC_C_ST|BSC_C_CLEAR|BSC_C_READ
#define START_WRITE     BSC_C_I2CEN|BSC_C_ST
 
#define BSC_S_CLKT  (1 << 9)
#define BSC_S_ERR     (1 << 8)
#define BSC_S_RXF     (1 << 7)
#define BSC_S_TXE     (1 << 6)
#define BSC_S_RXD     (1 << 5)
#define BSC_S_TXD     (1 << 4)
#define BSC_S_RXR     (1 << 3)
#define BSC_S_TXW     (1 << 2)
#define BSC_S_DONE    (1 << 1)
#define BSC_S_TA      1
 
#define CLEAR_STATUS    BSC_S_CLKT|BSC_S_ERR|BSC_S_DONE


 
