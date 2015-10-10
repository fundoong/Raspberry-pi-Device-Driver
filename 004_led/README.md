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

1) w

 LED ON 

2) w

 LED OFF

3) q

 quit ./test application


#4. module remove

1) $    sudo rmmod mod

2) $    sudo rm -f /root/*

