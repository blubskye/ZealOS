// VM.HH - Virtual Memory Management for POSIX Processes
// Provides per-process virtual address spaces for proper fork() semantics

#ifndef _VM_HH
#define _VM_HH

// ============================================================================
// PAGE TABLE CONSTANTS
// ============================================================================

// x86-64 4-level paging structure
// PML4 (512 entries) -> PML3/PDPT (512 entries) -> PML2/PD (512 entries) -> PT (512 entries)
// Each entry is 8 bytes (64-bit)

#define PAGE_SIZE           0x1000          // 4KB pages for user space
#define PAGE_SIZE_2M        0x200000        // 2MB pages
#define PAGE_SIZE_1G        0x40000000      // 1GB pages

#define PAGE_SHIFT          12              // 4KB = 2^12
#define PAGE_SHIFT_2M       21              // 2MB = 2^21
#define PAGE_SHIFT_1G       30              // 1GB = 2^30

// Page table entry bits
#define PTE_PRESENT         0x001           // Page is present
#define PTE_WRITE           0x002           // Page is writable
#define PTE_USER            0x004           // Page accessible from ring 3
#define PTE_PWT             0x008           // Page write-through
#define PTE_PCD             0x010           // Page cache disable
#define PTE_ACCESSED        0x020           // Page was accessed
#define PTE_DIRTY           0x040           // Page was written
#define PTE_PS              0x080           // Page size (1=large page)
#define PTE_GLOBAL          0x100           // Global page (TLB not flushed)
#define PTE_COW             0x200           // Copy-on-write (available bit)
#define PTE_NX              (1ULL << 63)    // No execute

// Address masks
#define PTE_ADDR_MASK       0x000FFFFFFFFFF000ULL  // Physical address bits

// Page table index extraction
#define PML4_INDEX(va)      (((va) >> 39) & 0x1FF)
#define PML3_INDEX(va)      (((va) >> 30) & 0x1FF)
#define PML2_INDEX(va)      (((va) >> 21) & 0x1FF)
#define PT_INDEX(va)        (((va) >> 12) & 0x1FF)

// Virtual address limits for user space
#define USER_SPACE_START    0x0000000000400000ULL  // Start at 4MB
#define USER_SPACE_END      0x00007FFFFFFFFFFFULL  // End of canonical lower half
#define KERNEL_SPACE_START  0xFFFF800000000000ULL  // Start of canonical upper half

// ============================================================================
// MEMORY REGION TYPES
// ============================================================================

#define VM_REGION_FREE      0
#define VM_REGION_CODE      1   // Executable code (.text)
#define VM_REGION_DATA      2   // Initialized data (.data)
#define VM_REGION_BSS       3   // Uninitialized data (.bss)
#define VM_REGION_HEAP      4   // Heap (grows up)
#define VM_REGION_STACK     5   // Stack (grows down)
#define VM_REGION_MMAP      6   // Memory mapped regions
#define VM_REGION_SHARED    7   // Shared memory

// Region flags
#define VM_FLAG_READ        0x01
#define VM_FLAG_WRITE       0x02
#define VM_FLAG_EXEC        0x04
#define VM_FLAG_SHARED      0x08
#define VM_FLAG_GROWSDOWN   0x10    // Stack-like region
#define VM_FLAG_LOCKED      0x20    // Cannot be swapped
#define VM_FLAG_COW         0x40    // Copy-on-write

// ============================================================================
// VIRTUAL MEMORY STRUCTURES
// ============================================================================

// Memory region descriptor
class CVMRegion {
    U64             start;          // Start virtual address
    U64             end;            // End virtual address (exclusive)
    U8              type;           // Region type (VM_REGION_*)
    U8              flags;          // Region flags (VM_FLAG_*)
    U8              pad[6];
    CVMRegion       *next;          // Next region in list
    CVMRegion       *prev;          // Previous region
    U8              *name;          // Optional name for debugging
};

// Physical page tracking for COW
class CPhysPage {
    U64             phys_addr;      // Physical address
    I64             ref_count;      // Reference count for COW
    CPhysPage       *next;          // Hash chain
};

// Page table entry (for manipulation)
class CPTE {
    U64             entry;          // Raw page table entry
};

// Virtual memory context for a process
class CVMContext {
    U64             cr3;            // Page table root (PML4 physical address)
    U64             *pml4_virt;     // Virtual address of PML4 for manipulation

    // Memory regions
    CVMRegion       *regions;       // Linked list of memory regions
    I64             region_count;   // Number of regions

    // Heap management
    U64             heap_start;     // Start of heap
    U64             heap_end;       // Current heap end (brk)
    U64             heap_max;       // Maximum heap address

    // Stack
    U64             stack_top;      // Top of stack (highest address)
    U64             stack_bottom;   // Current stack bottom
    U64             stack_max;      // Maximum stack size

    // Statistics
    I64             total_pages;    // Total pages allocated
    I64             cow_pages;      // Pages marked COW
    I64             page_faults;    // Page fault count

    // Reference counting
    I64             ref_count;      // Reference count (for shared VMs)

    // Parent for COW
    CVMContext      *parent;        // Parent context (for COW tracking)
};

// Page frame allocator
class CPageAllocator {
    U64             free_list;      // Head of free page list
    I64             free_count;     // Number of free pages
    I64             total_pages;    // Total pages managed
    U8              *bitmap;        // Page allocation bitmap
    I64             bitmap_size;    // Size of bitmap in bytes
    U64             base_addr;      // Base physical address
};

// ============================================================================
// GLOBAL STATE
// ============================================================================

// The kernel's page tables (identity mapped, shared by all)
extern U64 *vm_kernel_pml4;

// Global page allocator for user pages
extern CPageAllocator *vm_page_allocator;

// Hash table for physical page reference counting
#define PHYS_PAGE_HASH_SIZE 1024
extern CPhysPage *vm_phys_page_hash[PHYS_PAGE_HASH_SIZE];

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

// Initialization
U0 VMInit();
U0 VMPageAllocatorInit(U64 base, U64 size);

// Page allocation
U64 VMAllocPage();
U0 VMFreePage(U64 phys_addr);
U64 VMAllocPages(I64 count);
U0 VMFreePages(U64 phys_addr, I64 count);

// Physical page reference counting
I64 VMPhysPageRef(U64 phys_addr);
I64 VMPhysPageUnref(U64 phys_addr);
I64 VMPhysPageGetRef(U64 phys_addr);

// Virtual memory context
CVMContext *VMContextNew();
U0 VMContextFree(CVMContext *ctx);
CVMContext *VMContextFork(CVMContext *parent);

// Page table manipulation
U64 *VMGetPTE(CVMContext *ctx, U64 virt_addr, Bool create);
U0 VMMapPage(CVMContext *ctx, U64 virt_addr, U64 phys_addr, U64 flags);
U0 VMUnmapPage(CVMContext *ctx, U64 virt_addr);
U64 VMTranslate(CVMContext *ctx, U64 virt_addr);

// Memory regions
CVMRegion *VMRegionAdd(CVMContext *ctx, U64 start, U64 end, U8 type, U8 flags);
U0 VMRegionRemove(CVMContext *ctx, CVMRegion *region);
CVMRegion *VMRegionFind(CVMContext *ctx, U64 addr);

// Memory mapping
U64 VMMmap(CVMContext *ctx, U64 addr, U64 length, U8 flags);
U0 VMMunmap(CVMContext *ctx, U64 addr, U64 length);

// Heap management (brk/sbrk)
U64 VMBrk(CVMContext *ctx, U64 new_brk);

// Stack management
U64 VMStackGrow(CVMContext *ctx, U64 fault_addr);

// Page fault handling
I64 VMPageFault(CVMContext *ctx, U64 fault_addr, U64 error_code);
I64 VMHandleCOW(CVMContext *ctx, U64 fault_addr);

// CR3 switching
U0 VMSwitchContext(CVMContext *ctx);
U0 VMFlushTLB();
U0 VMFlushTLBPage(U64 virt_addr);

// Utility
U0 VMCopyPage(U64 dst_phys, U64 src_phys);
U0 VMZeroPage(U64 phys_addr);

// Debug
U0 VMDumpContext(CVMContext *ctx);
U0 VMDumpPageTables(CVMContext *ctx);

#endif // _VM_HH
