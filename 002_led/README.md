
Tutorial 002 LED : LED on, off example

Hardware
Raspberry pi       
  pin 4       ---(Register 330Ω) --- ( + LED - ) ---┐
                                                    │
  GND         --------------------------------------┘
 

How to use
1. $ sudo make
2. $ sudo insmod mod.ko
3. $ sudo dmesg
   LED on
4. $ sudo rmmod mod
   LED off
