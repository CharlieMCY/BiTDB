#ifndef TZVFS_TEEUSER_H_
#define TZVFS_TEEUSER_H_

#include <utee_syscalls.h>

// tzvfs start
long int tzvfs_sysconf(int name);
int tzvfs_open(const char *filename, int flags, mode_t mode);
int tzvfs_gettimeofday(struct tzvfs_timeval *tv, struct tzvfs_timezone *tz);
unsigned int tzvfs_sleep(unsigned int seconds);
void *tzvfs_dlopen(const char *filename, int flag);
char *tzvfs_dlerror(void);
void *tzvfs_dlsym(void *handle, const char *symbol);
int tzvfs_dlclose(void *handle);
time_t tzvfs_time(time_t *t);
int tzvfs_utimes(const char *filename, const struct tzvfs_timeval times[2]);
struct tzvfs_tm *tzvfs_localtime(const time_t *timep);
pid_t tzvfs_getpid(void);
int tzvfs_fsync(int fd);
int tzvfs_close(int fd);
int tzvfs_access(const char *pathname, int mode);
char *tzvfs_getcwd(char *buf, size_t size);
int tzvfs_lstat( const char* path, struct tzvfs_stat *buf );
int tzvfs_stat(const char *path, struct tzvfs_stat *buf);
int tzvfs_fstat(int fd, struct tzvfs_stat *buf);
int tzvfs_ftruncate(int fd, off_t length);
int tzvfs_fcntl(int fd, int cmd, ... /* arg */ );
ssize_t tzvfs_read(int fd, void *buf, size_t count);
ssize_t tzvfs_write(int fd, const void *buf, size_t count);
int tzvfs_fchmod(int fd, mode_t mode);
int tzvfs_unlink(const char *pathname);
int tzvfs_mkdir(const char *pathname, mode_t mode);
int tzvfs_rmdir(const char *pathname);
int tzvfs_fchown(int fd, uid_t owner, gid_t group);
uid_t tzvfs_geteuid(void);
char* tzvfs_getenv(const char *name);
void *tzvfs_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
int tzvfs_munmap(void *addr, size_t length);
void *tzvfs_mremap(void *old_address, size_t old_size, size_t new_size, int flags, ... /* void *new_address */);
ssize_t tzvfs_readlink(const char *path, char *buf, size_t bufsiz);
size_t tzvfs_strcspn(const char *str1, const char *str2);
off_t tzvfs_lseek(int fd, off_t offset, int whence);
// tzvfs end

#endif
