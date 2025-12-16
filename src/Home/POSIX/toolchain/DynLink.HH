// DynLink.HH - Dynamic Linking Definitions for ZealOS
// Part of the POSIX compatibility layer
//
// Defines structures for dynamic linking support.

#ifndef _DYNLINK_HH
#define _DYNLINK_HH

// ============================================================================
// DYNAMIC LINKER CONFIGURATION
// ============================================================================

#define DL_MAX_LIBS         64      // Maximum loaded libraries
#define DL_MAX_PATH         256     // Maximum path length
#define DL_MAX_SEARCH_PATHS 16      // Maximum search paths
#define DL_HASH_SIZE        1024    // Symbol hash table size

// ============================================================================
// LIBRARY SEARCH PATHS
// ============================================================================

// Default library search paths (ZealOS equivalents)
// /lib:/usr/lib:/usr/local/lib mapped to ZealOS paths
#define DL_DEFAULT_PATH     "/Home/POSIX/lib:/Home/POSIX/usr/lib"

// ============================================================================
// DLOPEN FLAGS
// ============================================================================

#define RTLD_LAZY       0x00001     // Lazy function call binding
#define RTLD_NOW        0x00002     // Immediate function call binding
#define RTLD_BINDING_MASK 0x00003   // Mask for binding flags

#define RTLD_NOLOAD     0x00004     // Do not load the object
#define RTLD_DEEPBIND   0x00008     // Use deep binding
#define RTLD_GLOBAL     0x00100     // Make symbols available for relocation
#define RTLD_LOCAL      0x00000     // Opposite of GLOBAL (default)
#define RTLD_NODELETE   0x01000     // Do not delete object on close

// Special handles
#define RTLD_DEFAULT    ((void*)0)   // Default search order
#define RTLD_NEXT       ((void*)-1)  // Next symbol after current

// ============================================================================
// DYNAMIC SECTION ENTRY CACHE
// ============================================================================

class CDynEntry {
    U8  *strtab;            // DT_STRTAB - Dynamic string table
    I64 strsz;              // DT_STRSZ - String table size

    U8  *symtab;            // DT_SYMTAB - Dynamic symbol table
    I64 syment;             // DT_SYMENT - Symbol entry size

    U8  *hash;              // DT_HASH - Symbol hash table
    U8  *gnu_hash;          // DT_GNU_HASH - GNU hash table

    U8  *rela;              // DT_RELA - Rela relocations
    I64 relasz;             // DT_RELASZ - Size of Rela table
    I64 relaent;            // DT_RELAENT - Size of Rela entry

    U8  *rel;               // DT_REL - Rel relocations
    I64 relsz;              // DT_RELSZ - Size of Rel table
    I64 relent;             // DT_RELENT - Size of Rel entry

    U8  *jmprel;            // DT_JMPREL - PLT relocations
    I64 pltrelsz;           // DT_PLTRELSZ - Size of PLT relocs
    I64 pltrel;             // DT_PLTREL - Type (DT_REL or DT_RELA)

    U8  *pltgot;            // DT_PLTGOT - PLT GOT address

    U64 init;               // DT_INIT - Initialization function
    U64 fini;               // DT_FINI - Finalization function
    U64 init_array;         // DT_INIT_ARRAY
    I64 init_arraysz;       // DT_INIT_ARRAYSZ
    U64 fini_array;         // DT_FINI_ARRAY
    I64 fini_arraysz;       // DT_FINI_ARRAYSZ

    I64 flags;              // DT_FLAGS
    I64 flags_1;            // DT_FLAGS_1
};

// ============================================================================
// LOADED LIBRARY
// ============================================================================

class CDynLib {
    U8      *path;              // Full path to library
    U8      *soname;            // Library SONAME
    U8      *file_data;         // Raw file contents
    I64     file_size;          // File size

    // Memory mapping
    U8      *base_addr;         // Base load address
    I64     load_size;          // Total mapped size
    U64     min_vaddr;          // Minimum virtual address
    U64     max_vaddr;          // Maximum virtual address

    // ELF structures
    Elf64_Ehdr  *ehdr;          // ELF header
    Elf64_Phdr  *phdrs;         // Program headers
    Elf64_Dyn   *dynamic;       // Dynamic section
    I64         dyn_count;      // Dynamic entry count

    // Parsed dynamic section
    CDynEntry   dyn;            // Cached dynamic entries

    // Dependencies
    U8      *needed[DL_MAX_LIBS];   // DT_NEEDED library names
    I64     needed_count;

    // State
    I64     ref_count;          // Reference count
    I64     flags;              // RTLD_* flags
    Bool    initialized;        // Init functions called
    Bool    relocated;          // Relocations applied

    // Linkage
    CDynLib *next;              // Next in load order
    CDynLib *prev;              // Previous in load order
};

// ============================================================================
// GLOBAL SYMBOL TABLE ENTRY
// ============================================================================

class CDynSymbol {
    U8      *name;              // Symbol name
    U64     addr;               // Symbol address
    I64     size;               // Symbol size
    U8      type;               // Symbol type
    U8      bind;               // Symbol binding
    CDynLib *lib;               // Defining library
    CDynSymbol *next;           // Hash chain
};

// ============================================================================
// DYNAMIC LINKER STATE
// ============================================================================

class CDynLinker {
    // Loaded libraries
    CDynLib     *libs[DL_MAX_LIBS];
    I64         lib_count;

    // Load order list (head is first loaded)
    CDynLib     *load_head;
    CDynLib     *load_tail;

    // Global symbol table (hash table)
    CDynSymbol  *symtab[DL_HASH_SIZE];

    // Library search paths
    U8          *search_paths[DL_MAX_SEARCH_PATHS];
    I64         path_count;

    // Main executable (special case)
    CDynLib     *main_exe;

    // Error handling
    U8          error[256];
    Bool        has_error;

    // Debug/verbose mode
    Bool        verbose;
};

// ============================================================================
// DL_INFO STRUCTURE (for dladdr)
// ============================================================================

class Dl_info {
    U8      *dli_fname;     // Pathname of shared object
    U8      *dli_fbase;     // Base address of shared object
    U8      *dli_sname;     // Name of nearest symbol
    U8      *dli_saddr;     // Address of nearest symbol
};

// ============================================================================
// GNU HASH TABLE
// ============================================================================

#define DT_GNU_HASH     0x6FFFFEF5

class CELFGnuHash {
    U32     nbuckets;
    U32     symoffset;
    U32     bloom_size;
    U32     bloom_shift;
    // Followed by: U64 bloom[bloom_size]
    // Followed by: U32 buckets[nbuckets]
    // Followed by: U32 chains[]
};

#endif // _DYNLINK_HH
