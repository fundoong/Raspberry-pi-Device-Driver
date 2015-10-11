#1. module compile & run

1) $   sudo make

2) $   sudo insmod mod.ko

3) $   sudo dmesg

>[  327.459535] <1> Hello world from kernel!!

>[  327.459594] <1> major number = 246    

>major number is not static, you have to check major number

4) $   sudo mknod /dev/MOD c 246 0
 

#2. test code compile & run

1) $   gcc -o test test.c

2) $   sudo ./test



#3. running ./test



- open new terminal

2) $ cd /var/log

3) $ tail -f syslog

>Oct 11 05:30:53 raspberrypi kernel: [ 2375.291436] open

>Oct 11 05:30:54 raspberrypi kernel: [ 2377.084692] [-6,-16,-40] [-46,92,3889]

>Oct 11 05:30:56 raspberrypi kernel: [ 2378.484712] [-2,-15,-40] [-30,88,3851]

>Oct 11 05:30:57 raspberrypi kernel: [ 2379.414713] [-1,-15,-39] [-30,89,3808]

>Oct 11 05:30:58 raspberrypi kernel: [ 2380.344722] [-7,-16,-40] [-43,87,3917]

>Oct 11 05:30:59 raspberrypi kernel: [ 2381.414731] [-5,-14,-40] [-38,90,3864]
 
- previous terminal 

1) $ sudo ./test

2) "r"

print gyro, accel on kernel syslog

3) "q"

 quit ./test application


#4. module remove

1) $    sudo rmmod mod

2) $    sudo rm -f /root/*

