#ifndef TZVFS_H_
#define TZVFS_H_

#include <stdio.h>
#include <stdarg.h> // for variable arguments functions
#include <stdlib.h>
#include <string.h>

# define MREMAP_MAYMOVE	1

// errno code start
#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		 5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC	 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK	15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR	20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY	26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */
#define	ETIMEDOUT	110	/* Connection timed out */
#define	ENOLCK		37	/* No record locks available */
// errno code end

// type which optee not supported start
typedef int __pid_t;
typedef __pid_t pid_t;
typedef unsigned int __uid_t;
typedef __uid_t uid_t;
typedef unsigned int __gid_t;
typedef __gid_t gid_t;
typedef unsigned long long int __dev_t;
typedef __dev_t dev_t;
typedef unsigned long int __ino_t;
typedef unsigned long long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned int mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long long int __off64_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef long time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef long int __blksize_t;
typedef long int __blkcnt_t;
typedef long long int __blkcnt64_t;
typedef __off_t off_t;
typedef long int __syscall_slong_t;
typedef long ssize_t;
// type which optee not supported end

// unixFullPathname marco start
# define __S_IFDIR 0040000
# define __S_IFLNK 0120000
# define __S_IFMT 0170000
# define __S_IFREG 0100000
# define __S_ISTYPE(mode, mask)	(((mode) & __S_IFMT) == (mask))
# define S_ISDIR(mode)	 __S_ISTYPE((mode), __S_IFDIR)
# define S_ISLNK(mode)	 __S_ISTYPE((mode), __S_IFLNK)
# define S_ISREG(mode)	 __S_ISTYPE((mode), __S_IFREG)
// unixFullPathname marco end

// lseek start
# define SEEK_SET	0	/* Seek from beginning of file.  */
# define SEEK_CUR	1	/* Seek from current position.  */
# define SEEK_END	2	/* Seek from end of file.  */
// lseek end

// mmap types start
# define PROT_READ	0x1		/* Page can be read.  */
# define MAP_FAILED	((void *) -1)
# define MAP_SHARED	0x01		/* Share changes.  */
// mman types end

// unixFullPathname marco start
# define __S_IFDIR 0040000
# define __S_IFLNK 0120000
# define __S_IFMT 0170000
# define __S_IFREG 0100000
# define __S_ISTYPE(mode, mask)	(((mode) & __S_IFMT) == (mask))
# define S_ISDIR(mode)	 __S_ISTYPE((mode), __S_IFDIR)
# define S_ISLNK(mode)	 __S_ISTYPE((mode), __S_IFLNK)
# define S_ISREG(mode)	 __S_ISTYPE((mode), __S_IFREG)
// unixFullPathname marco end

// open flags start
# define O_RDONLY	     00
# define O_RDWR		     02
# define O_CREAT	   0100	/* Not fcntl.  */
# define O_EXCL		   0200	/* Not fcntl.  */
// open flags end

// access flags start
# define	R_OK	4		/* Test for read permission.  */
# define	W_OK	2		/* Test for write permission.  */
# define	F_OK	0		/* Test for existence.  */
// access flags end

// tzvfs_stat start
struct tzvfs_timespec
{
  __time_t tv_sec;		/* Seconds.  */
  __syscall_slong_t tv_nsec;	/* Nanoseconds.  */
};
struct tzvfs_stat
  {
    __dev_t st_dev;			/* Device.  */
    unsigned short int __pad1;
    __ino_t __st_ino;			/* 32bit file serial number.	*/
    __mode_t st_mode;			/* File mode.  */
    __nlink_t st_nlink;			/* Link count.  */
    __uid_t st_uid;			/* User ID of the file's owner.	*/
    __gid_t st_gid;			/* Group ID of the file's group.*/
    __dev_t st_rdev;			/* Device number, if device.  */
    unsigned short int __pad2;
    __off64_t st_size;			/* Size of file, in bytes.  */
    __blksize_t st_blksize;		/* Optimal block size for I/O.  */
    __blkcnt64_t st_blocks;		/* Number 512-byte blocks allocated. */
    /* Nanosecond resolution timestamps are stored in a format
       equivalent to 'struct timespec'.  This is the type used
       whenever possible but the Unix namespace rules do not allow the
       identifier 'timespec' to appear in the <sys/stat.h> header.
       Therefore we have to handle the use of this header in strictly
       standard-compliant sources special.  */
    struct tzvfs_timespec st_atim;		/* Time of last access.  */
    struct tzvfs_timespec st_mtim;		/* Time of last modification.  */
    struct tzvfs_timespec st_ctim;		/* Time of last status change.  */
    __ino64_t st_ino;			/* File serial number.	*/
  };
// tzvfs_stat end

// tzvfs_flock start
struct tzvfs_flock
  {
    short int l_type;	/* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.	*/
    short int l_whence;	/* Where `l_start' is relative to (like `lseek').  */
    __off64_t l_start;	/* Offset where the lock begins.  */
    __off64_t l_len;	/* Size of the locked area; zero means until EOF.  */
    __pid_t l_pid;	/* Process holding the lock.  */
  };

# define F_RDLCK		0	/* Read lock.  */
# define F_WRLCK		1	/* Write lock.  */
# define F_UNLCK		2	/* Remove lock.  */
# define F_GETLK	    5	/* Get record locking info.  */
# define F_SETLK	    6	/* Set record locking info (non-blocking).  */
// tzvfs_flock end

// time types start
struct tzvfs_timeval
{
  __time_t tv_sec;		/* Seconds.  */
  __suseconds_t tv_usec;	/* Microseconds.  */
};
struct tzvfs_timezone
  {
    int tz_minuteswest;		/* Minutes west of GMT.  */
    int tz_dsttime;		/* Nonzero if DST is ever in effect.  */
  };
struct tzvfs_tm
{
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year	- 1900.  */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/

  long int tm_gmtoff;		/* Seconds east of UTC.  */
  const char *tm_zone;		/* Timezone abbreviation.  */
};
// time types end

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
