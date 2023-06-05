#include <tee_internal_api.h>
#include "./include/tzvfs_teeuser.h"

int tzvfs_errno;

static TEE_Time start, end;
extern int write_time, read_time, lseek_time, strcspn_time;

off_t offset_g = 0;
int whence_g = SEEK_CUR;

int tzvfs_open(const char *filename, int flags, mode_t mode){
  int ret = -1;
  
  // TEE_GetREETime(&start);
  ret = utee_tzvfs_open(&tzvfs_errno, filename, flags, mode);
  // TEE_GetREETime(&end);
  // DMSG("has been called, filename=%s, flags=%d, mode=%d, ret=%d, tzvfs_errno=%d", filename, flags, mode, ret, tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));

  // if (!flag) {
  //   chachapoly_init(&ctx, key, 256);
  // }

  return ret;
}

int tzvfs_close(int fd){
  int ret = -1;
  
  // TEE_GetREETime(&start);
  ret = utee_tzvfs_close(&tzvfs_errno, fd);
  // TEE_GetREETime(&end);
  // DMSG("has been called, fd=%d, ret=%d, tzvfs_errno=%d", fd, ret, tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  return ret;
}

char *tzvfs_getcwd(char *buf, size_t size){
  char* ret = NULL;
  
  // TEE_GetREETime(&start);
  ret = utee_tzvfs_getcwd(&tzvfs_errno, buf, size);
  // TEE_GetREETime(&end);
  // DMSG("has been called, buf=%x, size=%d, ret=%d, tzvfs_errno=%d", buf, size, ret, tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  return ret;
}

int tzvfs_lstat( const char* path, struct tzvfs_stat *buf ) {
  int ret = -1;

  // TEE_GetREETime(&start);
  ret = utee_tzvfs_lstat(&tzvfs_errno, path, buf);
  // TEE_GetREETime(&end);
  // DMSG("has been called, path=%s, buf=%x, ret=%d, sizeof(struct tzvfs_flock)=%d, tzvfs_errno=%d", path, buf, ret, sizeof(struct tzvfs_flock), tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  return ret;
}

int tzvfs_stat(const char *path, struct tzvfs_stat *buf){
  int ret = -1;

  // TEE_GetREETime(&start);
  ret = utee_tzvfs_stat(&tzvfs_errno, path, buf);
  // TEE_GetREETime(&end);
  // DMSG("has been called, path=%s, buf=%x, ret=%d, tzvfs_errno=%d", path, buf, ret, tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  return ret;
}

int tzvfs_fstat(int fd, struct tzvfs_stat *buf){
  int ret = -1;

  // TEE_GetREETime(&start);
  ret = utee_tzvfs_fstat(&tzvfs_errno, fd, buf);
  // TEE_GetREETime(&end);
  // DMSG("has been called, fd=%d, buf=%x, ret=%d, tzvfs_errno=%d", fd, buf, ret, tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  return ret;
}

int tzvfs_fcntl(int fd, int cmd, ... /* arg */ ){
  int ret = -1;
  
  // Read one argument
  va_list valist;
  va_start(valist, cmd);
  struct tzvfs_flock* arg = va_arg(valist, struct tzvfs_flock*);
  va_end(valist);
  
  // TEE_GetREETime(&start);
  ret = utee_tzvfs_fcntl(&tzvfs_errno, fd, cmd, arg);
  // TEE_GetREETime(&end);
  // DMSG("has been called, fd=%d, cmd=%d, arg=%x, ret=%d, tzvfs_errno=%d", fd, cmd, arg, ret, sizeof(struct tzvfs_flock), tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  return ret;
}

ssize_t tzvfs_read(int fd, void *buf, size_t count){
  ssize_t ret = -1;

  // TEE_GetREETime(&start);
  ret = utee_tzvfs_read(&tzvfs_errno, fd, buf, count,  offset_g, whence_g);
  
  // if (buf) {
  //   chachapoly_crypt(&ctx, nonce, ad, 12, buf, count, crypto_buf, hmac_buf, 16, 0);
  // }

  offset_g = 0;
  whence_g = SEEK_CUR;
  // TEE_GetREETime(&end);
  // DMSG("has been called, fd=%d, buf=%x, count=%d, ret=%d, tzvfs_errno=%d", fd, buf, count, ret, tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  // read_time += 1000*(end.seconds-start.seconds) + (end.millis-start.millis);
  return ret;
}

pid_t tzvfs_getpid(void){
  pid_t ret = -1;

  // TEE_GetREETime(&start);
  ret = utee_tzvfs_getpid(&tzvfs_errno);
  // TEE_GetREETime(&end);
  // DMSG("has been called, ret=%d, tzvfs_errno=%d", ret, tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  return ret;
}

uid_t tzvfs_geteuid(void){
  uid_t ret = -1;

  // TEE_GetREETime(&start);
  ret = utee_tzvfs_geteuid(&tzvfs_errno);
  // TEE_GetREETime(&end);
  // DMSG("has been called, ret=%d, tzvfs_errno=%d", ret, tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  return ret;
}

off_t tzvfs_lseek(int fd, off_t offset, int whence){
  off_t ret = offset;

  offset_g = offset;
  whence_g = whence;

  // TEE_GetREETime(&start);
  // ret = utee_tzvfs_lseek(&tzvfs_errno, fd, offset, whence);
  // TEE_GetREETime(&end);
  // DMSG("has been called, fd=%d, offset=%d, whence=%d, ret=%d, tzvfs_errno=%d", fd, offset, whence, ret, tzvfs_errno);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  // lseek_time += 1000*(end.seconds-start.seconds) + (end.millis-start.millis);
  return ret;
}

ssize_t tzvfs_write(int fd, const void *buf, size_t count){
  ssize_t ret = -1;

  // DMSG("has been called");
  // TEE_GetREETime(&start);
  ret = utee_tzvfs_write(&tzvfs_errno, fd, buf, count,  offset_g, whence_g);
  
  // if (buf) {
  //   chachapoly_crypt(&ctx, nonce, ad, 12, buf, count, crypto_buf, hmac_buf, 16, 1);
  // }

  offset_g = 0;
  whence_g = SEEK_CUR;
  // TEE_GetREETime(&end);
  // DMSG("time: %d ms", 1000*(end.seconds-start.seconds) + (end.millis-start.millis));
  // write_time += 1000*(end.seconds-start.seconds) + (end.millis-start.millis);
  
  return ret;
}

int tzvfs_unlink(const char *pathname){
  int ret = -1;
  
  DMSG("has been called");
  ret = utee_tzvfs_unlink(&tzvfs_errno, pathname);

  return ret;
}

int tzvfs_access(const char *pathname, int mode){
  int ret = -1;

  DMSG("has been called");
  ret = utee_tzvfs_access(&tzvfs_errno, pathname, mode);

  return ret;
}

void *tzvfs_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off){
  void* ret = (void *)-1;

  DMSG("has been called");
  ret =  utee_tzvfs_mmap(&tzvfs_errno, addr, len, prot, flags, fildes, off);
  
  return ret;
}

void *tzvfs_mremap(void *old_address, size_t old_size, size_t new_size, int flags, ... /* void *new_address */){
  void* ret = (void *)-1;

  DMSG("has been called");
  ret =  utee_tzvfs_mremap(&tzvfs_errno, old_address, old_size, new_size, flags);

  return ret;
}

int tzvfs_munmap(void *addr, size_t length){
  int ret = -1;

  DMSG("has been called");
  ret = utee_tzvfs_munmap(&tzvfs_errno, addr, length);

  return ret;
}

size_t tzvfs_strcspn(const char *str1, const char *str2){
  // size_t ret = 0;

  // // DMSG("has been called");
  // ret = utee_tzvfs_strcspn(&tzvfs_errno, str1, str2);
  
  // return ret;
  // TEE_GetREETime(&start);
  char *p1 = (char *)str1;
  char *p2;
  while (*p1 != '\0') {
      p2 = (char *)str2;
      while (*p2 != '\0') {
          if (*p2 == *p1) return p1-str1;
          ++p2;
      }
      ++p1;
  }
  // TEE_GetREETime(&end);
  // strcspn_time += 1000*(end.seconds-start.seconds) + (end.millis-start.millis);
  return  p1 - str1;
}

int tzvfs_utimes(const char *filename, const struct tzvfs_timeval times[2]){
  int ret = -1;

  DMSG("has been called");
  ret = utee_tzvfs_utimes(&tzvfs_errno, filename, times);
  
  return ret;
}

int tzvfs_fsync(int fd){
  int ret = -1;

  DMSG("has been called");
  ret = utee_tzvfs_fsync(&tzvfs_errno, fd);

  return ret;
}

char* tzvfs_getenv(const char *name){
  char* ret = NULL;

  DMSG("has been called");
  ret = utee_tzvfs_getenv(&tzvfs_errno, name);
  
  return ret;
}

time_t tzvfs_time(time_t *t){
  time_t ret = -1;

  DMSG("has been called");
  ret =  utee_tzvfs_time(&tzvfs_errno, t);

  return ret;
}

unsigned int tzvfs_sleep(unsigned int seconds){
  unsigned int ret;

  DMSG("has been called");
  ret = utee_tzvfs_sleep(&tzvfs_errno, seconds);

  return ret;
}

int tzvfs_gettimeofday(struct tzvfs_timeval *tv, struct tzvfs_timezone *tz){
  int ret = -1;

  DMSG("has been called");
  ret = utee_tzvfs_gettimeofday(&tzvfs_errno, tv, tz);

  return ret;
}

int tzvfs_fchown(int fd, uid_t owner, gid_t group){
  int ret = -1;

  DMSG("has been called");
  ret = utee_tzvfs_fchown(&tzvfs_errno, fd, owner, group);
  
  return ret;
}

int tzvfs_ftruncate(int fd, off_t length){
  DMSG("%s: haven't been realized!\n", __func__);
  return 0;
}

int tzvfs_fchmod(int fd, mode_t mode){
  DMSG("%s: haven't been realized!\n", __func__);
  return 0;
}

void *tzvfs_dlopen(const char *filename, int flag){
  DMSG("%s: haven't been realized!\n", __func__);
  return NULL;
}

char *tzvfs_dlerror(void){
  DMSG("%s: haven't been realized!\n", __func__);
  return NULL;
}

void *tzvfs_dlsym(void *handle, const char *symbol){
  DMSG("%s: haven't been realized!\n", __func__);
  return NULL;
}

int tzvfs_dlclose(void *handle){
  DMSG("%s: haven't been realized!\n", __func__);
  return 0;
}

int tzvfs_mkdir(const char *pathname, mode_t mode) {
  DMSG("%s: haven't been realized!\n", __func__);
  return 0;
}

int tzvfs_rmdir(const char *pathname){
  DMSG("%s: haven't been realized!\n", __func__);
  return 0;
}

ssize_t tzvfs_readlink(const char *path, char *buf, size_t bufsiz){
  DMSG("%s: haven't been realized!\n", __func__);
  return 0;
}

long int tzvfs_sysconf(int name){
  long int ret = -1;
  DMSG("%s: haven't been realized!\n", __func__);
  return ret;
}

struct tzvfs_tm *tzvfs_localtime(const time_t *timep){
  struct tzvfs_tm * ret = NULL;
  DMSG("%s: haven't been realized!\n", __func__);
  return ret;
}
