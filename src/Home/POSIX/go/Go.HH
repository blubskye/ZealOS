// Go.HH - Go Language Runtime Support for ZealOS
// Part of the POSIX compatibility layer
//
// This provides the OS interface layer for running Go programs on ZealOS.
// Go requires specific syscalls and primitives that we map to ZealOS equivalents.

#ifndef _GO_HH
#define _GO_HH

// ============================================================================
// GO RUNTIME CONFIGURATION
// ============================================================================

#define GOOS            "zealos"
#define GOARCH          "amd64"
#define GOROOT          "/Home/POSIX/go"

// Runtime limits
#define GO_MAX_PROCS            1       // Single core
#define GO_STACK_MIN            2048    // Minimum goroutine stack (2KB)
#define GO_STACK_SYSTEM         8192    // System goroutine stack
#define GO_MAX_THREADS          256     // Maximum OS threads (M's)
#define GO_SCHED_QUANTUM        10000   // Scheduler quantum in microseconds

// Memory configuration
#define GO_HEAP_ARENA_SIZE      (64 * 1024 * 1024)  // 64MB heap arenas
#define GO_STACK_GUARD          928     // Stack guard size

// ============================================================================
// GO M-P-G MODEL STRUCTURES
// ============================================================================

// Forward declarations
class CGoG;     // Goroutine
class CGoM;     // Machine (OS thread)
class CGoP;     // Processor

// Goroutine status
#define GO_GIDLE        0   // Just allocated
#define GO_GRUNNABLE    1   // On run queue
#define GO_GRUNNING     2   // Running on M
#define GO_GSYSCALL     3   // In syscall
#define GO_GWAITING     4   // Blocked
#define GO_GDEAD        6   // Dead, can be reused
#define GO_GCOPYSTACK   8   // Stack being copied
#define GO_GPREEMPTED   9   // Preempted

// M (Machine/OS Thread) status
#define GO_MIDLE        0   // Idle
#define GO_MRUNNING     1   // Running
#define GO_MSYSCALL     2   // In syscall
#define GO_MSPINNING    3   // Looking for work

// P (Processor) status
#define GO_PIDLE        0   // Idle
#define GO_PRUNNING     1   // Running
#define GO_PSYSCALL     2   // M in syscall
#define GO_PGCSTOP      3   // Stopped for GC
#define GO_PDEAD        4   // No longer used

// ============================================================================
// GOROUTINE (G)
// ============================================================================

class CGoG {
    // Stack info
    void        *stack_lo;      // Stack low address
    void        *stack_hi;      // Stack high address
    void        *stack_guard;   // Stack guard for overflow check

    // Scheduling
    I64         goid;           // Goroutine ID
    I64         status;         // Goroutine status
    CGoM        *m;             // Current M (NULL if not running)
    CGoG        *sched_link;    // Next G in queue

    // Function to run
    void        *fn;            // Function pointer
    void        *fn_arg;        // Function argument

    // Parking
    I64         wait_reason;    // Why waiting
    void        *wait_lock;     // Lock waiting on
    I64         wait_trace_ev;  // Trace event

    // Context
    void        *ctxt;          // Closure context
    I64         panic_defer;    // Panic/defer state

    // Preemption
    Bool        preempt;        // Preemption signal
    Bool        preempt_stop;   // Stop preemption

    // Labels for debugging
    U8          *waiting_label;
    U8          *state_label;
};

// ============================================================================
// MACHINE/OS THREAD (M)
// ============================================================================

class CGoM {
    I64         id;             // M ID
    CGoG        *curg;          // Current goroutine
    CGoG        *g0;            // Scheduler goroutine (stack for runtime)
    CGoP        *p;             // Associated P (NULL if not running)
    CGoP        *nextp;         // P to associate with
    CGoP        *oldp;          // P before syscall

    // OS thread
    CTask       *task;          // ZealOS task
    I64         thread_id;      // OS thread ID

    // State
    I64         status;         // M status
    Bool        spinning;       // Looking for work
    Bool        blocked;        // Blocked in note

    // Syscall
    I64         syscall_pc;     // PC in syscall
    I64         syscall_sp;     // SP in syscall

    // Parking
    void        *park;          // Park/wake semaphore
    CGoM        *sched_link;    // Next M in queue

    // Signal handling
    void        *gsignal;       // Signal handling G
    void        *sigmask;       // Signal mask

    // TLS
    void        *tls[8];        // Thread-local storage
};

// ============================================================================
// PROCESSOR (P)
// ============================================================================

#define GO_RUN_QUEUE_SIZE   256

class CGoP {
    I64         id;             // P ID
    I64         status;         // P status
    CGoM        *m;             // Associated M

    // Run queue (local)
    CGoG        *run_queue[GO_RUN_QUEUE_SIZE];
    I64         run_queue_head;
    I64         run_queue_tail;
    I64         run_queue_len;

    // Free G's
    CGoG        *g_free;
    I64         g_free_count;

    // Scheduling
    CGoP        *link;          // Next P in list
    I64         schedtick;      // Incremented on schedule

    // Timers
    void        *timers;        // Timer heap

    // GC
    I64         gc_assists_done;
    Bool        gc_bg_mark_worker;
};

// ============================================================================
// SCHEDULER STATE
// ============================================================================

class CGoSched {
    // Global run queue
    CGoG        *run_queue_head;
    CGoG        *run_queue_tail;
    I64         run_queue_size;

    // Idle lists
    CGoM        *m_idle;
    I64         m_idle_count;
    CGoP        *p_idle;
    I64         p_idle_count;

    // All M's and P's
    CGoM        *all_m;
    CGoP        *all_p[GO_MAX_PROCS];
    I64         gomaxprocs;

    // Counts
    I64         m_count;        // Total M's
    I64         g_count;        // Total G's
    I64         last_poll;      // Last netpoll time

    // Locks
    void        *lock;          // Scheduler lock

    // Stopping
    Bool        gc_waiting;     // GC waiting for stop
    I64         stop_wait;      // Threads to stop
    void        *stop_note;     // Stop notification

    // IDs
    I64         next_goid;
    I64         next_mid;

    // Preemption
    I64         preempt_time;   // Next preempt check
};

// Global scheduler
extern CGoSched gosched;

// ============================================================================
// FUTEX EMULATION
// ============================================================================

// Futex operations
#define FUTEX_WAIT          0
#define FUTEX_WAKE          1
#define FUTEX_PRIVATE_FLAG  128

// Futex wait queue entry
class CFutexWaiter {
    CTask       *task;          // Waiting task
    U32         *addr;          // Futex address
    CFutexWaiter *next;         // Next waiter
};

// Futex functions
I64 futex(U32 *uaddr, I64 op, U32 val, void *timeout, U32 *uaddr2, U32 val3);
I64 futex_wait(U32 *addr, U32 val, I64 timeout_ns);
I64 futex_wake(U32 *addr, I64 count);

// ============================================================================
// NOTE/SEMAPHORE (Go's synchronization primitive)
// ============================================================================

// Note states
#define GO_NOTE_CLEARED     0
#define GO_NOTE_SIGNALED    1

class CGoNote {
    U32         key;            // State (0=cleared, 1=signaled)
};

U0 GoNoteInit(CGoNote *n);
U0 GoNoteClear(CGoNote *n);
U0 GoNoteWakeup(CGoNote *n);
Bool GoNoteSleep(CGoNote *n, I64 timeout_ns);  // Returns TRUE if signaled

// ============================================================================
// MEMORY ALLOCATION
// ============================================================================

// Memory stats
class CGoMemStats {
    U64         alloc;          // Bytes allocated and in use
    U64         total_alloc;    // Total bytes allocated
    U64         sys;            // Bytes from system
    U64         mallocs;        // Number of mallocs
    U64         frees;          // Number of frees
    U64         heap_alloc;     // Heap bytes allocated
    U64         heap_sys;       // Heap bytes from system
    U64         heap_idle;      // Heap bytes idle
    U64         heap_inuse;     // Heap bytes in use
    U64         heap_released;  // Heap bytes released
    U64         heap_objects;   // Number of heap objects
    U64         stack_inuse;    // Stack bytes in use
    U64         stack_sys;      // Stack bytes from system
    U64         gc_sys;         // GC metadata bytes
    U64         next_gc;        // Next GC target heap size
    U64         last_gc;        // Last GC time
    U64         pause_total_ns; // Total GC pause time
    U64         num_gc;         // Number of GCs
    Bool        enable_gc;      // GC enabled
};

extern CGoMemStats go_memstats;

// Memory functions
void *GoSysAlloc(I64 size);
U0 GoSysFree(void *addr, I64 size);
void *GoSysMmap(void *addr, I64 size, I64 prot, I64 flags);
U0 GoSysMunmap(void *addr, I64 size);

// ============================================================================
// THREAD CREATION
// ============================================================================

// Create new OS thread (M)
CGoM *GoNewM(CGoP *p, Bool spinning);

// Thread entry point
U0 GoMStart(CGoM *m);

// ============================================================================
// GO RUNTIME API
// ============================================================================

// Initialize runtime
U0 GoRuntimeInit();

// Create goroutine
CGoG *GoNewG(void *fn, void *arg, I64 stack_size);

// Schedule goroutine
U0 GoSchedule(CGoG *g);

// Run scheduler
U0 GoSchedRun();

// Yield current goroutine
U0 GoYield();

// Park current goroutine
U0 GoPark(void *lock, U8 *reason);

// Unpark goroutine
U0 GoReady(CGoG *g);

// Exit goroutine
U0 GoExit();

// Get current G
CGoG *GoGetG();

// Get current M
CGoM *GoGetM();

// ============================================================================
// SYSCALL SUPPORT
// ============================================================================

// Enter syscall mode
U0 GoEnterSyscall();

// Exit syscall mode
U0 GoExitSyscall();

// Syscall with potential block
I64 GoSyscall(I64 num, I64 a1, I64 a2, I64 a3);
I64 GoSyscall6(I64 num, I64 a1, I64 a2, I64 a3, I64 a4, I64 a5, I64 a6);

// ============================================================================
// SIGNAL HANDLING
// ============================================================================

// Signal handler type
typedef U0 (*GoSigHandler)(I64 sig, void *info, void *ctx);

// Set up signal handlers
U0 GoInitSig();

// Handle signal
U0 GoSigHandler(I64 sig);

// ============================================================================
// TIME
// ============================================================================

// Nanosecond time
I64 GoNanotime();

// Wallclock time
I64 GoWalltime(I64 *sec, I64 *nsec);

// Sleep
U0 GoSleep(I64 ns);

// ============================================================================
// ARCHITECTURE SPECIFIC
// ============================================================================

// TLS (Thread Local Storage)
U0 GoSetTLS(void *tls);
void *GoGetTLS();

// Stack bounds
U0 GoSetStackBounds(void *lo, void *hi);

// CPU features
extern Bool go_has_sse2;
extern Bool go_has_sse3;
extern Bool go_has_ssse3;
extern Bool go_has_sse41;
extern Bool go_has_sse42;
extern Bool go_has_avx;
extern Bool go_has_avx2;

#endif // _GO_HH
