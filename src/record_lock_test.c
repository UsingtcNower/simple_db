#include "record_lock.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void Perror(const char *str) {
	char buf[100];
	snprintf(buf, 100, "%s at line:%d, file:%s\n", str, __LINE__, __FILE__);
	perror(buf);
}

void
gen_tmp_file(const char *filename) {
	int	fd;
	char ch = 'a';
	/*create a file, and fill it with alph table*/
	if((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE)) == -1) {
		Perror("open error");
	}
	for(int i=0;i<26;i++) {
		ch = ch+i;
		if(write(fd, &ch, 1) != 1) {
			Perror("write error");
		}
	}
	if(close(fd) == -1) {
		Perror("close error");
	}
}

void
do_lock(const char *filename) {
	int fd;
	char ch = 'z';
	/* test read lock */
	if((fd = open(filename, O_RDONLY, NULL)) < 0) {
		Perror("open error");
	}
	if(read_lock(fd, 0, SEEK_SET, 1) == -1) {
		Perror("read_lock error");
	} else {
		printf("Process %d set read_lock successfully\n", getpid());
	}
	if(read(fd, &ch, 1) != 1) {
		Perror("read error");
	} else {
		printf("Process %d read %c from file\n", getpid(), ch);
	}
	if(close(fd) == -1) {
		Perror("close error");
	}
	/*test write lock*/
	ch = 'z';
	if((fd = open(filename, O_WRONLY, NULL)) < 0) {
		Perror("open error");
	}
	do {
		if(write_lock(fd,1,SEEK_SET,1) == -1) {
			if(errno != EAGAIN) {
				Perror("write_lock error");
			}
		} else {
			printf("Process %d set write_lock successfully\n", getpid());
			break;
		}
	}while(1);
	if(write(fd, &ch, 1) != 1) {
		Perror("write error");
	}
	sleep(2);
	if(un_lock(fd,1,SEEK_SET,1) == -1) {
		Perror("un_lock error");
	} else {
		printf("Process %d unlock successfully\n", getpid());
	}
	if(close(fd) == -1) {
		Perror("close error");
	}
}

void test() {
	pid_t	pid;
	if((pid = fork()) < 0) {
		Perror("fork error");
	} else if(pid == 0) {
		do_lock("lock.dat");
	} else {
		sleep(2);
		do_lock("lock.dat");
	}
}

int
main()
{
	gen_tmp_file("lock.dat");
	test();
}
