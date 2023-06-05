#ifndef TEE_TZVFS_H
#define TEE_TZVFS_H

#include <tee/tzvfs.h>

int syscall_tzvfs_open(int *tzvfs_errno, const char *filename, int flags, mode_t mode);
int syscall_tzvfs_close(int *tzvfs_errno, int fd);
char *syscall_tzvfs_getcwd(int *tzvfs_errno, char *buf, size_t size);
int syscall_tzvfs_lstat(int *tzvfs_errno, const char* path, struct tzvfs_stat *buf);
int syscall_tzvfs_stat(int *tzvfs_errno, const char *path, struct tzvfs_stat *buf);
int syscall_tzvfs_fstat(int *tzvfs_errno, int fd, struct tzvfs_stat *buf);
int syscall_tzvfs_fcntl(int *tzvfs_errno, int fd, int cmd, struct tzvfs_flock *arg);
ssize_t syscall_tzvfs_read(int *tzvfs_errno, int fd, void *buf, size_t count,  off_t offset, int whence);
ssize_t tzvfs_write_page(int *tzvfs_errno, int fd, const void *buf, size_t count,  off_t offset, int whence);
ssize_t syscall_tzvfs_write(int *tzvfs_errno, int fd, const void *buf, size_t count,  off_t offset, int whence);
uid_t syscall_tzvfs_geteuid(int *tzvfs_errno);
int syscall_tzvfs_unlink(int *tzvfs_errno, const char *pathname);
int syscall_tzvfs_access(int *tzvfs_errno, const char *pathname, int mode);
void *syscall_tzvfs_mmap(int *tzvfs_errno, void *addr, size_t len, int prot, int flags, int fildes, off_t off);
void *syscall_tzvfs_mremap(int *tzvfs_errno, void *old_address, size_t old_size, size_t new_size, int flags);
int syscall_tzvfs_munmap(int *tzvfs_errno, void *addr, size_t length);
size_t syscall_tzvfs_strcspn(int *tzvfs_errno, const char *str1, const char *str2);
int syscall_tzvfs_utimes(int *tzvfs_errno, const char *filename, const struct tzvfs_timeval times[2]);
off_t syscall_tzvfs_lseek(int *tzvfs_errno, int fd, off_t offset, int whence);
int syscall_tzvfs_fsync(int *tzvfs_errno, int fd);
char* syscall_tzvfs_getenv(int *tzvfs_errno, const char *name);
pid_t syscall_tzvfs_getpid(int *tzvfs_errno);
time_t syscall_tzvfs_time(int *tzvfs_errno, time_t *t);
unsigned int syscall_tzvfs_sleep(int *tzvfs_errno, unsigned int seconds);
int syscall_tzvfs_gettimeofday(int *tzvfs_errno, struct tzvfs_timeval *tv, struct tzvfs_timezone *tz);
int syscall_tzvfs_fchown(int *tzvfs_errno, int fd, uid_t owner, gid_t group);

#endif