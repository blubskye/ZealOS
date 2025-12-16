// FSTools.HH - Filesystem Tools Header for ZealOS
// Part of the POSIX compatibility layer
//
// Provides utilities for:
// - mkfs.*   - Format/create filesystems
// - fsck.*   - Check and repair filesystems
// - dumpe2fs - Dump ext2/3/4 filesystem info
// - ntfsinfo - Dump NTFS filesystem info

#ifndef _FSTOOLS_HH
#define _FSTOOLS_HH

#include "../VFS.HH"
#include "../ext4/Ext4.HH"
#include "../ntfs/NTFS.HH"

// ============================================================================
// COMMON DEFINITIONS
// ============================================================================

// Filesystem check flags
#define FSCK_READONLY       0x0001  // Don't modify filesystem
#define FSCK_VERBOSE        0x0002  // Verbose output
#define FSCK_FORCE          0x0004  // Force check even if clean
#define FSCK_AUTO_REPAIR    0x0008  // Automatically repair errors
#define FSCK_ASK_REPAIR     0x0010  // Ask before repairing

// Filesystem check results
#define FSCK_OK             0       // No errors found
#define FSCK_CORRECTED      1       // Errors corrected
#define FSCK_REBOOT         2       // System should be rebooted
#define FSCK_ERRORS         4       // Errors left uncorrected
#define FSCK_OPERATIONAL    8       // Operational error
#define FSCK_USAGE          16      // Usage or syntax error
#define FSCK_CANCELLED      32      // Cancelled by user

// mkfs options
#define MKFS_QUICK          0x0001  // Quick format (don't zero)
#define MKFS_VERBOSE        0x0002  // Verbose output
#define MKFS_FORCE          0x0004  // Force even if device has data

// ============================================================================
// MKFS STRUCTURES
// ============================================================================

// Ext4 mkfs options
class CMkfsExt4Opts {
    U32     block_size;         // Block size (1024, 2048, 4096)
    U32     inode_size;         // Inode size (128, 256, 512)
    U32     inode_ratio;        // Bytes per inode
    U32     reserved_percent;   // Reserved blocks percentage
    U8      *label;             // Volume label
    U8      *uuid;              // Volume UUID (NULL = generate)
    Bool    has_journal;        // Create journal (ext3/4)
    Bool    extent_tree;        // Use extents (ext4)
    Bool    dir_index;          // Directory hashing
    Bool    sparse_super;       // Sparse superblock
    U32     features_compat;    // Compatible features
    U32     features_incompat;  // Incompatible features
    U32     features_ro_compat; // Read-only compatible features
};

// FAT32 mkfs options
class CMkfsFat32Opts {
    U32     sectors_per_cluster;
    U8      *label;             // Volume label (11 chars max)
    U32     volume_id;          // Volume ID (0 = generate)
    Bool    quick;              // Quick format
};

// ============================================================================
// FSCK STRUCTURES
// ============================================================================

// Filesystem check context
class CFsckContext {
    I64     device_fd;          // Device file descriptor
    U8      *device_path;       // Device path
    U32     flags;              // Check flags

    // Statistics
    I64     errors_found;
    I64     errors_fixed;
    I64     files_checked;
    I64     dirs_checked;
    I64     blocks_checked;

    // Progress callback
    U0      (*progress)(CFsckContext *ctx, I64 percent, U8 *message);
    U0      *user_data;
};

// ============================================================================
// MKFS FUNCTIONS
// ============================================================================

// Create ext2/3/4 filesystem
I64 MkfsExt4(U8 *device, CMkfsExt4Opts *opts);
I64 MkfsExt3(U8 *device, CMkfsExt4Opts *opts);
I64 MkfsExt2(U8 *device, CMkfsExt4Opts *opts);

// Create FAT32 filesystem
I64 MkfsFat32(U8 *device, CMkfsFat32Opts *opts);

// Initialize default options
U0 MkfsExt4DefaultOpts(CMkfsExt4Opts *opts);
U0 MkfsFat32DefaultOpts(CMkfsFat32Opts *opts);

// ============================================================================
// FSCK FUNCTIONS
// ============================================================================

// Check ext2/3/4 filesystem
I64 FsckExt4(U8 *device, U32 flags);

// Check FAT32 filesystem
I64 FsckFat32(U8 *device, U32 flags);

// Check NTFS filesystem (read-only analysis)
I64 FsckNtfs(U8 *device, U32 flags);

// ============================================================================
// FILESYSTEM INFO FUNCTIONS
// ============================================================================

// Dump ext2/3/4 superblock info
I64 DumpExt4Info(U8 *device);

// Dump NTFS volume info
I64 DumpNtfsInfo(U8 *device);

// Dump FAT32 volume info
I64 DumpFat32Info(U8 *device);

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Generate UUID
U0 GenerateUUID(U8 *uuid);

// Format UUID as string
U8 *FormatUUID(U8 *uuid);

// Parse UUID from string
I64 ParseUUID(U8 *str, U8 *uuid);

// Get device size in bytes
I64 GetDeviceSize(U8 *device);

// Get device sector size
I64 GetDeviceSectorSize(U8 *device);

#endif // _FSTOOLS_HH
