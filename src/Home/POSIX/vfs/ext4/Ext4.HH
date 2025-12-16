// Ext4.HH - Ext2/3/4 Filesystem Definitions for ZealOS
// Part of the POSIX compatibility layer
//
// On-disk structure definitions based on FreeBSD ext2fs implementation.
// Original FreeBSD code is BSD-2-Clause licensed.
// See LICENSE file in this directory.
//
// References:
// - FreeBSD sys/fs/ext2fs/ (BSD-2-Clause)
// - Linux Documentation/filesystems/ext4/
//
// This implementation supports:
// - Ext2/3/4 superblock parsing
// - Block group descriptors (32-bit and 64-bit)
// - Inode reading
// - Directory traversal
// - Extent-based block mapping (ext4)
// - Symbolic links (fast and regular)

#ifndef _EXT4_HH
#define _EXT4_HH

#include "../VFS.HH"

// ============================================================================
// MAGIC NUMBERS AND CONSTANTS
// ============================================================================

#define EXT2_SUPER_MAGIC        0xEF53
#define EXT4_EXTENT_MAGIC       0xF30A

// Superblock location
#define EXT2_SUPER_OFFSET       1024
#define EXT2_SUPER_SIZE         1024

// Special inodes
#define EXT2_BAD_INO            1       // Bad blocks inode
#define EXT2_ROOT_INO           2       // Root inode
#define EXT2_ACL_IDX_INO        3       // ACL index inode
#define EXT2_ACL_DATA_INO       4       // ACL data inode
#define EXT2_BOOT_LOADER_INO    5       // Boot loader inode
#define EXT2_UNDEL_DIR_INO      6       // Undelete directory inode
#define EXT2_RESIZE_INO         7       // Resize inode
#define EXT2_JOURNAL_INO        8       // Journal inode
#define EXT2_FIRST_INO          11      // First non-reserved inode

// Filesystem state
#define EXT2_VALID_FS           1       // Unmounted cleanly
#define EXT2_ERROR_FS           2       // Errors detected

// Filesystem revision levels
#define EXT2_GOOD_OLD_REV       0       // Original format
#define EXT2_DYNAMIC_REV        1       // V2 format with dynamic inode sizes

#define EXT2_GOOD_OLD_INODE_SIZE 128

// Block addressing constants
#define EXT2_NDIR_BLOCKS        12      // Direct blocks
#define EXT2_IND_BLOCK          12      // Single indirect block
#define EXT2_DIND_BLOCK         13      // Double indirect block
#define EXT2_TIND_BLOCK         14      // Triple indirect block
#define EXT2_N_BLOCKS           15      // Total blocks in inode

#define EXT2_MAX_SYMLINK_LEN    (EXT2_N_BLOCKS * 4)  // Max inline symlink

// Feature flags - Compatible
#define EXT2_FEATURE_COMPAT_DIR_PREALLOC    0x0001
#define EXT2_FEATURE_COMPAT_IMAGIC_INODES   0x0002
#define EXT3_FEATURE_COMPAT_HAS_JOURNAL     0x0004
#define EXT2_FEATURE_COMPAT_EXT_ATTR        0x0008
#define EXT2_FEATURE_COMPAT_RESIZE_INODE    0x0010
#define EXT2_FEATURE_COMPAT_DIR_INDEX       0x0020

// Feature flags - Read-only compatible
#define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER     0x0001
#define EXT2_FEATURE_RO_COMPAT_LARGE_FILE       0x0002
#define EXT2_FEATURE_RO_COMPAT_BTREE_DIR        0x0004
#define EXT4_FEATURE_RO_COMPAT_HUGE_FILE        0x0008
#define EXT4_FEATURE_RO_COMPAT_GDT_CSUM         0x0010
#define EXT4_FEATURE_RO_COMPAT_DIR_NLINK        0x0020
#define EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE      0x0040
#define EXT4_FEATURE_RO_COMPAT_METADATA_CSUM    0x0400

// Feature flags - Incompatible
#define EXT2_FEATURE_INCOMPAT_COMPRESSION   0x0001
#define EXT2_FEATURE_INCOMPAT_FILETYPE      0x0002
#define EXT3_FEATURE_INCOMPAT_RECOVER       0x0004
#define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV   0x0008
#define EXT2_FEATURE_INCOMPAT_META_BG       0x0010
#define EXT4_FEATURE_INCOMPAT_EXTENTS       0x0040
#define EXT4_FEATURE_INCOMPAT_64BIT         0x0080
#define EXT4_FEATURE_INCOMPAT_MMP           0x0100
#define EXT4_FEATURE_INCOMPAT_FLEX_BG       0x0200
#define EXT4_FEATURE_INCOMPAT_CSUM_SEED     0x2000

// Inode mode flags (same as POSIX)
#define EXT2_S_IFSOCK   0xC000  // Socket
#define EXT2_S_IFLNK    0xA000  // Symbolic link
#define EXT2_S_IFREG    0x8000  // Regular file
#define EXT2_S_IFBLK    0x6000  // Block device
#define EXT2_S_IFDIR    0x4000  // Directory
#define EXT2_S_IFCHR    0x2000  // Character device
#define EXT2_S_IFIFO    0x1000  // FIFO
#define EXT2_S_IFMT     0xF000  // Format mask

// Inode flags
#define EXT2_SECRM_FL           0x00000001  // Secure deletion
#define EXT2_UNRM_FL            0x00000002  // Undelete
#define EXT2_COMPR_FL           0x00000004  // Compressed file
#define EXT2_SYNC_FL            0x00000008  // Synchronous updates
#define EXT2_IMMUTABLE_FL       0x00000010  // Immutable file
#define EXT2_APPEND_FL          0x00000020  // Append only
#define EXT2_NODUMP_FL          0x00000040  // Do not dump
#define EXT2_NOATIME_FL         0x00000080  // Do not update atime
#define EXT4_INDEX_FL           0x00001000  // Hash-indexed directory
#define EXT4_EXTENTS_FL         0x00080000  // Uses extents

// Directory entry file types
#define EXT2_FT_UNKNOWN         0
#define EXT2_FT_REG_FILE        1
#define EXT2_FT_DIR             2
#define EXT2_FT_CHRDEV          3
#define EXT2_FT_BLKDEV          4
#define EXT2_FT_FIFO            5
#define EXT2_FT_SOCK            6
#define EXT2_FT_SYMLINK         7

// Directory entry alignment
#define EXT2_DIR_PAD            4
#define EXT2_DIR_ROUND          (EXT2_DIR_PAD - 1)
#define EXT2_DIR_REC_LEN(len)   (((len) + 8 + EXT2_DIR_ROUND) & ~EXT2_DIR_ROUND)

#define EXT2_NAME_LEN           255

// ============================================================================
// ON-DISK STRUCTURES
// ============================================================================

// Superblock (on-disk, at offset 1024)
class CExt2Superblock {
    U32 s_inodes_count;         // Total inodes
    U32 s_blocks_count;         // Total blocks (low 32 bits)
    U32 s_r_blocks_count;       // Reserved blocks count
    U32 s_free_blocks_count;    // Free blocks count (low 32 bits)
    U32 s_free_inodes_count;    // Free inodes count
    U32 s_first_data_block;     // First data block
    U32 s_log_block_size;       // Block size = 1024 << s_log_block_size
    U32 s_log_frag_size;        // Fragment size (obsolete)
    U32 s_blocks_per_group;     // Blocks per group
    U32 s_frags_per_group;      // Fragments per group (obsolete)
    U32 s_inodes_per_group;     // Inodes per group
    U32 s_mtime;                // Mount time
    U32 s_wtime;                // Write time
    U16 s_mnt_count;            // Mount count
    U16 s_max_mnt_count;        // Max mount count
    U16 s_magic;                // Magic signature (0xEF53)
    U16 s_state;                // Filesystem state
    U16 s_errors;               // Behavior on errors
    U16 s_minor_rev_level;      // Minor revision level
    U32 s_lastcheck;            // Time of last check
    U32 s_checkinterval;        // Max time between checks
    U32 s_creator_os;           // Creator OS
    U32 s_rev_level;            // Revision level
    U16 s_def_resuid;           // Default uid for reserved blocks
    U16 s_def_resgid;           // Default gid for reserved blocks

    // EXT2_DYNAMIC_REV superblock fields
    U32 s_first_ino;            // First non-reserved inode
    U16 s_inode_size;           // Inode structure size
    U16 s_block_group_nr;       // Block group number (of this superblock)
    U32 s_feature_compat;       // Compatible features
    U32 s_feature_incompat;     // Incompatible features
    U32 s_feature_ro_compat;    // Read-only compatible features
    U8  s_uuid[16];             // Filesystem UUID
    U8  s_volume_name[16];      // Volume name
    U8  s_last_mounted[64];     // Directory last mounted on
    U32 s_algo_bitmap;          // Compression algorithm usage

    // Performance hints
    U8  s_prealloc_blocks;      // Blocks to preallocate
    U8  s_prealloc_dir_blocks;  // Directory prealloc blocks
    U16 s_reserved_gdt_blocks;  // Online resize reserved GDT blocks

    // Ext3 journaling
    U8  s_journal_uuid[16];     // Journal superblock UUID
    U32 s_journal_inum;         // Journal inode number
    U32 s_journal_dev;          // Journal device number
    U32 s_last_orphan;          // Head of orphan inode list

    // Ext3 hash indexing
    U32 s_hash_seed[4];         // Hash seed
    U8  s_def_hash_version;     // Default hash version
    U8  s_jnl_backup_type;      // Journal backup type
    U16 s_desc_size;            // Group descriptor size

    // Ext4 fields
    U32 s_default_mount_opts;
    U32 s_first_meta_bg;        // First metablock group
    U32 s_mkfs_time;            // Filesystem creation time
    U32 s_jnl_blocks[17];       // Backup of journal inode

    // 64-bit support
    U32 s_blocks_count_hi;      // Blocks count (high 32 bits)
    U32 s_r_blocks_count_hi;    // Reserved blocks (high 32 bits)
    U32 s_free_blocks_count_hi; // Free blocks (high 32 bits)
    U16 s_min_extra_isize;      // Min extra inode size
    U16 s_want_extra_isize;     // Want extra inode size
    U32 s_flags;                // Miscellaneous flags
    U16 s_raid_stride;          // RAID stride
    U16 s_mmp_interval;         // MMP checking interval
    U64 s_mmp_block;            // MMP block location
    U32 s_raid_stripe_width;    // Blocks on all data disks
    U8  s_log_groups_per_flex;  // FLEX_BG group size
    U8  s_checksum_type;        // Metadata checksum algorithm
    U8  s_reserved_pad;
    U64 s_kbytes_written;       // Lifetime write count
    U32 s_snapshot_inum;        // Active snapshot inode
    U32 s_snapshot_id;          // Snapshot sequence ID
    U64 s_snapshot_r_blocks_count;
    U32 s_snapshot_list;        // Snapshot list head inode
    U32 s_error_count;          // Error count
    U32 s_first_error_time;     // First error time
    U32 s_first_error_ino;      // First error inode
    U64 s_first_error_block;    // First error block
    U8  s_first_error_func[32]; // First error function
    U32 s_first_error_line;     // First error line number
    U32 s_last_error_time;
    U32 s_last_error_ino;
    U32 s_last_error_line;
    U64 s_last_error_block;
    U8  s_last_error_func[32];
    U8  s_mount_opts[64];       // Mount options string
    U32 s_usr_quota_inum;       // User quota inode
    U32 s_grp_quota_inum;       // Group quota inode
    U32 s_overhead_blocks;      // Overhead blocks
    U32 s_backup_bgs[2];        // Groups with sparse_super2
    U8  s_encrypt_algos[4];     // Encryption algorithms
    U8  s_encrypt_pw_salt[16];  // Encryption key salt
    U32 s_lpf_ino;              // Lost+found inode
    U32 s_prj_quota_inum;       // Project quota inode
    U32 s_checksum_seed;        // Checksum seed
    U32 s_reserved[98];         // Padding
    U32 s_checksum;             // Superblock checksum
};

// Block group descriptor (32-bit)
class CExt2GroupDesc {
    U32 bg_block_bitmap;        // Block bitmap block
    U32 bg_inode_bitmap;        // Inode bitmap block
    U32 bg_inode_table;         // Inode table block
    U16 bg_free_blocks_count;   // Free blocks count
    U16 bg_free_inodes_count;   // Free inodes count
    U16 bg_used_dirs_count;     // Directory count
    U16 bg_flags;               // Block group flags
    U32 bg_exclude_bitmap_lo;   // Exclude bitmap location
    U16 bg_block_bitmap_csum_lo;
    U16 bg_inode_bitmap_csum_lo;
    U16 bg_itable_unused;       // Unused inodes count
    U16 bg_checksum;            // Group descriptor checksum
};

// Block group descriptor (64-bit extension)
class CExt4GroupDesc {
    U32 bg_block_bitmap;        // Block bitmap block (low)
    U32 bg_inode_bitmap;        // Inode bitmap block (low)
    U32 bg_inode_table;         // Inode table block (low)
    U16 bg_free_blocks_count;   // Free blocks count (low)
    U16 bg_free_inodes_count;   // Free inodes count (low)
    U16 bg_used_dirs_count;     // Directory count (low)
    U16 bg_flags;
    U32 bg_exclude_bitmap_lo;
    U16 bg_block_bitmap_csum_lo;
    U16 bg_inode_bitmap_csum_lo;
    U16 bg_itable_unused_lo;
    U16 bg_checksum;
    // 64-bit extensions
    U32 bg_block_bitmap_hi;     // Block bitmap block (high)
    U32 bg_inode_bitmap_hi;     // Inode bitmap block (high)
    U32 bg_inode_table_hi;      // Inode table block (high)
    U16 bg_free_blocks_count_hi;
    U16 bg_free_inodes_count_hi;
    U16 bg_used_dirs_count_hi;
    U16 bg_itable_unused_hi;
    U32 bg_exclude_bitmap_hi;
    U16 bg_block_bitmap_csum_hi;
    U16 bg_inode_bitmap_csum_hi;
    U32 bg_reserved;
};

// Inode (on-disk)
class CExt2Inode {
    U16 i_mode;                 // File mode
    U16 i_uid;                  // Owner UID (low 16 bits)
    U32 i_size;                 // File size (low 32 bits)
    U32 i_atime;                // Access time
    U32 i_ctime;                // Change time
    U32 i_mtime;                // Modification time
    U32 i_dtime;                // Deletion time
    U16 i_gid;                  // Owner GID (low 16 bits)
    U16 i_links_count;          // Hard link count
    U32 i_blocks;               // Block count (512-byte units or fs blocks)
    U32 i_flags;                // Inode flags
    U32 i_osd1;                 // OS-dependent 1
    U32 i_block[EXT2_N_BLOCKS]; // Block pointers / extent header
    U32 i_generation;           // File generation (for NFS)
    U32 i_file_acl;             // Extended attributes block (low)
    U32 i_size_high;            // File size (high 32 bits) / dir_acl
    U32 i_faddr;                // Fragment address (obsolete)
    // OS-dependent 2 (Linux):
    U16 i_blocks_high;          // Block count high bits
    U16 i_file_acl_high;        // EA block high bits
    U16 i_uid_high;             // UID high bits
    U16 i_gid_high;             // GID high bits
    U16 i_checksum_lo;          // Inode checksum (low)
    U16 i_reserved;
    // Extended inode fields (if s_inode_size > 128)
    U16 i_extra_isize;          // Extra inode size
    U16 i_checksum_hi;          // Inode checksum (high)
    U32 i_ctime_extra;          // Extra change time
    U32 i_mtime_extra;          // Extra modification time
    U32 i_atime_extra;          // Extra access time
    U32 i_crtime;               // Creation time
    U32 i_crtime_extra;         // Extra creation time
    U32 i_version_hi;           // High bits of inode version
    U32 i_projid;               // Project ID
};

// Directory entry (old format)
class CExt2DirEntry {
    U32 inode;                  // Inode number
    U16 rec_len;                // Record length
    U16 name_len;               // Name length
    U8  name[EXT2_NAME_LEN];    // File name
};

// Directory entry (new format with file type)
class CExt2DirEntry2 {
    U32 inode;                  // Inode number
    U16 rec_len;                // Record length
    U8  name_len;               // Name length
    U8  file_type;              // File type
    U8  name[EXT2_NAME_LEN];    // File name
};

// Extent header
class CExt4ExtentHeader {
    U16 eh_magic;               // Magic number (0xF30A)
    U16 eh_entries;             // Number of valid entries
    U16 eh_max;                 // Maximum entries
    U16 eh_depth;               // Depth of tree (0 = leaf)
    U32 eh_generation;          // Tree generation
};

// Extent (leaf node)
class CExt4Extent {
    U32 ee_block;               // First logical block
    U16 ee_len;                 // Number of blocks
    U16 ee_start_hi;            // High 16 bits of physical block
    U32 ee_start_lo;            // Low 32 bits of physical block
};

// Extent index (internal node)
class CExt4ExtentIdx {
    U32 ei_block;               // Logical block covered
    U32 ei_leaf_lo;             // Low 32 bits of child block
    U16 ei_leaf_hi;             // High 16 bits of child block
    U16 ei_unused;
};

// ============================================================================
// IN-MEMORY STRUCTURES
// ============================================================================

// Parsed superblock info
class CExt4FS {
    // Device access
    I64             device_fd;      // Device file descriptor
    U8              *device_path;   // Device path

    // Superblock info
    U32             block_size;     // Block size in bytes
    U32             log_block_size; // Log2(block_size) - 10
    U64             total_blocks;   // Total block count
    U64             free_blocks;    // Free block count
    U32             total_inodes;   // Total inode count
    U32             free_inodes;    // Free inode count
    U32             inodes_per_group;
    U32             blocks_per_group;
    U32             inode_size;     // Size of inode structure
    U32             first_ino;      // First non-reserved inode
    U32             group_count;    // Number of block groups
    U32             desc_size;      // Group descriptor size

    // Feature flags
    U32             features_compat;
    U32             features_incompat;
    U32             features_ro_compat;

    // Group descriptors
    U8              *group_desc_buf;    // Buffer for group descriptors
    I64             group_desc_count;

    // UUID and label
    U8              uuid[16];
    U8              volume_name[17];

    // State
    Bool            is_64bit;       // 64-bit block numbers
    Bool            has_extents;    // Extent-based files
    Bool            has_filetype;   // Directory entry has file type
    Bool            read_only;      // Mounted read-only

    // VFS integration
    CVFSSuperblock  *vfs_sb;
    CVFSMount       *vfs_mount;
};

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

// Superblock operations
I64 Ext4Mount(CVFSMount *mnt, U8 *device, U8 *options);
I64 Ext4Unmount(CVFSMount *mnt);
I64 Ext4Sync(CVFSSuperblock *sb);
I64 Ext4Statfs(CVFSSuperblock *sb, U0 *buf);

// Inode operations
I64 Ext4Lookup(CVFSInode *dir, U8 *name, CVFSInode **result);
I64 Ext4Readlink(CVFSInode *inode, U8 *buf, I64 size);
I64 Ext4Getattr(CVFSInode *inode);

// File operations
I64 Ext4Open(CVFSFile *file);
I64 Ext4Close(CVFSFile *file);
I64 Ext4Read(CVFSFile *file, U0 *buf, I64 size, I64 offset);
I64 Ext4Readdir(CVFSFile *file, U0 *buf, I64 count);

// Internal helpers
U64 Ext4GetBlockAddr(CExt4FS *fs, I64 group, I64 field);
I64 Ext4ReadBlock(CExt4FS *fs, U64 block, U0 *buf);
I64 Ext4ReadBlocks(CExt4FS *fs, U64 block, I64 count, U0 *buf);
CExt2Inode *Ext4ReadInode(CExt4FS *fs, U64 ino);
U64 Ext4InodeBlock(CExt4FS *fs, CExt2Inode *inode, U64 logical_block);
U64 Ext4ExtentBlock(CExt4FS *fs, CExt2Inode *inode, U64 logical_block);
U64 Ext4IndirectBlock(CExt4FS *fs, CExt2Inode *inode, U64 logical_block);

// Registration
U0 Ext4Register();

#endif // _EXT4_HH
