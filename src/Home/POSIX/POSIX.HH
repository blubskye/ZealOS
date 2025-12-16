// POSIX.HH - POSIX Compatibility Layer for ZealOS
// Provides POSIX-like functionality to enable GNU tools and POSIX shell
//
// This layer maps POSIX concepts to ZealOS equivalents:
// - File descriptors -> ZealOS file handles
// - Processes -> ZealOS tasks
// - Signals -> ZealOS messages
// - fork() -> Spawn() based emulation

// ============================================================================
// POSIX LIMITS
// ============================================================================

#define POSIX_PATH_MAX          4096
#define POSIX_NAME_MAX          255
#define POSIX_OPEN_MAX          1024
#define POSIX_ARG_MAX           131072
#define POSIX_PIPE_BUF          4096
#define POSIX_CHILD_MAX         256
#define POSIX_ENV_MAX           32767

// ============================================================================
// FILE DESCRIPTOR CONSTANTS
// ============================================================================

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

// File descriptor types
#define FD_TYPE_NONE    0
#define FD_TYPE_FILE    1
#define FD_TYPE_PIPE    2
#define FD_TYPE_SOCKET  3
#define FD_TYPE_TTY     4
#define FD_TYPE_DIR     5

// ============================================================================
// OPEN FLAGS (fcntl.h)
// ============================================================================

#define O_RDONLY        0x0000
#define O_WRONLY        0x0001
#define O_RDWR          0x0002
#define O_ACCMODE       0x0003

#define O_CREAT         0x0040
#define O_EXCL          0x0080
#define O_NOCTTY        0x0100
#define O_TRUNC         0x0200
#define O_APPEND        0x0400
#define O_NONBLOCK      0x0800
#define O_SYNC          0x1000
#define O_DIRECTORY     0x10000
#define O_NOFOLLOW      0x20000
#define O_CLOEXEC       0x80000

// ============================================================================
// FILE MODE BITS (sys/stat.h)
// ============================================================================

#define S_IFMT      0170000     // File type mask
#define S_IFSOCK    0140000     // Socket
#define S_IFLNK     0120000     // Symbolic link
#define S_IFREG     0100000     // Regular file
#define S_IFBLK     0060000     // Block device
#define S_IFDIR     0040000     // Directory
#define S_IFCHR     0020000     // Character device
#define S_IFIFO     0010000     // FIFO/pipe

#define S_ISUID     0004000     // Set-user-ID
#define S_ISGID     0002000     // Set-group-ID
#define S_ISVTX     0001000     // Sticky bit

#define S_IRWXU     0000700     // Owner RWX
#define S_IRUSR     0000400     // Owner read
#define S_IWUSR     0000200     // Owner write
#define S_IXUSR     0000100     // Owner execute

#define S_IRWXG     0000070     // Group RWX
#define S_IRGRP     0000040     // Group read
#define S_IWGRP     0000020     // Group write
#define S_IXGRP     0000010     // Group execute

#define S_IRWXO     0000007     // Other RWX
#define S_IROTH     0000004     // Other read
#define S_IWOTH     0000002     // Other write
#define S_IXOTH     0000001     // Other execute

// File type test macros
#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)

// ============================================================================
// SEEK CONSTANTS (unistd.h)
// ============================================================================

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

// ============================================================================
// ERRNO VALUES
// ============================================================================

#define EPERM           1       // Operation not permitted
#define ENOENT          2       // No such file or directory
#define ESRCH           3       // No such process
#define EINTR           4       // Interrupted system call
#define EIO             5       // I/O error
#define ENXIO           6       // No such device or address
#define E2BIG           7       // Argument list too long
#define ENOEXEC         8       // Exec format error
#define EBADF           9       // Bad file descriptor
#define ECHILD          10      // No child processes
#define EAGAIN          11      // Try again / Would block
#define EWOULDBLOCK     EAGAIN
#define ENOMEM          12      // Out of memory
#define EACCES          13      // Permission denied
#define EFAULT          14      // Bad address
#define ENOTBLK         15      // Block device required
#define EBUSY           16      // Device or resource busy
#define EEXIST          17      // File exists
#define EXDEV           18      // Cross-device link
#define ENODEV          19      // No such device
#define ENOTDIR         20      // Not a directory
#define EISDIR          21      // Is a directory
#define EINVAL          22      // Invalid argument
#define ENFILE          23      // File table overflow
#define EMFILE          24      // Too many open files
#define ENOTTY          25      // Not a typewriter
#define ETXTBSY         26      // Text file busy
#define EFBIG           27      // File too large
#define ENOSPC          28      // No space left on device
#define ESPIPE          29      // Illegal seek
#define EROFS           30      // Read-only file system
#define EMLINK          31      // Too many links
#define EPIPE           32      // Broken pipe
#define EDOM            33      // Math argument out of domain
#define ERANGE          34      // Math result not representable
#define EDEADLK         35      // Resource deadlock would occur
#define ENAMETOOLONG    36      // File name too long
#define ENOLCK          37      // No record locks available
#define ENOSYS          38      // Function not implemented
#define ENOTEMPTY       39      // Directory not empty
#define ELOOP           40      // Too many symbolic links
#define ENOMSG          42      // No message of desired type
#define EIDRM           43      // Identifier removed
#define ENOTSOCK        88      // Socket operation on non-socket
#define ECONNRESET      104     // Connection reset by peer
#define ENOBUFS         105     // No buffer space available
#define EISCONN         106     // Transport endpoint connected
#define ENOTCONN        107     // Transport endpoint not connected
#define ETIMEDOUT       110     // Connection timed out
#define ECONNREFUSED    111     // Connection refused
#define EINPROGRESS     115     // Operation now in progress

// ============================================================================
// SIGNAL NUMBERS
// ============================================================================

#define SIGHUP          1       // Hangup
#define SIGINT          2       // Interrupt (Ctrl+C)
#define SIGQUIT         3       // Quit (Ctrl+\)
#define SIGILL          4       // Illegal instruction
#define SIGTRAP         5       // Trace/breakpoint trap
#define SIGABRT         6       // Abort
#define SIGBUS          7       // Bus error
#define SIGFPE          8       // Floating point exception
#define SIGKILL         9       // Kill (cannot be caught)
#define SIGUSR1         10      // User-defined signal 1
#define SIGSEGV         11      // Segmentation fault
#define SIGUSR2         12      // User-defined signal 2
#define SIGPIPE         13      // Broken pipe
#define SIGALRM         14      // Alarm clock
#define SIGTERM         15      // Termination
#define SIGSTKFLT       16      // Stack fault
#define SIGCHLD         17      // Child stopped or terminated
#define SIGCONT         18      // Continue if stopped
#define SIGSTOP         19      // Stop (cannot be caught)
#define SIGTSTP         20      // Stop typed at terminal (Ctrl+Z)
#define SIGTTIN         21      // Background read from tty
#define SIGTTOU         22      // Background write to tty
#define SIGURG          23      // Urgent condition on socket
#define SIGXCPU         24      // CPU time limit exceeded
#define SIGXFSZ         25      // File size limit exceeded
#define SIGVTALRM       26      // Virtual alarm clock
#define SIGPROF         27      // Profiling timer expired
#define SIGWINCH        28      // Window resize signal
#define SIGIO           29      // I/O now possible
#define SIGPWR          30      // Power failure
#define SIGSYS          31      // Bad system call

#define NSIG            32      // Number of signals

#define SIG_DFL         0       // Default signal handling
#define SIG_IGN         1       // Ignore signal
#define SIG_ERR         (-1)    // Error return

// Signal action flags
#define SA_NOCLDSTOP    0x00000001
#define SA_NOCLDWAIT    0x00000002
#define SA_SIGINFO      0x00000004
#define SA_RESTART      0x10000000
#define SA_NODEFER      0x40000000
#define SA_RESETHAND    0x80000000

// ============================================================================
// WAIT FLAGS
// ============================================================================

#define WNOHANG         1       // Don't block waiting
#define WUNTRACED       2       // Report stopped children

// Wait status macros
#define WIFEXITED(s)    (((s) & 0x7F) == 0)
#define WEXITSTATUS(s)  (((s) >> 8) & 0xFF)
#define WIFSIGNALED(s)  (((s) & 0x7F) > 0 && ((s) & 0x7F) < 0x7F)
#define WTERMSIG(s)     ((s) & 0x7F)
#define WIFSTOPPED(s)   (((s) & 0xFF) == 0x7F)
#define WSTOPSIG(s)     (((s) >> 8) & 0xFF)
#define WCOREDUMP(s)    ((s) & 0x80)

// ============================================================================
// FCNTL COMMANDS
// ============================================================================

#define F_DUPFD         0       // Duplicate file descriptor
#define F_GETFD         1       // Get file descriptor flags
#define F_SETFD         2       // Set file descriptor flags
#define F_GETFL         3       // Get file status flags
#define F_SETFL         4       // Set file status flags
#define F_GETLK         5       // Get record locking info
#define F_SETLK         6       // Set record locking info
#define F_SETLKW        7       // Set record locking info (blocking)
#define F_DUPFD_CLOEXEC 1030    // Duplicate with close-on-exec

#define FD_CLOEXEC      1       // Close-on-exec flag

// ============================================================================
// POSIX STRUCTURES
// ============================================================================

// File status structure
class CPosixStat
{
	U64     st_dev;         // Device ID
	U64     st_ino;         // Inode number
	U32     st_mode;        // File type and mode
	U32     st_nlink;       // Number of hard links
	U32     st_uid;         // User ID
	U32     st_gid;         // Group ID
	U64     st_rdev;        // Device ID (if special file)
	I64     st_size;        // Total size in bytes
	I64     st_blksize;     // Block size for filesystem I/O
	I64     st_blocks;      // Number of 512B blocks allocated
	U64     st_atime;       // Access time
	U64     st_mtime;       // Modification time
	U64     st_ctime;       // Status change time
};

// Directory entry
class CPosixDirent
{
	U64     d_ino;          // Inode number
	I64     d_off;          // Offset to next entry
	U16     d_reclen;       // Length of this record
	U8      d_type;         // Type of file
	U8      d_name[256];    // Filename
};

// Directory entry types
#define DT_UNKNOWN      0
#define DT_FIFO         1
#define DT_CHR          2
#define DT_DIR          4
#define DT_BLK          6
#define DT_REG          8
#define DT_LNK          10
#define DT_SOCK         12

// Signal set (64 signals max)
class CPosixSigSet
{
	U64     bits;
};

// Signal action structure
class CPosixSigAction
{
	I64     sa_handler;     // Signal handler or SIG_DFL/SIG_IGN
	U64     sa_flags;       // Signal action flags
	CPosixSigSet sa_mask;   // Signals to block during handler
};

// Time value structure
class CPosixTimeVal
{
	I64     tv_sec;         // Seconds
	I64     tv_usec;        // Microseconds
};

class CPosixTimeSpec
{
	I64     tv_sec;         // Seconds
	I64     tv_nsec;        // Nanoseconds
};

// ============================================================================
// PIPE STRUCTURE
// ============================================================================

#define PIPE_BUFFER_SIZE    65536

class CPosixPipe
{
	U8     *buffer;         // Circular buffer
	I64     size;           // Buffer size
	I64     read_pos;       // Read position
	I64     write_pos;      // Write position
	I64     count;          // Bytes in buffer
	Bool    read_closed;    // Read end closed
	Bool    write_closed;   // Write end closed
	I64     readers;        // Number of readers
	I64     writers;        // Number of writers
};

// ============================================================================
// FILE DESCRIPTOR STRUCTURE
// ============================================================================

class CPosixFD
{
	I64     type;           // FD_TYPE_*
	I64     flags;          // O_* flags
	I64     fd_flags;       // FD_CLOEXEC etc.
	I64     offset;         // Current file position
	I64     ref_count;      // Reference count for dup()

	// Type-specific data
	union {
		// For FD_TYPE_FILE
		struct {
			U8     *path;           // File path
			I64     handle;         // ZealOS file handle (or -1 for memory)
			U8     *data;           // File data (for read files)
			I64     data_size;      // Size of data
		};
		// For FD_TYPE_PIPE
		struct {
			CPosixPipe *pipe;       // Pipe object
			Bool    is_read_end;    // TRUE if this is read end
		};
		// For FD_TYPE_SOCKET
		struct {
			I64     socket;         // Socket handle
		};
		// For FD_TYPE_DIR
		struct {
			U8     *dir_path;       // Directory path
			CDirEntry *entries;     // Directory entries
			I64     entry_count;    // Number of entries
			I64     entry_pos;      // Current position
		};
	};
};

// ============================================================================
// PROCESS STRUCTURE
// ============================================================================

class CPosixProcess
{
	I64     pid;                            // Process ID
	I64     ppid;                           // Parent process ID
	I64     pgid;                           // Process group ID
	I64     sid;                            // Session ID

	CTask  *task;                           // ZealOS task

	// File descriptors
	CPosixFD *fd_table[POSIX_OPEN_MAX];
	I64     fd_count;                       // Number of open FDs

	// Environment
	U8    **environ;                        // Environment variables
	I64     environ_count;                  // Number of env vars
	I64     environ_capacity;               // Capacity

	// Current directory
	U8     *cwd;                            // Current working directory

	// Signal handling
	CPosixSigAction sigactions[NSIG];       // Signal handlers
	CPosixSigSet    sigmask;                // Blocked signals
	CPosixSigSet    sigpending;             // Pending signals

	// Exit status
	I64     exit_status;                    // Exit code
	Bool    exited;                         // Has exited
	Bool    signaled;                       // Killed by signal
	I64     term_signal;                    // Terminating signal

	// vfork state
	Bool    vforked;                        // Was created by vfork
	CPosixProcess *vfork_parent;            // Parent for vfork

	// Child processes
	CPosixProcess *children[POSIX_CHILD_MAX];
	I64     child_count;

	// Process tree links
	CPosixProcess *parent;
	CPosixProcess *next_sibling;
	CPosixProcess *first_child;

	// Command info
	U8     *argv0;                          // Program name
	U8    **argv;                           // Arguments
	I64     argc;                           // Argument count

	// Virtual memory context (for true fork with COW)
	U64     vm_context;                     // Pointer to CVMContext (cast from VM.HH)
};

// ============================================================================
// GLOBAL STATE
// ============================================================================

// Current process (per-task, stored in task-local storage)
extern CPosixProcess *posix_current;

// Process table
extern CPosixProcess *posix_processes[POSIX_CHILD_MAX];
extern I64 posix_process_count;
extern I64 posix_next_pid;

// Global errno (per-task in real implementation)
extern I64 posix_errno;

// ============================================================================
// FUNCTION DECLARATIONS - File Descriptors
// ============================================================================

// File descriptor management
I64 PosixFDAlloc(CPosixProcess *proc);
I64 PosixFDAllocAbove(CPosixProcess *proc, I64 min);
U0 PosixFDFree(CPosixProcess *proc, I64 fd);
CPosixFD *PosixFDGet(CPosixProcess *proc, I64 fd);
I64 PosixFDDup(CPosixProcess *proc, I64 oldfd);
I64 PosixFDDup2(CPosixProcess *proc, I64 oldfd, I64 newfd);

// ============================================================================
// FUNCTION DECLARATIONS - File Operations
// ============================================================================

I64 PosixOpen(U8 *path, I64 flags, I64 mode);
I64 PosixClose(I64 fd);
I64 PosixRead(I64 fd, U8 *buf, I64 count);
I64 PosixWrite(I64 fd, U8 *buf, I64 count);
I64 PosixLseek(I64 fd, I64 offset, I64 whence);
I64 PosixFstat(I64 fd, CPosixStat *buf);
I64 PosixStat(U8 *path, CPosixStat *buf);
I64 PosixLstat(U8 *path, CPosixStat *buf);
I64 PosixFcntl(I64 fd, I64 cmd, I64 arg);
I64 PosixAccess(U8 *path, I64 mode);
I64 PosixUnlink(U8 *path);
I64 PosixRename(U8 *oldpath, U8 *newpath);
I64 PosixMkdir(U8 *path, I64 mode);
I64 PosixRmdir(U8 *path);
I64 PosixChdir(U8 *path);
U8 *PosixGetcwd(U8 *buf, I64 size);

// ============================================================================
// FUNCTION DECLARATIONS - Pipe Operations
// ============================================================================

I64 PosixPipe(I64 *pipefd);
I64 PosixPipe2(I64 *pipefd, I64 flags);

// ============================================================================
// FUNCTION DECLARATIONS - Directory Operations
// ============================================================================

I64 PosixOpenDir(U8 *path);
CPosixDirent *PosixReadDir(I64 fd);
I64 PosixCloseDir(I64 fd);
U0 PosixRewindDir(I64 fd);

// ============================================================================
// FUNCTION DECLARATIONS - Process Operations
// ============================================================================

I64 PosixGetPid();
I64 PosixGetPPid();
I64 PosixGetUid();
I64 PosixGetGid();
I64 PosixGetEuid();
I64 PosixGetEgid();

// Fork emulation (spawn-based)
I64 PosixFork();
I64 PosixVfork();

// Exec family
I64 PosixExecve(U8 *path, U8 **argv, U8 **envp);
I64 PosixExecvp(U8 *file, U8 **argv);
I64 PosixExecv(U8 *path, U8 **argv);

// Wait family
I64 PosixWait(I64 *status);
I64 PosixWaitpid(I64 pid, I64 *status, I64 options);

// Exit
U0 PosixExit(I64 status);
U0 Posix_Exit(I64 status);

// ============================================================================
// FUNCTION DECLARATIONS - Signal Operations
// ============================================================================

I64 PosixKill(I64 pid, I64 sig);
I64 PosixRaise(I64 sig);
I64 PosixSignal(I64 signum, I64 handler);
I64 PosixSigaction(I64 signum, CPosixSigAction *act, CPosixSigAction *oldact);
I64 PosixSigprocmask(I64 how, CPosixSigSet *set, CPosixSigSet *oldset);
I64 PosixSigsuspend(CPosixSigSet *mask);
I64 PosixPause();
I64 PosixAlarm(I64 seconds);

// Signal set operations
U0 PosixSigemptyset(CPosixSigSet *set);
U0 PosixSigfillset(CPosixSigSet *set);
U0 PosixSigaddset(CPosixSigSet *set, I64 signum);
U0 PosixSigdelset(CPosixSigSet *set, I64 signum);
I64 PosixSigismember(CPosixSigSet *set, I64 signum);

// ============================================================================
// FUNCTION DECLARATIONS - Environment Operations
// ============================================================================

U8 *PosixGetenv(U8 *name);
I64 PosixSetenv(U8 *name, U8 *value, I64 overwrite);
I64 PosixUnsetenv(U8 *name);
I64 PosixPutenv(U8 *string);
I64 PosixClearenv();

// ============================================================================
// FUNCTION DECLARATIONS - Terminal Operations
// ============================================================================

I64 PosixIsatty(I64 fd);
U8 *PosixTtyname(I64 fd);
I64 PosixTcgetpgrp(I64 fd);
I64 PosixTcsetpgrp(I64 fd, I64 pgrp);

// ============================================================================
// FUNCTION DECLARATIONS - Misc
// ============================================================================

I64 PosixSleep(I64 seconds);
I64 PosixUsleep(I64 usec);
I64 PosixNanosleep(CPosixTimeSpec *req, CPosixTimeSpec *rem);
I64 PosixGetTimeOfDay(CPosixTimeVal *tv, U0 *tz);
I64 PosixTime(I64 *t);

// ============================================================================
// ERROR HANDLING
// ============================================================================

// Set errno and return -1
I64 PosixSetErrno(I64 err);

// Get error string
U8 *PosixStrerror(I64 errnum);

// ============================================================================
// INITIALIZATION
// ============================================================================

// Initialize POSIX layer for current task
CPosixProcess *PosixInit();

// Clean up POSIX layer for current task
U0 PosixCleanup(CPosixProcess *proc);

// Get current process
CPosixProcess *PosixGetCurrent();

