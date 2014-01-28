#include "record_lock.h"
#include <errno.h>

int 
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len) {
	struct flock lock = {
		.l_type	= type,
		.l_start	= offset,
		.l_whence	= whence,
		.l_len	= len,
	};
	return fcntl(fd, cmd, &lock);
}

pid_t
lock_test(int fd, int type, off_t offset, int whence, off_t len) {
	struct flock lock = {
		.l_type	= type,
		.l_start	= offset,
		.l_whence	= whence,
		.l_len	= len,
	};
	if( fcntl(fd, F_GETLK, &lock) < 0) {
		perror("fcntl error");
	}
	if(lock.l_type == F_UNLCK) {
		return 0; /* false, region isn't locked by another proc*/
	}
	return lock.l_pid;
}


