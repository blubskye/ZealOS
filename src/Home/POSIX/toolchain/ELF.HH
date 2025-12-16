// ELF.HH - ELF Binary Format Definitions for ZealOS
// Part of the POSIX compatibility layer
//
// Defines ELF64 structures and constants for loading executable files.

#ifndef _ELF_HH
#define _ELF_HH

// ============================================================================
// ELF IDENTIFICATION
// ============================================================================

#define EI_NIDENT       16

#define EI_MAG0         0   // File identification
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3
#define EI_CLASS        4   // File class
#define EI_DATA         5   // Data encoding
#define EI_VERSION      6   // File version
#define EI_OSABI        7   // OS/ABI identification
#define EI_ABIVERSION   8   // ABI version
#define EI_PAD          9   // Start of padding bytes

// Magic number
#define ELFMAG0         0x7F
#define ELFMAG1         'E'
#define ELFMAG2         'L'
#define ELFMAG3         'F'
#define ELFMAG          "\x7FELF"

// File class
#define ELFCLASSNONE    0   // Invalid class
#define ELFCLASS32      1   // 32-bit objects
#define ELFCLASS64      2   // 64-bit objects

// Data encoding
#define ELFDATANONE     0   // Invalid data encoding
#define ELFDATA2LSB     1   // Little endian
#define ELFDATA2MSB     2   // Big endian

// OS/ABI identification
#define ELFOSABI_NONE       0   // No extensions or unspecified
#define ELFOSABI_SYSV       0   // UNIX System V ABI
#define ELFOSABI_LINUX      3   // Linux
#define ELFOSABI_FREEBSD    9   // FreeBSD
#define ELFOSABI_ZEALOS     254 // ZealOS (custom)

// ============================================================================
// ELF FILE TYPES
// ============================================================================

#define ET_NONE         0   // No file type
#define ET_REL          1   // Relocatable file
#define ET_EXEC         2   // Executable file
#define ET_DYN          3   // Shared object file
#define ET_CORE         4   // Core file

// ============================================================================
// MACHINE TYPES
// ============================================================================

#define EM_NONE         0   // No machine
#define EM_386          3   // Intel 80386
#define EM_X86_64       62  // AMD x86-64

// ============================================================================
// SPECIAL SECTION INDICES
// ============================================================================

#define SHN_UNDEF       0       // Undefined section
#define SHN_LORESERVE   0xFF00  // Start of reserved indices
#define SHN_COMMON      0xFFF2  // Common symbols
#define SHN_ABS         0xFFF1  // Absolute values
#define SHN_HIRESERVE   0xFFFF  // End of reserved indices

// ============================================================================
// SECTION TYPES
// ============================================================================

#define SHT_NULL        0   // Inactive
#define SHT_PROGBITS    1   // Program defined
#define SHT_SYMTAB      2   // Symbol table
#define SHT_STRTAB      3   // String table
#define SHT_RELA        4   // Relocation entries with addends
#define SHT_HASH        5   // Symbol hash table
#define SHT_DYNAMIC     6   // Dynamic linking
#define SHT_NOTE        7   // Notes
#define SHT_NOBITS      8   // No space (bss)
#define SHT_REL         9   // Relocation entries
#define SHT_SHLIB       10  // Reserved
#define SHT_DYNSYM      11  // Dynamic symbol table
#define SHT_INIT_ARRAY  14  // Initialization functions
#define SHT_FINI_ARRAY  15  // Termination functions

// ============================================================================
// SECTION FLAGS
// ============================================================================

#define SHF_WRITE       0x1         // Writable
#define SHF_ALLOC       0x2         // Occupies memory
#define SHF_EXECINSTR   0x4         // Executable
#define SHF_MERGE       0x10        // Might be merged
#define SHF_STRINGS     0x20        // Contains strings
#define SHF_INFO_LINK   0x40        // sh_info contains SHT index
#define SHF_LINK_ORDER  0x80        // Preserve order after combining
#define SHF_TLS         0x400       // Thread local storage

// ============================================================================
// PROGRAM HEADER TYPES
// ============================================================================

#define PT_NULL         0   // Unused
#define PT_LOAD         1   // Loadable segment
#define PT_DYNAMIC      2   // Dynamic linking info
#define PT_INTERP       3   // Interpreter path
#define PT_NOTE         4   // Auxiliary info
#define PT_SHLIB        5   // Reserved
#define PT_PHDR         6   // Program header table
#define PT_TLS          7   // Thread local storage
#define PT_GNU_EH_FRAME 0x6474E550  // Exception handling
#define PT_GNU_STACK    0x6474E551  // Stack flags
#define PT_GNU_RELRO    0x6474E552  // Read-only after relocation

// ============================================================================
// PROGRAM HEADER FLAGS
// ============================================================================

#define PF_X            0x1 // Execute
#define PF_W            0x2 // Write
#define PF_R            0x4 // Read

// ============================================================================
// SYMBOL BINDING
// ============================================================================

#define STB_LOCAL       0   // Local symbol
#define STB_GLOBAL      1   // Global symbol
#define STB_WEAK        2   // Weak symbol

// ============================================================================
// SYMBOL TYPES
// ============================================================================

#define STT_NOTYPE      0   // Unspecified
#define STT_OBJECT      1   // Data object
#define STT_FUNC        2   // Function
#define STT_SECTION     3   // Section
#define STT_FILE        4   // File name
#define STT_COMMON      5   // Common data
#define STT_TLS         6   // Thread local storage

// ============================================================================
// SYMBOL MACROS
// ============================================================================

#define ELF64_ST_BIND(i)    ((i) >> 4)
#define ELF64_ST_TYPE(i)    ((i) & 0xF)
#define ELF64_ST_INFO(b,t)  (((b) << 4) + ((t) & 0xF))

// ============================================================================
// RELOCATION TYPES (x86-64)
// ============================================================================

#define R_X86_64_NONE       0   // No relocation
#define R_X86_64_64         1   // Direct 64-bit
#define R_X86_64_PC32       2   // PC relative 32-bit signed
#define R_X86_64_GOT32      3   // 32-bit GOT entry
#define R_X86_64_PLT32      4   // 32-bit PLT address
#define R_X86_64_COPY       5   // Copy symbol at runtime
#define R_X86_64_GLOB_DAT   6   // Create GOT entry
#define R_X86_64_JUMP_SLOT  7   // Create PLT entry
#define R_X86_64_RELATIVE   8   // Adjust by program base
#define R_X86_64_GOTPCREL   9   // 32-bit signed PC relative to GOT
#define R_X86_64_32         10  // Direct 32-bit zero extended
#define R_X86_64_32S        11  // Direct 32-bit sign extended

#define ELF64_R_SYM(i)      ((i) >> 32)
#define ELF64_R_TYPE(i)     ((i) & 0xFFFFFFFF)
#define ELF64_R_INFO(s,t)   (((U64)(s) << 32) + (t))

// ============================================================================
// ELF64 HEADER
// ============================================================================

class Elf64_Ehdr {
    U8      e_ident[EI_NIDENT]; // Magic number and other info
    U16     e_type;             // Object file type
    U16     e_machine;          // Architecture
    U32     e_version;          // Object file version
    U64     e_entry;            // Entry point virtual address
    U64     e_phoff;            // Program header table file offset
    U64     e_shoff;            // Section header table file offset
    U32     e_flags;            // Processor-specific flags
    U16     e_ehsize;           // ELF header size
    U16     e_phentsize;        // Program header table entry size
    U16     e_phnum;            // Program header table entry count
    U16     e_shentsize;        // Section header table entry size
    U16     e_shnum;            // Section header table entry count
    U16     e_shstrndx;         // Section name string table index
};

// ============================================================================
// ELF64 SECTION HEADER
// ============================================================================

class Elf64_Shdr {
    U32     sh_name;        // Section name (string table index)
    U32     sh_type;        // Section type
    U64     sh_flags;       // Section flags
    U64     sh_addr;        // Section virtual address at execution
    U64     sh_offset;      // Section file offset
    U64     sh_size;        // Section size in bytes
    U32     sh_link;        // Link to another section
    U32     sh_info;        // Additional section info
    U64     sh_addralign;   // Section alignment
    U64     sh_entsize;     // Entry size if section holds table
};

// ============================================================================
// ELF64 PROGRAM HEADER
// ============================================================================

class Elf64_Phdr {
    U32     p_type;         // Segment type
    U32     p_flags;        // Segment flags
    U64     p_offset;       // Segment file offset
    U64     p_vaddr;        // Segment virtual address
    U64     p_paddr;        // Segment physical address
    U64     p_filesz;       // Segment size in file
    U64     p_memsz;        // Segment size in memory
    U64     p_align;        // Segment alignment
};

// ============================================================================
// ELF64 SYMBOL TABLE ENTRY
// ============================================================================

class Elf64_Sym {
    U32     st_name;        // Symbol name (string table index)
    U8      st_info;        // Symbol type and binding
    U8      st_other;       // Symbol visibility
    U16     st_shndx;       // Section index
    U64     st_value;       // Symbol value
    U64     st_size;        // Symbol size
};

// ============================================================================
// ELF64 RELOCATION ENTRIES
// ============================================================================

class Elf64_Rel {
    U64     r_offset;       // Address
    U64     r_info;         // Relocation type and symbol index
};

class Elf64_Rela {
    U64     r_offset;       // Address
    U64     r_info;         // Relocation type and symbol index
    I64     r_addend;       // Addend
};

// ============================================================================
// ELF64 DYNAMIC SECTION
// ============================================================================

class Elf64_Dyn {
    I64     d_tag;          // Dynamic entry type
    union {
        U64 d_val;          // Integer value
        U64 d_ptr;          // Address value
    } d_un;
};

// Dynamic tags
#define DT_NULL         0   // Marks end of dynamic section
#define DT_NEEDED       1   // Name of needed library
#define DT_PLTRELSZ     2   // Size of PLT relocs
#define DT_PLTGOT       3   // PLT/GOT
#define DT_HASH         4   // Symbol hash table
#define DT_STRTAB       5   // String table
#define DT_SYMTAB       6   // Symbol table
#define DT_RELA         7   // Rela relocs
#define DT_RELASZ       8   // Size of Rela relocs
#define DT_RELAENT      9   // Size of one Rela reloc
#define DT_STRSZ        10  // Size of string table
#define DT_SYMENT       11  // Size of symbol table entry
#define DT_INIT         12  // Init function
#define DT_FINI         13  // Fini function
#define DT_SONAME       14  // Shared object name
#define DT_RPATH        15  // Library search path
#define DT_SYMBOLIC     16  // Symbol search order
#define DT_REL          17  // Rel relocs
#define DT_RELSZ        18  // Size of Rel relocs
#define DT_RELENT       19  // Size of one Rel reloc
#define DT_PLTREL       20  // Type of reloc in PLT
#define DT_DEBUG        21  // Debug info
#define DT_TEXTREL      22  // Reloc might modify .text
#define DT_JMPREL       23  // PLT relocs
#define DT_BIND_NOW     24  // Process relocations at load
#define DT_INIT_ARRAY   25  // Array with init functions
#define DT_FINI_ARRAY   26  // Array with fini functions
#define DT_INIT_ARRAYSZ 27  // Size of DT_INIT_ARRAY
#define DT_FINI_ARRAYSZ 28  // Size of DT_FINI_ARRAY

#endif // _ELF_HH
