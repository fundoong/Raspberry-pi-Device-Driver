#Button 
# H/W
 [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
 5v  5v  GND  14  15  18 GND  23  24 GND  25  8   7   
 3v3  2   3    4 GND  17  27  22 3v3  10  9   11 GND  
 [ ] [ ] [ ] [*] [*] [*] [ ] [ ] [*] [ ] [ ] [ ] [ ]
          ┌───┘   │   │           │
          └LED─R1─┤   ├─R2────────┘
                  ┌───┐
                  Button
> R1 : 330 ohm
> R2 : 1  kohm
#1. module compile & run

1) $   sudo make

2) $   sudo insmod mod.ko

3) $   sudo dmesg

>[  327.459535] <1> Hello world from kernel!!

>[  327.459594] <1> major number = 246    

> - major number is not static, you have to check major number

4) $   sudo mknod /dev/MOD c 246 0
 

#2. test code compile & run

1) $   gcc -o test test.c

2) $   sudo ./test



#3. running ./test

1) w100

 LED blink 100 millisecond(faster)

2) w 2000

 LED blink 2 second (slower)

3) q

 quit ./test application


#4. module remove

1) $    sudo rmmod mod

2) $    sudo rm -f /root/*

