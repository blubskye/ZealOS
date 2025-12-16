# ZealOS Toolchain Phases B-D: libc, LLVM, and Go

## Overview

This document outlines the remaining phases to achieve a full C/C++ toolchain and Go support on ZealOS.

**Completed:**
- Phase A1: Shell interpreter (sh) ✓
- Phase A2: Build tools (make, install) ✓
- Phase A3: Text tools (sed, awk, find, xargs) ✓
- Phase A4: Compression (tar, gzip, bzip2) ✓

**Remaining:**
- Phase B: Complete libc
- Phase C: LLVM/Clang toolchain
- Phase D: Go language port

---

## Phase B: Complete libc

### B1: Threading (pthreads)

ZealOS is single-core but many programs expect pthreads. Implement as cooperative threading or stubs.

**Files to create:**
```
/Home/POSIX/libc/pthread.HH
/Home/POSIX/libc/pthread.ZC
```

**Key types:**
```c
typedef I64 pthread_t;
typedef struct { I64 flags; } pthread_attr_t;
typedef struct { I64 lock; } pthread_mutex_t;
typedef struct { I64 value; } pthread_cond_t;
typedef struct { I64 count; } pthread_rwlock_t;
typedef I64 pthread_key_t;  // Thread-local storage
```

**Key functions:**
```c
// Thread management
I64 pthread_create(pthread_t *thread, pthread_attr_t *attr,
                   void *(*start)(void*), void *arg);
I64 pthread_join(pthread_t thread, void **retval);
I64 pthread_detach(pthread_t thread);
pthread_t pthread_self();
I64 pthread_equal(pthread_t t1, pthread_t t2);
void pthread_exit(void *retval);

// Mutexes
I64 pthread_mutex_init(pthread_mutex_t *mutex, void *attr);
I64 pthread_mutex_destroy(pthread_mutex_t *mutex);
I64 pthread_mutex_lock(pthread_mutex_t *mutex);
I64 pthread_mutex_trylock(pthread_mutex_t *mutex);
I64 pthread_mutex_unlock(pthread_mutex_t *mutex);

// Condition variables
I64 pthread_cond_init(pthread_cond_t *cond, void *attr);
I64 pthread_cond_destroy(pthread_cond_t *cond);
I64 pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
I64 pthread_cond_signal(pthread_cond_t *cond);
I64 pthread_cond_broadcast(pthread_cond_t *cond);

// Thread-local storage
I64 pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
I64 pthread_key_delete(pthread_key_t key);
void *pthread_getspecific(pthread_key_t key);
I64 pthread_setspecific(pthread_key_t key, void *value);

// Once initialization
I64 pthread_once(pthread_once_t *once, void (*init_routine)(void));
```

**Implementation strategy:**
1. Single-threaded stubs (all threads run sequentially)
2. Use ZealOS tasks for actual parallelism if needed
3. Mutexes become no-ops in single-threaded mode
4. TLS uses global array indexed by pthread_t

### B2: Memory Mapping (mmap)

**Files to create:**
```
/Home/POSIX/libc/mman.HH
/Home/POSIX/libc/mman.ZC
```

**Key functions:**
```c
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t length);
int mprotect(void *addr, size_t len, int prot);
int msync(void *addr, size_t length, int flags);
int mlock(void *addr, size_t len);
int munlock(void *addr, size_t len);
```

**Constants:**
```c
#define PROT_NONE   0x0
#define PROT_READ   0x1
#define PROT_WRITE  0x2
#define PROT_EXEC   0x4

#define MAP_SHARED    0x01
#define MAP_PRIVATE   0x02
#define MAP_FIXED     0x10
#define MAP_ANONYMOUS 0x20
#define MAP_ANON      MAP_ANONYMOUS

#define MAP_FAILED    ((void*)-1)
```

**Implementation strategy:**
1. MAP_ANONYMOUS: Use MAlloc for anonymous mappings
2. File mappings: Read file into allocated memory
3. Track mappings in a table for munmap
4. mprotect: No-op (ZealOS doesn't have memory protection)

### B3: Signals

**Files to create:**
```
/Home/POSIX/libc/signal.HH
/Home/POSIX/libc/signal.ZC
```

**Key types:**
```c
typedef void (*sighandler_t)(int);
typedef U64 sigset_t;

struct sigaction {
    sighandler_t sa_handler;
    sigset_t sa_mask;
    int sa_flags;
};
```

**Key functions:**
```c
sighandler_t signal(int signum, sighandler_t handler);
int sigaction(int signum, struct sigaction *act, struct sigaction *oldact);
int kill(pid_t pid, int sig);
int raise(int sig);
int sigprocmask(int how, sigset_t *set, sigset_t *oldset);
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
int sigismember(sigset_t *set, int signum);
int sigpending(sigset_t *set);
int sigsuspend(sigset_t *mask);
```

**Signal numbers:**
```c
#define SIGHUP    1
#define SIGINT    2
#define SIGQUIT   3
#define SIGILL    4
#define SIGTRAP   5
#define SIGABRT   6
#define SIGBUS    7
#define SIGFPE    8
#define SIGKILL   9
#define SIGUSR1   10
#define SIGSEGV   11
#define SIGUSR2   12
#define SIGPIPE   13
#define SIGALRM   14
#define SIGTERM   15
#define SIGCHLD   17
#define SIGCONT   18
#define SIGSTOP   19
#define SIGTSTP   20
```

**Implementation strategy:**
1. Maintain handler table for each signal
2. raise() directly calls handler
3. kill() only works for current process (pid 0 or getpid())
4. SIGKILL/SIGSTOP cannot be caught
5. Default handlers: ignore, terminate, or core dump

### B4: Additional libc Functions

**Time functions:**
```c
time_t time(time_t *tloc);
struct tm *localtime(time_t *timer);
struct tm *gmtime(time_t *timer);
time_t mktime(struct tm *tm);
size_t strftime(char *s, size_t max, char *format, struct tm *tm);
int gettimeofday(struct timeval *tv, struct timezone *tz);
int clock_gettime(clockid_t clk_id, struct timespec *tp);
```

**Process functions:**
```c
pid_t fork();      // Returns -1 (not supported) or stub
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
int execve(char *path, char **argv, char **envp);
int execvp(char *file, char **argv);
int system(char *command);  // Use sh_main
void _exit(int status);
```

**File descriptor functions:**
```c
int dup(int oldfd);
int dup2(int oldfd, int newfd);
int pipe(int pipefd[2]);
int fcntl(int fd, int cmd, ...);
int ioctl(int fd, unsigned long request, ...);
int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

**Network stubs:**
```c
int socket(int domain, int type, int protocol);
int bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int connect(int sockfd, struct sockaddr *addr, socklen_t addrlen);
ssize_t send(int sockfd, void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

---

## Phase B5: libm (Math Library)

**File to create:**
```
/Home/POSIX/libc/libm.ZC
```

**Basic functions (use ZealOS built-ins where available):**
```c
// Trigonometric
double sin(double x);
double cos(double x);
double tan(double x);
double asin(double x);
double acos(double x);
double atan(double x);
double atan2(double y, double x);

// Hyperbolic
double sinh(double x);
double cosh(double x);
double tanh(double x);
double asinh(double x);
double acosh(double x);
double atanh(double x);

// Exponential and logarithmic
double exp(double x);
double exp2(double x);
double expm1(double x);
double log(double x);
double log10(double x);
double log2(double x);
double log1p(double x);

// Power functions
double pow(double x, double y);
double sqrt(double x);
double cbrt(double x);
double hypot(double x, double y);

// Rounding
double ceil(double x);
double floor(double x);
double trunc(double x);
double round(double x);
double nearbyint(double x);
double rint(double x);

// Remainder
double fmod(double x, double y);
double remainder(double x, double y);
double remquo(double x, double y, int *quo);

// Manipulation
double copysign(double x, double y);
double nan(const char *tagp);
double nextafter(double x, double y);

// Min/max/difference
double fmin(double x, double y);
double fmax(double x, double y);
double fdim(double x, double y);

// Classification
int fpclassify(double x);
int isfinite(double x);
int isinf(double x);
int isnan(double x);
int isnormal(double x);
int signbit(double x);

// Error and gamma
double erf(double x);
double erfc(double x);
double lgamma(double x);
double tgamma(double x);

// Bessel (optional)
double j0(double x);
double j1(double x);
double jn(int n, double x);
double y0(double x);
double y1(double x);
double yn(int n, double x);
```

**Float versions (append 'f'):**
```c
float sinf(float x);
float cosf(float x);
// ... etc
```

**Long double versions (append 'l'):**
```c
long double sinl(long double x);
long double cosl(long double x);
// ... etc
```

---

## Phase C: LLVM/Clang Toolchain

### C1: LLVM ZealOS Target Backend

This requires creating a new LLVM target for ZealOS/x86-64.

**Key components:**

1. **Target Triple:** `x86_64-unknown-zealos`

2. **Target Registration:**
   - `ZealOSTargetMachine.cpp`
   - `ZealOSTargetInfo.cpp`
   - `ZealOSMCTargetDesc.cpp`

3. **ABI Definition:**
   - Calling convention (use x86-64 SysV or custom)
   - Data layout
   - Type sizes

4. **Code Generation:**
   - Instruction selection (reuse x86 backend)
   - Register allocation
   - Frame lowering

5. **Object Format:**
   - Custom ZealOS binary format or ELF subset
   - Symbol table format
   - Relocation types

**Files to create in LLVM source:**
```
llvm/lib/Target/X86/ZealOS/
  ZealOSTargetMachine.cpp
  ZealOSSubtarget.cpp
  ZealOSISelLowering.cpp
  ZealOSFrameLowering.cpp
  ZealOSMCTargetDesc.cpp

clang/lib/Driver/ToolChains/
  ZealOS.cpp
  ZealOS.h
```

### C2: Cross-Compile LLVM/Clang

Build LLVM on Linux targeting ZealOS:

```bash
# Clone LLVM
git clone https://github.com/llvm/llvm-project.git
cd llvm-project

# Apply ZealOS target patches
patch -p1 < zealos-target.patch

# Configure build
cmake -S llvm -B build \
  -DLLVM_ENABLE_PROJECTS="clang;lld" \
  -DLLVM_TARGETS_TO_BUILD="X86" \
  -DLLVM_DEFAULT_TARGET_TRIPLE="x86_64-unknown-zealos" \
  -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build -j$(nproc)

# Create ZealOS sysroot with headers and libs
mkdir -p sysroot/include sysroot/lib
cp -r /path/to/zealos/posix/headers sysroot/include/
```

### C3: Runtime Libraries

Need to provide:

1. **compiler-rt:** Low-level compiler support
   - Integer division/multiplication for large types
   - Floating-point emulation
   - Stack protector
   - Sanitizers (optional)

2. **libc++:** C++ standard library
   - Containers, algorithms, iterators
   - Streams, strings
   - Memory management
   - Threading (using our pthreads)

3. **libc++abi:** C++ ABI support
   - Exception handling
   - RTTI
   - Guard variables

---

## Phase D: Go Language Port

### D1: Port Go 1.4 (Last C Version)

Go 1.4 is the last version written in C, making it the bootstrap point.

**Prerequisites:**
- Working C compiler (from Phase C)
- Complete libc (from Phase B)
- make, bash equivalents (from Phase A)

**Steps:**

1. **Get Go 1.4 source:**
   ```bash
   wget https://go.dev/dl/go1.4-bootstrap-20171003.tar.gz
   tar xzf go1.4-bootstrap-20171003.tar.gz
   ```

2. **Patch for ZealOS:**
   - `src/cmd/dist/build.c` - Add zealos OS detection
   - `src/runtime/os_zealos.c` - OS-specific runtime
   - `src/syscall/zsyscall_zealos_amd64.go` - Syscall wrappers

3. **Key runtime components:**
   ```c
   // os_zealos.c
   void runtime·osinit(void);
   void runtime·goenvs(void);
   void runtime·minit(void);
   void runtime·unminit(void);
   void runtime·newosproc(M *mp, void *stk);
   void runtime·sigpanic(void);
   ```

4. **Build:**
   ```bash
   cd go/src
   GOOS=zealos GOARCH=amd64 ./make.bash
   ```

### D2: Bootstrap Modern Go

Once Go 1.4 works, use it to build newer Go versions:

```bash
# Build Go 1.17 (last version buildable with 1.4)
export GOROOT_BOOTSTRAP=/path/to/go1.4
cd go1.17/src
GOOS=zealos GOARCH=amd64 ./make.bash

# Build Go 1.20+ (requires Go 1.17+)
export GOROOT_BOOTSTRAP=/path/to/go1.17
cd go1.22/src
GOOS=zealos GOARCH=amd64 ./make.bash
```

**ZealOS-specific Go runtime:**

1. **Memory allocator:** Use ZealOS MAlloc or implement Go's allocator
2. **Scheduler:** Map goroutines to ZealOS tasks
3. **Garbage collector:** May need simplification for ZealOS
4. **System calls:** Map to POSIX shim layer
5. **Networking:** Use ZealOS network stack (if available)

---

## Estimated Effort

| Phase | Component | Lines of Code | Complexity |
|-------|-----------|---------------|------------|
| B1 | pthreads | ~800 | Medium |
| B2 | mmap | ~400 | Medium |
| B3 | signals | ~500 | Medium |
| B4 | libc misc | ~1500 | Medium |
| B5 | libm | ~2000 | Medium-High |
| C1 | LLVM target | ~3000 | Very High |
| C2 | Build system | ~500 | High |
| C3 | Runtime libs | ~5000+ | Very High |
| D1 | Go 1.4 port | ~2000 | High |
| D2 | Modern Go | ~1000 | Medium |

**Total: ~15,000+ lines of new code**

---

## Dependencies

```
Phase A (Complete)
    │
    ▼
Phase B1-B4 (libc) ──────┐
    │                    │
    ▼                    │
Phase B5 (libm)          │
    │                    │
    ├────────────────────┤
    ▼                    ▼
Phase C1-C2 (LLVM) ◄─────┘
    │
    ▼
Phase C3 (Runtime libs)
    │
    ▼
Phase D1 (Go 1.4)
    │
    ▼
Phase D2 (Modern Go)
```

---

## Testing Strategy

### Phase B Testing
```c
// pthread_test.c
pthread_t t;
pthread_create(&t, NULL, thread_func, NULL);
pthread_join(t, NULL);

// mmap_test.c
void *p = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
memset(p, 0x42, 4096);
munmap(p, 4096);

// signal_test.c
signal(SIGUSR1, handler);
raise(SIGUSR1);
```

### Phase C Testing
```bash
# Compile simple C program
clang --target=x86_64-unknown-zealos -c hello.c -o hello.o

# Link
ld.lld hello.o -o hello

# Run on ZealOS
./hello
```

### Phase D Testing
```go
// hello.go
package main

import "fmt"

func main() {
    fmt.Println("Hello from Go on ZealOS!")
}
```

```bash
GOOS=zealos GOARCH=amd64 go build hello.go
```
