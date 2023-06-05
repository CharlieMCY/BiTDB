#ifndef TEE_SUPP_TZVFS_H
#define TEE_SUPP_TZVFS_H

#include <tee_client_api.h>
struct tee_ioctl_param;
TEEC_Result tee_supp_tzvfs_process(size_t num_params,
				struct tee_ioctl_param *params);

// TZVFS define start
#define TZVFS_FS_NAME_MAX 350

/*
 * tzvfs_open
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_OPEN
 *          value[0].b      flags
 *          value[0].c      mode
 * [in]     memref[1]	      A string holding the file name
 * [out]    value[2].a	    File descriptor of open file
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_OPEN		0

/*
 * tzvfs_close
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_CLOSE
 *          value[0].b      fd
 * [out]    value[1].a	    ret
 *          value[1].b      errno
 */
#define TZVFS_RPC_FS_CLOSE		1

/*
 * tzvfs_getcwd
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_GETCWD
 * [out]    memref[1]	      A string holding the CWD name
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_GETCWD		2

/*
 * tzvfs_lstat
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_LSTAT
 * [in]     memref[1]	      A string holding the path name
 * [out]    memref[2]	      struct tzvfs_stat
 * [out]    value[3].a	    ret
 *          value[3].b      errno
 */
#define TZVFS_RPC_FS_LSTAT		3

/*
 * tzvfs_stat
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_STAT
 * [in]     memref[1]	      A string holding the path name
 * [out]    memref[2]	      struct tzvfs_stat
 * [out]    value[3].a	    ret
 *          value[3].b      errno
 */
#define TZVFS_RPC_FS_STAT		4

/*
 * tzvfs_fstat
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_FSTAT
 *          value[0].b      fd
 * [out]    memref[1]	      struct tzvfs_stat
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_FSTAT		5

/*
 * tzvfs_fcntl
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_FCNTL
 *          value[0].b      fd
 *          value[0].c      cmd
 * [out]    memref[1]	      struct tzvfs_flock
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_FCNTL		6

/*
 * tzvfs_read
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_READ
 *          value[0].b      fd
 * [out]    memref[1]	      buf
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_READ		7

/*
 * tzvfs_write
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_WRITE
 *          value[0].b      fd
 * [in]     memref[1]	      buf
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_WRITE		8

/*
 * tzvfs_geteuid
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_GETEUID
 * [out]    value[1].a	    ret
 *          value[1].b      errno
 */
#define TZVFS_RPC_FS_GETEUID		9

/*
 * tzvfs_unlink
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_UNLINK
 * [in]     memref[1]	      A string holding the pathname
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_UNLINK		10

/*
 * tzvfs_access
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_ACCESS
 *          value[0].b      mode
 * [in]     memref[1]	      A string holding the pathname
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_ACCESS		11

/*
 * tzvfs_mmap
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_MMAP
 * [in]     value[1].a	    addr
 *          value[1].b      len
 *          value[1].c      prot
 * [in]     value[2].a	    flags
 *          value[2].b      fildes
 *          value[2].c      off
 * [out]    value[3].a	    ret
 *          value[3].b      errno
 */
#define TZVFS_RPC_FS_MMAP		12

/*
 * tzvfs_mremap
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_MREMAP
 *          value[0].b      old_address
 *          value[0].c      old_size
 * [in]     value[1].a	    new_size
 *          value[1].b      flags
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_MREMAP		13

/*
 * tzvfs_munmap
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_MUNMAP
 *          value[0].b      addr
 *          value[0].c      length
 * [out]    value[1].a	    ret
 *          value[1].b      errno
 */
#define TZVFS_RPC_FS_MUNMAP		14

/*
 * tzvfs_strcspn
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_STRCSPN
 * [in]     memref[1]	      A string holding the str1
 * [in]     memref[2]	      A string holding the str1
 * [out]    value[3].a	    ret
 */
#define TZVFS_RPC_FS_STRCSPN		15


/*
 * tzvfs_utimes
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_UTIMES
 * [in]     memref[1]	      A string holding the filename
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_UTIMES		16

/*
 * tzvfs_lseek
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_LSEEK
 * [in]     value[1].a	    fd
 *          value[1].b      offset
 *          value[1].c      whence
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_LSEEK		17

/*
 * tzvfs_fsync
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_FSYNC
 * [in]     value[1].a	    fd
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_FSYNC		18

/*
 * tzvfs_getenv
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_GETENV
 * [in]     memref[1]	      A string holding the name
 * [out]    value[2].a	    ret
 */
#define TZVFS_RPC_FS_GETENV		20

/*
 * tzvfs_getpid
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_GETPID
 * [out]    value[1].a	    ret
 *          value[1].b      errno
 */
#define TZVFS_RPC_FS_GETPID		21

/*
 * tzvfs_time
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_TIME
 * [out]    value[1].a	    ret
 *          value[1].b      errno
 */
#define TZVFS_RPC_FS_TIME		22

/*
 * tzvfs_sleep
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_SLEEP
 *          value[0].b      seconds
 * [out]    value[1].a	    ret
 */
#define TZVFS_RPC_FS_SLEEP		23


/*
 * tzvfs_gettimeofday
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_GETTIMEOFDAY
 * [out]    memref[1]	      struct tzvfs_timeval
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_GETTIMEOFDAY		24

/*
 * tzvfs_fchown
 *
 * [in]     value[0].a	    TZVFS_RPC_FS_FCHOWN
 * [in]     value[1].a	    fd
 *          value[1].b      owner
 *          value[1].c      group
 * [out]    value[2].a	    ret
 *          value[2].b      errno
 */
#define TZVFS_RPC_FS_FCHOWN		25
// TZVFS define end

#endif