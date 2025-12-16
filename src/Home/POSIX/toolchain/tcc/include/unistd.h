// unistd.h - POSIX standard definitions for ZealOS TCC
// Part of the POSIX compatibility layer

#ifndef _UNISTD_H
#define _UNISTD_H

#include <stddef.h>

// Standard file descriptors
#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

// Access modes
#define F_OK    0   // Test for existence
#define X_OK    1   // Test for execute permission
#define W_OK    2   // Test for write permission
#define R_OK    4   // Test for read permission

// Seek whence values
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

// Types
typedef long off_t;
typedef int pid_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;

// Process identification
pid_t getpid(void);
pid_t getppid(void);
pid_t getpgrp(void);
pid_t getpgid(pid_t pid);
int setpgid(pid_t pid, pid_t pgid);
pid_t setsid(void);
pid_t getsid(pid_t pid);

// User and group identification
uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);
int setuid(uid_t uid);
int seteuid(uid_t uid);
int setgid(gid_t gid);
int setegid(gid_t gid);
int getgroups(int size, gid_t list[]);
int setgroups(size_t size, const gid_t *list);

// Process creation
pid_t fork(void);
pid_t vfork(void);

// Program execution
int execve(const char *path, char *const argv[], char *const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg, ...);

// Process termination
void _exit(int status);

// File operations
int access(const char *path, int mode);
int faccessat(int dirfd, const char *path, int mode, int flags);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
ssize_t pread(int fd, void *buf, size_t count, off_t offset);
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);
off_t lseek(int fd, off_t offset, int whence);
int close(int fd);
int dup(int oldfd);
int dup2(int oldfd, int newfd);
int dup3(int oldfd, int newfd, int flags);
int pipe(int pipefd[2]);
int pipe2(int pipefd[2], int flags);

// File system
int link(const char *oldpath, const char *newpath);
int linkat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, int flags);
int unlink(const char *pathname);
int unlinkat(int dirfd, const char *pathname, int flags);
int symlink(const char *target, const char *linkpath);
int symlinkat(const char *target, int newdirfd, const char *linkpath);
ssize_t readlink(const char *path, char *buf, size_t bufsiz);
ssize_t readlinkat(int dirfd, const char *path, char *buf, size_t bufsiz);
int rmdir(const char *pathname);
int chdir(const char *path);
int fchdir(int fd);
char *getcwd(char *buf, size_t size);
int chown(const char *path, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int lchown(const char *path, uid_t owner, gid_t group);
int fchownat(int dirfd, const char *path, uid_t owner, gid_t group, int flags);
int truncate(const char *path, off_t length);
int ftruncate(int fd, off_t length);

// Terminal
int isatty(int fd);
char *ttyname(int fd);
int ttyname_r(int fd, char *buf, size_t buflen);

// Sleeping
unsigned int sleep(unsigned int seconds);
int usleep(unsigned int usec);
int pause(void);

// System identification
int gethostname(char *name, size_t len);
int sethostname(const char *name, size_t len);
long gethostid(void);
int sethostid(long hostid);
char *getlogin(void);
int getlogin_r(char *buf, size_t bufsize);

// Miscellaneous
int chroot(const char *path);
int nice(int inc);
void sync(void);
int fsync(int fd);
int fdatasync(int fd);
long sysconf(int name);
long fpathconf(int fd, int name);
long pathconf(const char *path, int name);
size_t confstr(int name, char *buf, size_t len);
int getopt(int argc, char * const argv[], const char *optstring);

// External variables for getopt
extern char *optarg;
extern int optind, opterr, optopt;

// Alarm and process signals
unsigned int alarm(unsigned int seconds);

// Environment
extern char **environ;

// sysconf names
#define _SC_ARG_MAX             0
#define _SC_CHILD_MAX           1
#define _SC_CLK_TCK             2
#define _SC_NGROUPS_MAX         3
#define _SC_OPEN_MAX            4
#define _SC_STREAM_MAX          5
#define _SC_TZNAME_MAX          6
#define _SC_PAGESIZE            30
#define _SC_PAGE_SIZE           _SC_PAGESIZE
#define _SC_NPROCESSORS_CONF    83
#define _SC_NPROCESSORS_ONLN    84

#endif // _UNISTD_H
