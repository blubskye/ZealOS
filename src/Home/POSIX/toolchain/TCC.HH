// TCC.HH - TinyCC Integration Header for ZealOS
// Part of the POSIX compatibility layer
//
// This provides the OS interface layer for running TCC on ZealOS.
// TCC is a small, fast, self-hosting C99 compiler.

#ifndef _TCC_HH
#define _TCC_HH

// ============================================================================
// TCC CONFIGURATION
// ============================================================================

// Target architecture
#define TCC_TARGET_X86_64       1
#define TCC_TARGET_PE           0   // Not Windows
#define TCC_TARGET_MACHO        0   // Not macOS

// Feature flags
#define TCC_IS_NATIVE           1   // Can run code directly
#define TCC_USING_DOUBLE_FLOAT  1   // 64-bit floats
#define TCC_BCHECK              0   // Bounds checking disabled
#define TCC_OUTPUT_MEMORY       1   // Can output to memory

// Paths
#define CONFIG_TCCDIR           "/Home/POSIX/toolchain/tcc"
#define CONFIG_TCC_SYSINCLUDEPATHS  "/Home/POSIX/libc:/Home/POSIX/include"
#define CONFIG_TCC_LIBPATHS     "/Home/POSIX/lib"
#define CONFIG_TCC_CRTPREFIX    "/Home/POSIX/lib"

// ============================================================================
// TCC STATE
// ============================================================================

// Output types
#define TCC_OUTPUT_MEMORY   1   // Output to memory (JIT)
#define TCC_OUTPUT_EXE      2   // Executable file
#define TCC_OUTPUT_DLL      3   // Dynamic library
#define TCC_OUTPUT_OBJ      4   // Object file
#define TCC_OUTPUT_PREPROCESS 5 // Preprocessor output only

// File types
#define TCC_FILETYPE_BINARY     1
#define TCC_FILETYPE_C          2
#define TCC_FILETYPE_ASM        3
#define TCC_FILETYPE_ASM_PP     4
#define TCC_FILETYPE_AR         5

// Options
#define TCC_OPTION_HELP         1
#define TCC_OPTION_I            2
#define TCC_OPTION_D            3
#define TCC_OPTION_U            4
#define TCC_OPTION_L            5
#define TCC_OPTION_B            6
#define TCC_OPTION_l            7
#define TCC_OPTION_bench        8
#define TCC_OPTION_bt           9
#define TCC_OPTION_c            10
#define TCC_OPTION_g            11
#define TCC_OPTION_static       12
#define TCC_OPTION_shared       13
#define TCC_OPTION_soname       14
#define TCC_OPTION_o            15
#define TCC_OPTION_r            16
#define TCC_OPTION_Wl           17
#define TCC_OPTION_W            18
#define TCC_OPTION_O            19
#define TCC_OPTION_m            20
#define TCC_OPTION_f            21
#define TCC_OPTION_nostdinc     22
#define TCC_OPTION_nostdlib     23
#define TCC_OPTION_print_search_dirs 24
#define TCC_OPTION_v            25
#define TCC_OPTION_w            26
#define TCC_OPTION_E            27
#define TCC_OPTION_M            28
#define TCC_OPTION_MD           29
#define TCC_OPTION_MF           30
#define TCC_OPTION_MT           31
#define TCC_OPTION_run          32
#define TCC_OPTION_x            33

// Error callback type
typedef U0 (*TCCErrorCallback)(void *opaque, U8 *msg);

// ============================================================================
// TCC COMPILER STATE
// ============================================================================

class CTCCState {
    // Output
    I64     output_type;        // TCC_OUTPUT_*
    I64     verbose;            // Verbosity level
    Bool    do_debug;           // Generate debug info
    Bool    do_bounds_check;    // Enable bounds checking

    // Paths
    U8      **include_paths;    // Include directories
    I64     include_count;
    U8      **sysinclude_paths; // System include directories
    I64     sysinclude_count;
    U8      **library_paths;    // Library directories
    I64     library_count;

    // Defines
    U8      **defines;          // -D defines
    I64     define_count;
    U8      **undefines;        // -U undefines
    I64     undef_count;

    // Input files
    U8      **files;            // Source files
    I64     *file_types;        // File types
    I64     file_count;

    // Libraries
    U8      **libraries;        // -l libraries
    I64     lib_count;

    // Output
    U8      *outfile;           // Output filename
    void    *code_mem;          // Memory for JIT code
    I64     code_size;          // Size of generated code

    // Error handling
    TCCErrorCallback error_func;
    void    *error_opaque;
    I64     error_count;
    I64     warning_count;

    // Options
    Bool    nostdinc;           // Don't use standard includes
    Bool    nostdlib;           // Don't link standard library
    Bool    static_link;        // Static linking
    Bool    rdynamic;           // Export all symbols
    I64     optimize_level;     // -O level

    // Internal state (opaque)
    void    *internal;
};

// ============================================================================
// TCC API FUNCTIONS
// ============================================================================

// Create/destroy compiler state
CTCCState *tcc_new();
U0 tcc_delete(CTCCState *s);

// Set error callback
U0 tcc_set_error_func(CTCCState *s, void *opaque, TCCErrorCallback func);

// Set options
I64 tcc_set_options(CTCCState *s, U8 *options);
I64 tcc_set_output_type(CTCCState *s, I64 output_type);

// Preprocessor
U0 tcc_define_symbol(CTCCState *s, U8 *sym, U8 *value);
U0 tcc_undefine_symbol(CTCCState *s, U8 *sym);

// Include/library paths
I64 tcc_add_include_path(CTCCState *s, U8 *pathname);
I64 tcc_add_sysinclude_path(CTCCState *s, U8 *pathname);
I64 tcc_add_library_path(CTCCState *s, U8 *pathname);
I64 tcc_add_library(CTCCState *s, U8 *libraryname);

// Compilation
I64 tcc_add_file(CTCCState *s, U8 *filename);
I64 tcc_compile_string(CTCCState *s, U8 *buf);

// Linking
I64 tcc_set_lib_path(CTCCState *s, U8 *path);
I64 tcc_add_symbol(CTCCState *s, U8 *name, void *val);
void *tcc_get_symbol(CTCCState *s, U8 *name);

// Output
I64 tcc_output_file(CTCCState *s, U8 *filename);
I64 tcc_relocate(CTCCState *s, void *ptr);
I64 tcc_run(CTCCState *s, I64 argc, U8 **argv);

// ============================================================================
// TCC MEMORY OUTPUT
// ============================================================================

// Special value for tcc_relocate to allocate memory
#define TCC_RELOCATE_AUTO   ((void*)1)

// Get code/data sections
void *tcc_get_code(CTCCState *s, I64 *size);

// ============================================================================
// TCC DRIVER
// ============================================================================

// Main TCC driver function
I64 tcc_main(I64 argc, U8 **argv);

// Simplified compile-and-run
I64 tcc_run_string(U8 *code, I64 argc, U8 **argv);

// Compile to memory and get function pointer
void *tcc_compile_func(U8 *code, U8 *func_name);

#endif // _TCC_HH
