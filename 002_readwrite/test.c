#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


int main(){
	int fd;
	char buff[32] = "user";
	fd = open("/dev/MOD",O_RDWR);
	if(fd<0){
		printf("error!\n");
		return -1;
	}
	while(1){
		char g = getchar();
		if(g == 'r'){	
			read(fd,buff,32);
			printf("read : %s\n",buff);
		}
		else if(g == 'w'){
			write(fd, buff,32);
			printf("write : %s\n",buff);
		}
		else if(g == 'q'){
			break;
		}
	
		
	}
	close(fd);

	return 0;
}
