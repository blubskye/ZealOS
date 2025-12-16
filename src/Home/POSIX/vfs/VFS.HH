// VFS.HH - Virtual File System Abstraction Layer for ZealOS
// Part of the POSIX compatibility layer
//
// This provides a unified interface for multiple filesystem types,
// path resolution, symbolic links, and mount points.
//
// Design inspired by FreeBSD VFS (BSD-2-Clause license) and
// Linux VFS concepts. See LICENSE files in respective directories.

#ifndef _VFS_HH
#define _VFS_HH

// ============================================================================
// VFS CONFIGURATION
// ============================================================================

#define VFS_MAX_MOUNTS          32      // Maximum mount points
#define VFS_MAX_PATH            4096    // Maximum path length
#define VFS_MAX_NAME            255     // Maximum filename length
#define VFS_MAX_SYMLINK_DEPTH   40      // Maximum symlink resolution depth
#define VFS_INODE_HASH_SIZE     256     // Inode cache hash table size
#define VFS_DENTRY_CACHE_SIZE   1024    // Directory entry cache size

// ============================================================================
// INODE TYPES (compatible with POSIX mode_t)
// ============================================================================

#define VFS_IFREG       0100000     // Regular file
#define VFS_IFDIR       0040000     // Directory
#define VFS_IFLNK       0120000     // Symbolic link
#define VFS_IFBLK       0060000     // Block device
#define VFS_IFCHR       0020000     // Character device
#define VFS_IFIFO       0010000     // FIFO (named pipe)
#define VFS_IFSOCK      0140000     // Socket

#define VFS_IFMT        0170000     // File type mask

#define VFS_ISREG(m)    (((m) & VFS_IFMT) == VFS_IFREG)
#define VFS_ISDIR(m)    (((m) & VFS_IFMT) == VFS_IFDIR)
#define VFS_ISLNK(m)    (((m) & VFS_IFMT) == VFS_IFLNK)
#define VFS_ISBLK(m)    (((m) & VFS_IFMT) == VFS_IFBLK)
#define VFS_ISCHR(m)    (((m) & VFS_IFMT) == VFS_IFCHR)
#define VFS_ISFIFO(m)   (((m) & VFS_IFMT) == VFS_IFIFO)
#define VFS_ISSOCK(m)   (((m) & VFS_IFMT) == VFS_IFSOCK)

// ============================================================================
// VFS FLAGS
// ============================================================================

// Mount flags
#define VFS_MNT_RDONLY      0x00000001  // Read-only mount
#define VFS_MNT_NOSUID      0x00000002  // Ignore setuid bits
#define VFS_MNT_NODEV       0x00000004  // Disallow device access
#define VFS_MNT_NOEXEC      0x00000008  // Disallow execution
#define VFS_MNT_SYNCHRONOUS 0x00000010  // Synchronous writes
#define VFS_MNT_NOATIME     0x00000020  // Don't update access time

// Lookup flags
#define VFS_LOOKUP_FOLLOW   0x00000001  // Follow symlinks
#define VFS_LOOKUP_NOFOLLOW 0x00000000  // Don't follow final symlink
#define VFS_LOOKUP_PARENT   0x00000002  // Return parent directory
#define VFS_LOOKUP_CREATE   0x00000004  // Creating new entry

// Open flags (POSIX compatible)
#define VFS_O_RDONLY        0x0000
#define VFS_O_WRONLY        0x0001
#define VFS_O_RDWR          0x0002
#define VFS_O_ACCMODE       0x0003
#define VFS_O_CREAT         0x0040
#define VFS_O_EXCL          0x0080
#define VFS_O_TRUNC         0x0200
#define VFS_O_APPEND        0x0400
#define VFS_O_NONBLOCK      0x0800
#define VFS_O_DIRECTORY     0x10000
#define VFS_O_NOFOLLOW      0x20000
#define VFS_O_CLOEXEC       0x80000

// ============================================================================
// FILESYSTEM TYPE IDENTIFIERS
// ============================================================================

#define VFS_FS_UNKNOWN      0
#define VFS_FS_REDSEA       1       // ZealOS native
#define VFS_FS_FAT32        2       // FAT32/VFAT
#define VFS_FS_ISO9660      3       // CD/DVD
#define VFS_FS_NTFS         4       // NTFS (read-only)
#define VFS_FS_EXT2         5       // Linux ext2
#define VFS_FS_EXT3         6       // Linux ext3
#define VFS_FS_EXT4         7       // Linux ext4
#define VFS_FS_FUSE         8       // User-space filesystem
#define VFS_FS_TMPFS        9       // Memory filesystem
#define VFS_FS_DEVFS        10      // Device filesystem
#define VFS_FS_PROCFS       11      // Process filesystem
#define VFS_FS_SYSFS        12      // System filesystem

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================

class CVFSInode;
class CVFSDentry;
class CVFSMount;
class CVFSFile;
class CVFSSuperblock;
class CVFSFilesystem;

// ============================================================================
// FILESYSTEM OPERATIONS (per-filesystem type)
// ============================================================================

class CVFSOps {
    // Superblock operations
    I64     (*mount)(CVFSMount *mnt, U8 *device, U8 *options);
    I64     (*unmount)(CVFSMount *mnt);
    I64     (*sync)(CVFSSuperblock *sb);
    I64     (*statfs)(CVFSSuperblock *sb, U0 *buf);

    // Inode operations
    I64     (*lookup)(CVFSInode *dir, U8 *name, CVFSInode **result);
    I64     (*create)(CVFSInode *dir, U8 *name, U32 mode, CVFSInode **result);
    I64     (*mkdir)(CVFSInode *dir, U8 *name, U32 mode, CVFSInode **result);
    I64     (*rmdir)(CVFSInode *dir, U8 *name);
    I64     (*unlink)(CVFSInode *dir, U8 *name);
    I64     (*rename)(CVFSInode *old_dir, U8 *old_name,
                      CVFSInode *new_dir, U8 *new_name);
    I64     (*link)(CVFSInode *dir, U8 *name, CVFSInode *target);
    I64     (*symlink)(CVFSInode *dir, U8 *name, U8 *target);
    I64     (*readlink)(CVFSInode *inode, U8 *buf, I64 size);
    I64     (*getattr)(CVFSInode *inode);
    I64     (*setattr)(CVFSInode *inode, U0 *attr);
    I64     (*truncate)(CVFSInode *inode, I64 size);

    // File operations
    I64     (*open)(CVFSFile *file);
    I64     (*close)(CVFSFile *file);
    I64     (*read)(CVFSFile *file, U0 *buf, I64 size, I64 offset);
    I64     (*write)(CVFSFile *file, U0 *buf, I64 size, I64 offset);
    I64     (*readdir)(CVFSFile *file, U0 *buf, I64 count);
    I64     (*seek)(CVFSFile *file, I64 offset, I64 whence);
    I64     (*fsync)(CVFSFile *file);
    I64     (*ioctl)(CVFSFile *file, I64 cmd, U0 *arg);
    I64     (*mmap)(CVFSFile *file, U0 *addr, I64 len, I64 prot, I64 flags);
};

// ============================================================================
// SUPERBLOCK - Filesystem instance information
// ============================================================================

class CVFSSuperblock {
    U32             magic;              // Filesystem magic number
    U32             fs_type;            // VFS_FS_* type
    U32             flags;              // Mount flags
    U32             block_size;         // Block size in bytes
    U64             block_count;        // Total blocks
    U64             free_blocks;        // Free blocks
    U64             inode_count;        // Total inodes
    U64             free_inodes;        // Free inodes
    U64             max_file_size;      // Maximum file size
    U8              uuid[16];           // Filesystem UUID
    U8              label[64];          // Volume label

    CVFSInode       *root;              // Root inode
    CVFSMount       *mount;             // Mount point
    CVFSOps         *ops;               // Filesystem operations
    U0              *fs_private;        // Filesystem-specific data

    // Inode cache for this superblock
    CVFSInode       *inode_hash[VFS_INODE_HASH_SIZE];

    CVFSSuperblock  *next;              // Next superblock (list)
};

// ============================================================================
// INODE - File/directory metadata
// ============================================================================

class CVFSInode {
    U64             ino;                // Inode number
    U32             mode;               // File type and permissions
    U32             nlink;              // Hard link count
    U32             uid;                // Owner user ID
    U32             gid;                // Owner group ID
    U64             size;               // File size in bytes
    U64             blocks;             // Blocks allocated

    // Timestamps (UNIX epoch seconds)
    I64             atime;              // Access time
    I64             mtime;              // Modification time
    I64             ctime;              // Change time
    I64             btime;              // Birth/creation time

    // Device numbers (for device files)
    U32             rdev_major;
    U32             rdev_minor;

    // Symlink target (for symbolic links)
    U8              *symlink_target;

    // Relationships
    CVFSSuperblock  *sb;                // Superblock
    CVFSInode       *parent;            // Parent directory (NULL for root)
    CVFSDentry      *dentries;          // Directory entries (if directory)

    // Reference counting
    I64             ref_count;
    I64             open_count;

    // Filesystem-specific data
    U0              *fs_private;

    // Cache linkage
    CVFSInode       *hash_next;         // Hash chain
    CVFSInode       *lru_next;          // LRU list
    CVFSInode       *lru_prev;

    // Dirty flag
    Bool            dirty;
};

// ============================================================================
// DENTRY - Directory entry (name to inode mapping)
// ============================================================================

class CVFSDentry {
    U8              name[VFS_MAX_NAME + 1];  // Entry name
    U8              name_len;                // Name length

    CVFSInode       *inode;                 // Target inode
    CVFSInode       *parent;                // Parent directory inode

    // Cache linkage
    CVFSDentry      *next;                  // Next in directory
    CVFSDentry      *hash_next;             // Hash chain

    // Reference counting
    I64             ref_count;

    // Flags
    Bool            negative;               // Negative cache entry (not found)
};

// ============================================================================
// MOUNT - Mount point information
// ============================================================================

class CVFSMount {
    U8              *mount_point;           // Mount path (e.g., "/mnt/disk")
    U8              *device;                // Device path (e.g., "/dev/sda1")
    U8              *fs_type_name;          // Filesystem type name
    U32             fs_type;                // VFS_FS_* type
    U32             flags;                  // Mount flags

    CVFSSuperblock  *sb;                    // Superblock
    CVFSInode       *root;                  // Root inode of mounted fs
    CVFSInode       *mountpoint_inode;      // Inode where mounted

    // For submounts
    CVFSMount       *parent;                // Parent mount
    CVFSMount       *children;              // Child mounts
    CVFSMount       *sibling;               // Next sibling

    // Reference counting
    I64             ref_count;

    // Device info
    U0              *device_private;        // Device-specific data
    I64             device_fd;              // Device file descriptor

    CVFSMount       *next;                  // Global mount list
};

// ============================================================================
// FILE - Open file handle
// ============================================================================

class CVFSFile {
    CVFSInode       *inode;             // File inode
    CVFSMount       *mount;             // Mount point
    U32             flags;              // Open flags
    U32             mode;               // Open mode

    I64             offset;             // Current file position
    I64             ref_count;          // Reference count

    // Directory reading state
    I64             dir_pos;            // Directory position
    CVFSDentry      *dir_current;       // Current directory entry

    // Filesystem-specific data
    U0              *fs_private;
};

// ============================================================================
// FILESYSTEM TYPE REGISTRATION
// ============================================================================

class CVFSFilesystem {
    U8              *name;              // Filesystem name (e.g., "ext4")
    U32             type;               // VFS_FS_* type
    U32             flags;              // Capabilities

    CVFSOps         *ops;               // Operations

    // Probe function to identify filesystem
    I64             (*probe)(U8 *device, CVFSSuperblock *sb);

    CVFSFilesystem  *next;              // Next registered filesystem
};

// ============================================================================
// SYMBOLIC LINK TABLE (for filesystems that don't support symlinks natively)
// ============================================================================

#define VFS_SYMLINK_TABLE_SIZE  1024

class CVFSSymlink {
    U8              *path;              // Full path of symlink
    U8              *target;            // Target path
    U64             mtime;              // Creation/modification time
    CVFSSymlink     *next;              // Hash chain
};

// ============================================================================
// VFS GLOBAL STATE
// ============================================================================

class CVFS {
    // Mount table
    CVFSMount       *mounts;            // All mounts
    I64             mount_count;

    // Root filesystem
    CVFSMount       *root_mount;
    CVFSInode       *root_inode;

    // Registered filesystems
    CVFSFilesystem  *filesystems;

    // Symlink table (for overlay symlinks)
    CVFSSymlink     *symlink_table[VFS_SYMLINK_TABLE_SIZE];
    I64             symlink_count;

    // Dentry cache
    CVFSDentry      *dentry_cache[VFS_DENTRY_CACHE_SIZE];

    // Current working directory per process
    // (stored in CPosixProcess, not here)

    // Statistics
    I64             stat_lookups;
    I64             stat_cache_hits;
    I64             stat_reads;
    I64             stat_writes;

    // Lock (for thread safety - simplified in ZealOS)
    I64             lock;
};

// ============================================================================
// DIRECTORY ENTRY FOR READDIR
// ============================================================================

class CVFSDirent {
    U64             ino;                // Inode number
    U64             off;                // Offset to next entry
    U16             reclen;             // Record length
    U8              type;               // File type (DT_*)
    U8              name[VFS_MAX_NAME + 1];  // Filename
};

// Directory entry types (compatible with Linux)
#define DT_UNKNOWN      0
#define DT_FIFO         1
#define DT_CHR          2
#define DT_DIR          4
#define DT_BLK          6
#define DT_REG          8
#define DT_LNK          10
#define DT_SOCK         12
#define DT_WHT          14

// ============================================================================
// STATFS STRUCTURE
// ============================================================================

class CVFSStatfs {
    U64             f_type;             // Filesystem type
    U64             f_bsize;            // Block size
    U64             f_blocks;           // Total blocks
    U64             f_bfree;            // Free blocks
    U64             f_bavail;           // Available blocks (non-root)
    U64             f_files;            // Total inodes
    U64             f_ffree;            // Free inodes
    U64             f_fsid[2];          // Filesystem ID
    U64             f_namelen;          // Maximum name length
    U64             f_frsize;           // Fragment size
    U64             f_flags;            // Mount flags
};

#endif // _VFS_HH
