 
#1. module compile & run

1) $   sudo make

2) $   sudo insmod mod.ko


#2. running module

1) open new Terminal

2) $   cd /var/log

3) $   tail -f syslog

>Oct 10 14:50:58 raspberrypi kernel: [ 6745.645822] <1> Hello world from kernel!!

>Oct 10 14:50:58 raspberrypi kernel: [ 6745.645883] <1> major number = 245

>press button

>Oct 10 14:51:00 raspberrypi kernel: [ 6747.646439] interrupt!

>Oct 10 14:51:00 raspberrypi kernel: [ 6747.646749] interrupt!

>Oct 10 14:51:02 raspberrypi kernel: [ 6749.298257] interrupt!


4) press button -> LED Toggle

 



#4. module remove

1) $    sudo rmmod mod


