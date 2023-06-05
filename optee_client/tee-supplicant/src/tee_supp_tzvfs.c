#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>  // utimes() and gettimeofday()
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>  // time() and localtime()
#include <teec_trace.h>
#include <optee_msg_supplicant.h>
#include <tee_supplicant.h>
#include <tee_supp_tzvfs.h>

#ifndef __aligned
#define __aligned(x) __attribute__((__aligned__(x)))
#endif
#include <linux/tee.h>

#define SH_PAGE_SZIE 80 * 1024 * 1024

typedef struct shmemcache {
//   int lseek;
  int page[1024];
//   uint8_t isWrite;
}shmemcache;

static TEEC_Result ree_tzvfs_open(size_t num_params,
				   struct tee_ioctl_param *params)
{
	int ret = -1;
    char *filename = NULL;
	int flags = 0;
	mode_t mode = 0;

	if (num_params != 3 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	filename = tee_supp_param_to_va(params + 1);
	if (!filename) return TEEC_ERROR_BAD_PARAMETERS;
	flags = params[0].u.value.b;
	mode = params[0].u.value.c;
	
	ret = open(filename, flags, mode);
	params[2].u.value.a = ret;
	if (ret == -1) params[2].u.value.b = errno;
	printf("DMSG: call%s, filename=%s, flags=%d, mode=%d, ret=%d\n", __func__, filename, flags, mode, ret);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_close(size_t num_params,
				   struct tee_ioctl_param *params)
{
	int ret = -1;
	int fd = -1;

	if (num_params != 2 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	fd = params[0].u.value.b;

	ret = close(fd);
	params[1].u.value.a = ret;
	if (ret == -1) params[1].u.value.b = errno;
	printf("DMSG: call%s, fd=%d, ret=%d\n", __func__, fd, ret);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_getcwd(size_t num_params,
				   struct tee_ioctl_param *params)
{
	char *ret = NULL;
    char *buf = NULL;
	size_t size = 0;

	if (num_params != 3 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_OUTPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	buf = tee_supp_param_to_va(params + 1);
	if (!buf) return TEEC_ERROR_BAD_PARAMETERS;
	// size = MEMREF_SIZE(params + 1);
	size = params[1].u.memref.size;
	
	ret = getcwd(buf, size);
	params[2].u.value.a = ret;
	if (ret == NULL) params[2].u.value.b = errno;
	printf("DMSG: call%s, buf=%x, size=%d, ret=%x\n", __func__, buf, size, ret);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_lstat(size_t num_params,
				   struct tee_ioctl_param *params)
{
    int ret = -1;
    char *path = NULL;
    struct stat *buf = NULL;

	if (num_params != 4 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_OUTPUT ||
        (params[3].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

    path = tee_supp_param_to_va(params + 1);
    if (!path) return TEEC_ERROR_BAD_PARAMETERS;
    buf = tee_supp_param_to_va(params + 2);
    if (!buf) return TEEC_ERROR_BAD_PARAMETERS;

    ret = lstat(path, buf);
    params[3].u.value.a = ret;
    if (ret == -1) params[3].u.value.b = errno;
	printf("DMSG: call%s, path=%s, buf=%x, ret=%d, sizeof(struct flock)=%d, st_uid=%d\n", __func__, path, buf, ret, sizeof(struct flock), buf->st_uid);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
    return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_stat(size_t num_params,
				   struct tee_ioctl_param *params)
{
    int ret = -1;
    char *path = NULL;
    struct stat *buf = NULL;

	if (num_params != 4 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_OUTPUT ||
        (params[3].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

    path = tee_supp_param_to_va(params + 1);
    if (!path) return TEEC_ERROR_BAD_PARAMETERS;
    buf = tee_supp_param_to_va(params + 2);
    if (!buf) return TEEC_ERROR_BAD_PARAMETERS;

    ret = stat(path, buf);
    params[3].u.value.a = ret;
    if (ret == -1) params[3].u.value.b = errno;
	printf("DMSG: call%s, path=%s, buf=%x, ret=%d, sizeof(struct stat)=%d, st_uid=%d\n", __func__, path, buf, ret, sizeof(struct stat), buf->st_uid);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
    return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_fstat(size_t num_params,
				   struct tee_ioctl_param *params)
{
	int ret = -1;
	int fd = -1;
	struct stat *buf = NULL;

	if (num_params != 3 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_OUTPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	buf = tee_supp_param_to_va(params + 1);
    if (!buf) return TEEC_ERROR_BAD_PARAMETERS;
	fd = params[0].u.value.b;

	ret = fstat(fd, buf);
	params[2].u.value.a = ret;
	if (ret == -1) params[2].u.value.b = errno;
	printf("DMSG: call%s, fd=%d, buf=%x, ret=%d\n", __func__, fd, buf, ret);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_fcntl(size_t num_params,
				   struct tee_ioctl_param *params)
{
	int ret = -1;
	int fd = -1;
	int cmd = 0;
	struct flock *buf = NULL;

	if (num_params != 3 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	buf = tee_supp_param_to_va(params + 1);
    if (!buf) return TEEC_ERROR_BAD_PARAMETERS;
	fd = params[0].u.value.b;
	cmd = params[0].u.value.c;

	ret = fcntl(fd, cmd, buf);
	params[2].u.value.a = ret;
	if (ret == -1) params[2].u.value.b = errno;
	printf("DMSG: call%s, fd=%d, cmd=%d, buf=%x, ret=%d, sizeof(struct flock)=%d\n", __func__, fd, cmd, buf, ret, sizeof(struct flock));
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_read(size_t num_params,
				   struct tee_ioctl_param *params)
{
	ssize_t ret = -1;
	int fd = -1;
	void *buf = NULL;
	shmemcache *page_cache = NULL;
	size_t count = 0;
	off_t offset = 0;
	int whence = SEEK_SET;

	if (num_params != 4 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
		(params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ||
	    (params[3].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	buf = tee_supp_param_to_va(params + 2);
    if (!buf) return TEEC_ERROR_BAD_PARAMETERS;
	// count = MEMREF_SIZE(params + 1);
	count = params[2].u.memref.size;
	fd = params[0].u.value.b;
	offset = params[1].u.value.a;
	whence = params[1].u.value.b;

	if (count == SH_PAGE_SZIE) {
		count = 4096;
		page_cache = (shmemcache *)buf;
		int page_num = offset / 4096;
		buf = page_cache[page_num].page;
	}

	ret = lseek(fd, offset, whence);
	if (ret == -1) {
		params[3].u.value.a = ret;
		params[3].u.value.b = errno;
		return TEEC_SUCCESS;
	}

	ret = read(fd, buf, count);
	params[3].u.value.a = ret;
	if (ret == -1) params[3].u.value.b = errno;
	// printf("DMSG: call%s, fd=%d, buf=%x, count=%d, ret=%d, errno=%s\n", __func__, fd, buf, count, ret, strerror(errno));	
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_write(size_t num_params,
				   struct tee_ioctl_param *params)
{
    ssize_t ret = -1;
	int fd = -1;
	void *buf = NULL;
	size_t count = 0;
	off_t offset = 0;
	int whence = SEEK_SET;

	// printf("ENTERING function %s.\n", __func__);
	// printf("Number of parameters: %d.\n", num_params);

	if (num_params != 4 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
		(params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ||
	    (params[3].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT) {
		printf("TEEC ERROR: bad parameters.\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	buf = tee_supp_param_to_va(params + 2);
    if (!buf) return TEEC_ERROR_BAD_PARAMETERS;
	// count = MEMREF_SIZE(params + 1);
	count = params[2].u.memref.size;
	fd = params[0].u.value.b;
	offset = params[1].u.value.a;
	whence = params[1].u.value.b;

	ret = lseek(fd, offset, whence);
	if (ret == -1) {
		params[3].u.value.a = ret;
		params[3].u.value.b = errno;
		return TEEC_SUCCESS;
	}

	ret = write(fd, buf, count);
	params[3].u.value.a = ret;
	if (ret == -1) 
		params[3].u.value.b = errno;
	// printf("DMSG: call%s, fd=%d, buf=%x, count=%d, ret=%d, errno=%s\n", __func__, fd, buf, count, ret, strerror(errno));	
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_geteuid(size_t num_params,
				   struct tee_ioctl_param *params)
{
    printf("%s has not been realised!\n", __func__);
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_unlink(size_t num_params,
				   struct tee_ioctl_param *params)
{
    int ret = -1;
    char *path = NULL;

	if (num_params != 3 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	path = tee_supp_param_to_va(params + 1);
	
	if (!path) return TEEC_ERROR_BAD_PARAMETERS;
	
	ret = unlink(path);
	params[2].u.value.a = ret;
	if (ret == -1) params[2].u.value.b = errno;
	printf("DMSG: call%s, path=%s, ret=%d\n", __func__, path, ret);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_access(size_t num_params,
				   struct tee_ioctl_param *params)
{
    int ret = -1;
    char *path = NULL;
	int mode = 0;

	if (num_params != 3 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	path = tee_supp_param_to_va(params + 1);
	
	if (!path) return TEEC_ERROR_BAD_PARAMETERS;
	mode = params[0].u.value.b;
	
	ret = access(path, mode);
	params[2].u.value.a = ret;
	if (ret == -1) params[2].u.value.b = errno;
	printf("DMSG: call%s, path=%s, mode=%d, ret=%d\n", __func__, path, mode, ret);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_mmap(size_t num_params,
				   struct tee_ioctl_param *params)
{
    int ret = -1;
	void *addr;
	size_t len;
	int prot;
	int flags;
	int fildes;
	off_t off;

	if (num_params != 4 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
		(params[3].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT){
		printf("TEEC ERROR: bad parameters.\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	addr = params[1].u.value.a;
	len = params[1].u.value.b;
	prot = params[1].u.value.c;
	flags = params[2].u.value.a;
	fildes = params[2].u.value.b;
	off = params[2].u.value.c;
	
	ret = mmap(addr, len, prot, flags, fildes, off);
	params[3].u.value.a = ret;
	if (ret == -1) params[3].u.value.b = errno;
	printf("DMSG: call%s, ret=%d\n", __func__, ret);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_mremap(size_t num_params,
				   struct tee_ioctl_param *params)
{
	int ret = -1;
	void *old_address;
	size_t old_size;
	size_t new_size;
	int flags;

    if (num_params != 3 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;
	
	old_address = params[0].u.value.b;
	old_size = params[0].u.value.c;
	new_size = params[1].u.value.a;
	flags = params[1].u.value.b;

	ret = mremap(old_address, old_size, new_size, flags);
	
	params[2].u.value.a = ret;
	if (ret == -1) params[2].u.value.b = errno;
	printf("DMSG: call%s, ret=%d\n", __func__, ret);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_munmap(size_t num_params,
				   struct tee_ioctl_param *params)
{
    int ret = -1;
	void *addr;
	size_t length;

    if (num_params != 2 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT)
		return TEEC_ERROR_BAD_PARAMETERS;
	
	addr = params[0].u.value.b;
	length = params[0].u.value.c;

	ret = munmap(addr, length);
	
	params[1].u.value.a = ret;
	if (ret == -1) params[1].u.value.b = errno;
	printf("DMSG: call%s, ret=%d\n", __func__, ret);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_strcspn(size_t num_params,
				   struct tee_ioctl_param *params)
{
   int ret = -1;
   char *str1 = NULL;
   char *str2 = NULL;

	if (num_params != 4 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ||
		(params[3].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	str1 = tee_supp_param_to_va(params + 1);
	str2 = tee_supp_param_to_va(params + 2);
	
	if (!str1) return TEEC_ERROR_BAD_PARAMETERS;
	if (!str2) return TEEC_ERROR_BAD_PARAMETERS;
	
	ret = strcspn(str1, str2);
	params[3].u.value.a = ret;
	if (ret == -1) params[3].u.value.b = errno;
	// printf("DMSG: call%s, ret=%d, errno=%d, errstr=%s\n", __func__, ret, errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_utimes(size_t num_params,
				   struct tee_ioctl_param *params)
{
    printf("%s has not been realised!\n", __func__);
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_lseek(size_t num_params,
				   struct tee_ioctl_param *params)
{
	off_t ret = -1;
	int fd = -1;
	off_t offset = 0;
	int whence = 0;

	if (num_params != 3 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[2].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	fd = params[1].u.value.a;
	offset = params[1].u.value.b;
	whence = params[1].u.value.c;

	ret = lseek(fd, offset, whence);
	params[2].u.value.a = ret;
	if (ret == -1) params[2].u.value.b = errno;
	// printf("DMSG: call%s, fd=%d, offset=%d, whence=%d, ret=%d, errno=%s\n", __func__, fd, offset, whence, ret, strerror(errno));	
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_fsync(size_t num_params,
				   struct tee_ioctl_param *params)
{
    printf("%s has not been realised!\n", __func__);
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_getenv(size_t num_params,
				   struct tee_ioctl_param *params)
{
    printf("%s has not been realised!\n", __func__);
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_getpid(size_t num_params,
				   struct tee_ioctl_param *params)
{
	pid_t ret = -1;

	if (num_params != 2 ||
	    (params[0].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT ||
	    (params[1].attr & TEE_IOCTL_PARAM_ATTR_TYPE_MASK) !=
			TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;
	
	ret = getpid();
	params[1].u.value.a = ret;
	if (ret == -1) params[1].u.value.b = errno;
	printf("DMSG: call%s, ret=%d\n", __func__, ret);
	if (ret == -1) printf("errno=%d, errstr=%s\n", errno, strerror(errno));
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_time(size_t num_params,
				   struct tee_ioctl_param *params)
{
    printf("%s has not been realised!\n", __func__);
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_sleep(size_t num_params,
				   struct tee_ioctl_param *params)
{
    printf("%s has not been realised!\n", __func__);
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_gettimeofday(size_t num_params,
				   struct tee_ioctl_param *params)
{
    printf("%s has not been realised!\n", __func__);
	return TEEC_SUCCESS;
}

static TEEC_Result ree_tzvfs_fchown(size_t num_params,
				   struct tee_ioctl_param *params)
{
    printf("%s has not been realised!\n", __func__);
	return TEEC_SUCCESS;
}

TEEC_Result tee_supp_tzvfs_process(size_t num_params,
				struct tee_ioctl_param *params)
{
	switch (params->u.value.a) {
	case TZVFS_RPC_FS_OPEN:
		return ree_tzvfs_open(num_params, params);
	case TZVFS_RPC_FS_CLOSE:
		return ree_tzvfs_close(num_params, params);
	case TZVFS_RPC_FS_GETCWD:
		return ree_tzvfs_getcwd(num_params, params);
	case TZVFS_RPC_FS_LSTAT:
		return ree_tzvfs_lstat(num_params, params);
	case TZVFS_RPC_FS_STAT:
		return ree_tzvfs_stat(num_params, params);
	case TZVFS_RPC_FS_FSTAT:
		return ree_tzvfs_fstat(num_params, params);
	case TZVFS_RPC_FS_FCNTL:
		return ree_tzvfs_fcntl(num_params, params);
	case TZVFS_RPC_FS_READ:
		return ree_tzvfs_read(num_params, params);
	case TZVFS_RPC_FS_WRITE:
		return ree_tzvfs_write(num_params, params);
	case TZVFS_RPC_FS_GETEUID:
		return ree_tzvfs_geteuid(num_params, params);
	case TZVFS_RPC_FS_UNLINK:
		return ree_tzvfs_unlink(num_params, params);
	case TZVFS_RPC_FS_ACCESS:
		return ree_tzvfs_access(num_params, params);
	case TZVFS_RPC_FS_MMAP:
		return ree_tzvfs_mmap(num_params, params);
	case TZVFS_RPC_FS_MREMAP:
		return ree_tzvfs_mremap(num_params, params);
	case TZVFS_RPC_FS_MUNMAP:
		return ree_tzvfs_munmap(num_params, params);
	case TZVFS_RPC_FS_STRCSPN:
		return ree_tzvfs_strcspn(num_params, params);	
	case TZVFS_RPC_FS_UTIMES:
		return ree_tzvfs_utimes(num_params, params);
	case TZVFS_RPC_FS_LSEEK:
		return ree_tzvfs_lseek(num_params, params);
	case TZVFS_RPC_FS_FSYNC:
		return ree_tzvfs_fsync(num_params, params);
	case TZVFS_RPC_FS_GETENV:
		return ree_tzvfs_getenv(num_params, params);
	case TZVFS_RPC_FS_GETPID:
		return ree_tzvfs_getpid(num_params, params);
	case TZVFS_RPC_FS_TIME:
		return ree_tzvfs_time(num_params, params);
	case TZVFS_RPC_FS_SLEEP:
		return ree_tzvfs_sleep(num_params, params);
	case TZVFS_RPC_FS_GETTIMEOFDAY:
		return ree_tzvfs_gettimeofday(num_params, params);
	case TZVFS_RPC_FS_FCHOWN:
		return ree_tzvfs_fchown(num_params, params);
	default:
		return TEEC_ERROR_BAD_PARAMETERS;
	}
}
