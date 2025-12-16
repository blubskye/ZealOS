// fcntl.h - File control for ZealOS TCC
// Part of the POSIX compatibility layer

#ifndef _FCNTL_H
#define _FCNTL_H

#include <sys/types.h>

// File access modes
#define O_RDONLY    0x0000
#define O_WRONLY    0x0001
#define O_RDWR      0x0002
#define O_ACCMODE   0x0003

// File creation flags
#define O_CREAT     0x0040
#define O_EXCL      0x0080
#define O_NOCTTY    0x0100
#define O_TRUNC     0x0200

// File status flags
#define O_APPEND    0x0400
#define O_NONBLOCK  0x0800
#define O_NDELAY    O_NONBLOCK
#define O_SYNC      0x1000
#define O_DSYNC     0x1000
#define O_RSYNC     0x1000
#define O_ASYNC     0x2000

// Linux extensions
#define O_DIRECTORY 0x10000
#define O_NOFOLLOW  0x20000
#define O_CLOEXEC   0x80000
#define O_DIRECT    0x04000
#define O_NOATIME   0x40000
#define O_PATH      0x200000
#define O_TMPFILE   0x400000

// fcntl commands
#define F_DUPFD         0   // Duplicate file descriptor
#define F_GETFD         1   // Get file descriptor flags
#define F_SETFD         2   // Set file descriptor flags
#define F_GETFL         3   // Get file status flags
#define F_SETFL         4   // Set file status flags
#define F_GETLK         5   // Get record locking info
#define F_SETLK         6   // Set record locking info
#define F_SETLKW        7   // Set record locking info; wait if blocked
#define F_SETOWN        8   // Set owner for SIGIO
#define F_GETOWN        9   // Get owner for SIGIO
#define F_SETSIG        10  // Set signal for SIGIO
#define F_GETSIG        11  // Get signal for SIGIO
#define F_DUPFD_CLOEXEC 1030 // Duplicate with O_CLOEXEC

// File descriptor flags
#define FD_CLOEXEC  1   // Close on exec

// Advisory record locking
#define F_RDLCK     0   // Read lock
#define F_WRLCK     1   // Write lock
#define F_UNLCK     2   // Remove lock

// For posix_fadvise
#define POSIX_FADV_NORMAL       0
#define POSIX_FADV_RANDOM       1
#define POSIX_FADV_SEQUENTIAL   2
#define POSIX_FADV_WILLNEED     3
#define POSIX_FADV_DONTNEED     4
#define POSIX_FADV_NOREUSE      5

// File lock structure
struct flock {
    short   l_type;     // Type of lock
    short   l_whence;   // Where l_start is relative to
    off_t   l_start;    // Offset where lock begins
    off_t   l_len;      // Length of lock; 0 means until EOF
    pid_t   l_pid;      // Process holding lock
};

// Function declarations
int open(const char *pathname, int flags, ...);
int openat(int dirfd, const char *pathname, int flags, ...);
int creat(const char *pathname, mode_t mode);
int fcntl(int fd, int cmd, ...);

// POSIX
int posix_fadvise(int fd, off_t offset, off_t len, int advice);
int posix_fallocate(int fd, off_t offset, off_t len);

// File lock convenience functions
int lockf(int fd, int cmd, off_t len);

// lockf commands
#define F_ULOCK 0   // Unlock
#define F_LOCK  1   // Exclusive lock
#define F_TLOCK 2   // Test and lock
#define F_TEST  3   // Test if locked

// AT_* flags for *at functions
#define AT_FDCWD            -100    // Use current working directory
#define AT_SYMLINK_NOFOLLOW 0x100   // Do not follow symbolic links
#define AT_REMOVEDIR        0x200   // Remove directory instead of file
#define AT_SYMLINK_FOLLOW   0x400   // Follow symbolic links
#define AT_EACCESS          0x200   // Test access for effective IDs
#define AT_EMPTY_PATH       0x1000  // Allow empty relative pathname

#endif // _FCNTL_H
