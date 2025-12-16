// FUSE.HH - FUSE-like User-Space Filesystem Interface for ZealOS
// Part of the POSIX compatibility layer
//
// This provides a FUSE-compatible API for implementing filesystems
// in HolyC code. It allows user-space programs to provide filesystem
// implementations that integrate with the VFS layer.
//
// Design inspired by libfuse but adapted for ZealOS environment.

#ifndef _FUSE_HH
#define _FUSE_HH

#include "../VFS.HH"

// ============================================================================
// FUSE VERSION
// ============================================================================

#define FUSE_MAJOR_VERSION      2
#define FUSE_MINOR_VERSION      9
#define FUSE_VERSION            (FUSE_MAJOR_VERSION * 10 + FUSE_MINOR_VERSION)

// ============================================================================
// FUSE OPERATION RESULT CODES
// ============================================================================

#define FUSE_OK                 0

// ============================================================================
// FUSE FILE INFO
// ============================================================================

// Passed to all file operations
class CFuseFileInfo {
    I64     flags;              // Open flags
    U64     fh;                 // File handle (opaque, set by open)
    Bool    direct_io;          // Bypass page cache
    Bool    keep_cache;         // Don't invalidate cache
    Bool    flush;              // Flush on close
    Bool    nonseekable;        // Non-seekable file
    I64     lock_owner;         // Lock owner ID
};

// ============================================================================
// FUSE STAT
// ============================================================================

// File attributes (compatible with struct stat)
class CFuseStat {
    U64     st_ino;             // Inode number
    U32     st_mode;            // File type and mode
    U32     st_nlink;           // Number of hard links
    U32     st_uid;             // User ID
    U32     st_gid;             // Group ID
    U64     st_rdev;            // Device ID (if special file)
    I64     st_size;            // Total size in bytes
    I64     st_blksize;         // Block size for I/O
    I64     st_blocks;          // Number of 512B blocks
    I64     st_atime;           // Access time
    I64     st_mtime;           // Modification time
    I64     st_ctime;           // Change time
};

// ============================================================================
// FUSE DIRECTORY BUFFER
// ============================================================================

// Buffer for readdir
class CFuseDirBuf;

// Callback for adding directory entries
typedef I64 (*FuseFillDir)(CFuseDirBuf *buf, U8 *name, CFuseStat *st, I64 off);

// ============================================================================
// FUSE OPERATIONS STRUCTURE
// ============================================================================

// All filesystem operations - implement what you need, leave others NULL
class CFuseOps {
    // Get file attributes
    I64 (*getattr)(U8 *path, CFuseStat *st);

    // Read the target of a symbolic link
    I64 (*readlink)(U8 *path, U8 *buf, I64 size);

    // Create a file node (mknod)
    I64 (*mknod)(U8 *path, U32 mode, U64 rdev);

    // Create a directory
    I64 (*mkdir)(U8 *path, U32 mode);

    // Remove a file
    I64 (*unlink)(U8 *path);

    // Remove a directory
    I64 (*rmdir)(U8 *path);

    // Create a symbolic link
    I64 (*symlink)(U8 *target, U8 *linkpath);

    // Rename a file
    I64 (*rename)(U8 *from, U8 *to);

    // Create a hard link
    I64 (*link)(U8 *from, U8 *to);

    // Change file permissions
    I64 (*chmod)(U8 *path, U32 mode);

    // Change file ownership
    I64 (*chown)(U8 *path, U32 uid, U32 gid);

    // Change file size
    I64 (*truncate)(U8 *path, I64 size);

    // Open a file
    I64 (*open)(U8 *path, CFuseFileInfo *fi);

    // Read data from file
    I64 (*read)(U8 *path, U0 *buf, I64 size, I64 offset, CFuseFileInfo *fi);

    // Write data to file
    I64 (*write)(U8 *path, U0 *buf, I64 size, I64 offset, CFuseFileInfo *fi);

    // Get filesystem statistics
    I64 (*statfs)(U8 *path, CVFSStatfs *stbuf);

    // Flush cached data
    I64 (*flush)(U8 *path, CFuseFileInfo *fi);

    // Release an open file
    I64 (*release)(U8 *path, CFuseFileInfo *fi);

    // Synchronize file contents
    I64 (*fsync)(U8 *path, I64 isdatasync, CFuseFileInfo *fi);

    // Set extended attributes
    I64 (*setxattr)(U8 *path, U8 *name, U8 *value, I64 size, I64 flags);

    // Get extended attributes
    I64 (*getxattr)(U8 *path, U8 *name, U8 *value, I64 size);

    // List extended attributes
    I64 (*listxattr)(U8 *path, U8 *list, I64 size);

    // Remove extended attributes
    I64 (*removexattr)(U8 *path, U8 *name);

    // Open a directory
    I64 (*opendir)(U8 *path, CFuseFileInfo *fi);

    // Read directory
    I64 (*readdir)(U8 *path, U0 *buf, FuseFillDir filler,
                   I64 offset, CFuseFileInfo *fi);

    // Release a directory
    I64 (*releasedir)(U8 *path, CFuseFileInfo *fi);

    // Synchronize directory contents
    I64 (*fsyncdir)(U8 *path, I64 isdatasync, CFuseFileInfo *fi);

    // Initialize filesystem
    U0 *(*init)();

    // Clean up filesystem
    U0 (*destroy)(U0 *private_data);

    // Check file access permissions
    I64 (*access)(U8 *path, I64 mask);

    // Create and open a file
    I64 (*create)(U8 *path, U32 mode, CFuseFileInfo *fi);

    // Change the size of an open file
    I64 (*ftruncate)(U8 *path, I64 size, CFuseFileInfo *fi);

    // Get attributes from an open file
    I64 (*fgetattr)(U8 *path, CFuseStat *st, CFuseFileInfo *fi);

    // Change access/modification times
    I64 (*utimens)(U8 *path, I64 atime, I64 mtime);
};

// ============================================================================
// FUSE CONTEXT
// ============================================================================

// Context passed to filesystem implementation
class CFuseContext {
    U0      *private_data;      // Private data from init()
    U32     uid;                // User ID of calling process
    U32     gid;                // Group ID of calling process
    U32     pid;                // Process ID of calling process
};

// ============================================================================
// FUSE FILESYSTEM INSTANCE
// ============================================================================

class CFuseFS {
    U8              *mount_point;       // Where the filesystem is mounted
    CFuseOps        *ops;               // Filesystem operations
    U0              *private_data;      // User data from init()

    // VFS integration
    CVFSSuperblock  *vfs_sb;
    CVFSMount       *vfs_mount;

    // State
    Bool            mounted;
    I64             ref_count;
};

// ============================================================================
// FUSE API FUNCTIONS
// ============================================================================

// Create a new FUSE filesystem instance
CFuseFS *FuseNew(CFuseOps *ops, U0 *user_data);

// Mount the FUSE filesystem
I64 FuseMount(CFuseFS *fuse, U8 *mount_point);

// Unmount the FUSE filesystem
I64 FuseUnmount(CFuseFS *fuse);

// Get the current FUSE context
CFuseContext *FuseGetContext();

// Free a FUSE filesystem instance
U0 FuseFree(CFuseFS *fuse);

// ============================================================================
// VFS BRIDGE FUNCTIONS
// ============================================================================

// Internal functions for VFS integration
I64 FuseVFSMount(CVFSMount *mnt, U8 *device, U8 *options);
I64 FuseVFSUnmount(CVFSMount *mnt);
I64 FuseVFSLookup(CVFSInode *dir, U8 *name, CVFSInode **result);
I64 FuseVFSReadlink(CVFSInode *inode, U8 *buf, I64 size);
I64 FuseVFSOpen(CVFSFile *file);
I64 FuseVFSClose(CVFSFile *file);
I64 FuseVFSRead(CVFSFile *file, U0 *buf, I64 size, I64 offset);
I64 FuseVFSWrite(CVFSFile *file, U0 *buf, I64 size, I64 offset);
I64 FuseVFSReaddir(CVFSFile *file, U0 *buf, I64 count);
I64 FuseVFSCreate(CVFSInode *dir, U8 *name, U32 mode, CVFSInode **result);
I64 FuseVFSMkdir(CVFSInode *dir, U8 *name, U32 mode, CVFSInode **result);
I64 FuseVFSUnlink(CVFSInode *dir, U8 *name);
I64 FuseVFSRmdir(CVFSInode *dir, U8 *name);
I64 FuseVFSSymlink(CVFSInode *dir, U8 *name, U8 *target);
I64 FuseVFSTruncate(CVFSInode *inode, I64 size);

// ============================================================================
// HELPER MACROS
// ============================================================================

// Mode type macros (compatible with POSIX)
#define FUSE_S_IFMT     0170000
#define FUSE_S_IFSOCK   0140000
#define FUSE_S_IFLNK    0120000
#define FUSE_S_IFREG    0100000
#define FUSE_S_IFBLK    0060000
#define FUSE_S_IFDIR    0040000
#define FUSE_S_IFCHR    0020000
#define FUSE_S_IFIFO    0010000

#define FUSE_S_ISREG(m)  (((m) & FUSE_S_IFMT) == FUSE_S_IFREG)
#define FUSE_S_ISDIR(m)  (((m) & FUSE_S_IFMT) == FUSE_S_IFDIR)
#define FUSE_S_ISLNK(m)  (((m) & FUSE_S_IFMT) == FUSE_S_IFLNK)

// Registration
U0 FuseRegister();

#endif // _FUSE_HH
