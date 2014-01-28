#ifndef _RECORD_LOCK_H
#define _RECORD_LOCK_H

#include <fcntl.h>

#define read_lock(fd, offset, whence, len) \
	lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len) \
	lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
	lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len) \
	lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define un_lock(fd, offset, whence, len) \
	lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))
#define is_read_lockable(fd, offset, whence, len) \
	lock_test((fd), F_RDLCK, (offset), (whence), (len))
#define is_write_lockable(fd, offset, whence, len) \
	lock_test((fd), F_WRLCK, (offset), (whence), (len))

int 
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

pid_t
lock_test(int fd, int type, off_t offset, int whence, off_t len);
#endif
