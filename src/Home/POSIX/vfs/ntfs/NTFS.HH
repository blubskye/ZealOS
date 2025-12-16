// NTFS.HH - NTFS Filesystem Definitions for ZealOS
// Part of the POSIX compatibility layer
//
// Clean-room implementation based on publicly available NTFS documentation.
// This is a read-only driver for safety.
//
// References:
// - Microsoft NTFS Technical Reference
// - https://docs.microsoft.com/en-us/windows/win32/fileio/master-file-table
// - Public NTFS documentation and specifications

#ifndef _NTFS_HH
#define _NTFS_HH

#include "../VFS.HH"

// ============================================================================
// CONSTANTS
// ============================================================================

// Boot sector signature
#define NTFS_BOOT_MAGIC         0xAA55
#define NTFS_OEM_ID             "NTFS    "

// MFT record signatures
#define NTFS_FILE_SIGNATURE     0x454C4946  // 'FILE'
#define NTFS_INDX_SIGNATURE     0x58444E49  // 'INDX'
#define NTFS_BAAD_SIGNATURE     0x44414142  // 'BAAD'

// Special MFT records
#define NTFS_MFT_MFT            0   // $MFT - Master File Table
#define NTFS_MFT_MFTMIRR        1   // $MFTMirr
#define NTFS_MFT_LOGFILE        2   // $LogFile
#define NTFS_MFT_VOLUME         3   // $Volume
#define NTFS_MFT_ATTRDEF        4   // $AttrDef
#define NTFS_MFT_ROOT           5   // Root directory (.)
#define NTFS_MFT_BITMAP         6   // $Bitmap
#define NTFS_MFT_BOOT           7   // $Boot
#define NTFS_MFT_BADCLUS        8   // $BadClus
#define NTFS_MFT_SECURE         9   // $Secure
#define NTFS_MFT_UPCASE         10  // $UpCase
#define NTFS_MFT_EXTEND         11  // $Extend
#define NTFS_MFT_FIRST_USER     24  // First user file record

// Attribute types
#define NTFS_AT_STANDARD_INFO       0x10
#define NTFS_AT_ATTRIBUTE_LIST      0x20
#define NTFS_AT_FILENAME            0x30
#define NTFS_AT_OBJECT_ID           0x40
#define NTFS_AT_SECURITY_DESC       0x50
#define NTFS_AT_VOLUME_NAME         0x60
#define NTFS_AT_VOLUME_INFO         0x70
#define NTFS_AT_DATA                0x80
#define NTFS_AT_INDEX_ROOT          0x90
#define NTFS_AT_INDEX_ALLOCATION    0xA0
#define NTFS_AT_BITMAP              0xB0
#define NTFS_AT_REPARSE_POINT       0xC0
#define NTFS_AT_EA_INFO             0xD0
#define NTFS_AT_EA                  0xE0
#define NTFS_AT_END                 0xFFFFFFFF

// File record flags
#define NTFS_FILE_RECORD_IN_USE     0x0001
#define NTFS_FILE_RECORD_IS_DIR     0x0002

// Attribute flags
#define NTFS_ATTR_COMPRESSED        0x0001
#define NTFS_ATTR_ENCRYPTED         0x4000
#define NTFS_ATTR_SPARSE            0x8000

// Filename namespace types
#define NTFS_FILENAME_POSIX         0
#define NTFS_FILENAME_WIN32         1
#define NTFS_FILENAME_DOS           2
#define NTFS_FILENAME_WIN32_DOS     3

// Index entry flags
#define NTFS_INDEX_ENTRY_SUBNODE    0x0001
#define NTFS_INDEX_ENTRY_LAST       0x0002

// File attributes (same as Windows)
#define NTFS_FILE_ATTR_READONLY     0x00000001
#define NTFS_FILE_ATTR_HIDDEN       0x00000002
#define NTFS_FILE_ATTR_SYSTEM       0x00000004
#define NTFS_FILE_ATTR_DIRECTORY    0x10000000
#define NTFS_FILE_ATTR_ARCHIVE      0x00000020
#define NTFS_FILE_ATTR_DEVICE       0x00000040
#define NTFS_FILE_ATTR_NORMAL       0x00000080
#define NTFS_FILE_ATTR_TEMPORARY    0x00000100
#define NTFS_FILE_ATTR_SPARSE_FILE  0x00000200
#define NTFS_FILE_ATTR_REPARSE      0x00000400
#define NTFS_FILE_ATTR_COMPRESSED   0x00000800
#define NTFS_FILE_ATTR_ENCRYPTED    0x00004000

#define NTFS_NAME_LEN               255

// ============================================================================
// ON-DISK STRUCTURES
// ============================================================================

// NTFS Boot Sector (first sector of volume)
class CNTFSBootSector {
    U8  jump[3];            // 0x00: Jump instruction
    U8  oem_id[8];          // 0x03: "NTFS    "
    U16 bytes_per_sector;   // 0x0B: Usually 512
    U8  sectors_per_cluster;// 0x0D: Cluster size / sector size
    U8  reserved1[7];       // 0x0E: Reserved
    U8  media_type;         // 0x15: 0xF8 for hard disk
    U8  reserved2[2];       // 0x16: Reserved
    U16 sectors_per_track;  // 0x18: Sectors per track
    U16 heads;              // 0x1A: Number of heads
    U32 hidden_sectors;     // 0x1C: Hidden sectors
    U8  reserved3[4];       // 0x20: Reserved
    U8  reserved4[4];       // 0x24: Usually 0x80008000
    U64 total_sectors;      // 0x28: Total sectors on volume
    U64 mft_lcn;            // 0x30: LCN of $MFT
    U64 mft_mirr_lcn;       // 0x38: LCN of $MFTMirr
    I8  clusters_per_mft;   // 0x40: Clusters per MFT record (negative = 2^|val|)
    U8  reserved5[3];       // 0x41: Reserved
    I8  clusters_per_index; // 0x44: Clusters per index record
    U8  reserved6[3];       // 0x45: Reserved
    U64 volume_serial;      // 0x48: Volume serial number
    U32 checksum;           // 0x50: Checksum (unused)
    U8  bootstrap[426];     // 0x54: Bootstrap code
    U16 end_marker;         // 0x1FE: 0xAA55
};

// MFT Record Header
class CNTFSRecordHeader {
    U32 magic;              // 0x00: 'FILE' or 'INDX'
    U16 update_seq_offset;  // 0x04: Offset to update sequence
    U16 update_seq_count;   // 0x06: Size of update sequence
    U64 lsn;                // 0x08: Log file sequence number
    U16 sequence_number;    // 0x10: Record sequence number
    U16 hard_link_count;    // 0x12: Hard link count
    U16 attrs_offset;       // 0x14: Offset to first attribute
    U16 flags;              // 0x16: Record flags
    U32 bytes_used;         // 0x18: Real size of record
    U32 bytes_allocated;    // 0x1C: Allocated size
    U64 base_record;        // 0x20: Base MFT record (0 if base)
    U16 next_attr_id;       // 0x28: Next attribute ID
    U16 reserved;           // 0x2A: Align to 4 bytes (NTFS 3.0+)
    U32 record_number;      // 0x2C: This MFT record number (NTFS 3.0+)
};

// Attribute Header (common part)
class CNTFSAttrHeader {
    U32 type;               // 0x00: Attribute type
    U32 length;             // 0x04: Attribute length (including header)
    U8  non_resident;       // 0x08: 0 = resident, 1 = non-resident
    U8  name_length;        // 0x09: Attribute name length (in chars)
    U16 name_offset;        // 0x0A: Offset to attribute name
    U16 flags;              // 0x0C: Attribute flags
    U16 attr_id;            // 0x0E: Attribute ID
};

// Resident Attribute Header (follows CNTFSAttrHeader)
class CNTFSResidentAttr {
    U32 value_length;       // 0x10: Length of attribute value
    U16 value_offset;       // 0x14: Offset to attribute value
    U8  indexed;            // 0x16: Indexed flag
    U8  padding;            // 0x17: Padding
};

// Non-Resident Attribute Header (follows CNTFSAttrHeader)
class CNTFSNonResidentAttr {
    U64 start_vcn;          // 0x10: Starting VCN
    U64 end_vcn;            // 0x18: Ending VCN
    U16 runlist_offset;     // 0x20: Offset to run list
    U16 compression_unit;   // 0x22: Compression unit size
    U32 padding;            // 0x24: Padding
    U64 alloc_size;         // 0x28: Allocated size
    U64 data_size;          // 0x30: Real data size
    U64 init_size;          // 0x38: Initialized data size
    // For compressed: U64 compressed_size;
};

// Standard Information Attribute (0x10)
class CNTFSStdInfo {
    U64 creation_time;      // 0x00: File creation time
    U64 modification_time;  // 0x08: File modification time
    U64 mft_change_time;    // 0x10: MFT change time
    U64 access_time;        // 0x18: Last access time
    U32 file_attrs;         // 0x20: File attributes
    U32 max_versions;       // 0x24: Maximum versions (unused)
    U32 version_number;     // 0x28: Version number (unused)
    U32 class_id;           // 0x2C: Class ID (unused)
    // NTFS 3.0+ additional fields:
    U32 owner_id;           // 0x30: Owner ID
    U32 security_id;        // 0x34: Security ID
    U64 quota_charged;      // 0x38: Quota charged
    U64 usn;                // 0x40: USN
};

// Filename Attribute (0x30)
class CNTFSFilename {
    U64 parent_ref;         // 0x00: Parent directory MFT reference
    U64 creation_time;      // 0x08: File creation time
    U64 modification_time;  // 0x10: File modification time
    U64 mft_change_time;    // 0x18: MFT change time
    U64 access_time;        // 0x20: Last access time
    U64 alloc_size;         // 0x28: Allocated size
    U64 data_size;          // 0x30: Real size
    U32 file_attrs;         // 0x38: File attributes
    U32 reparse_tag;        // 0x3C: Reparse tag (if reparse point)
    U8  name_length;        // 0x40: Filename length (chars)
    U8  name_type;          // 0x41: Filename type (0-3)
    U16 name[];             // 0x42: Filename (UTF-16LE)
};

// Index Root Attribute (0x90)
class CNTFSIndexRoot {
    U32 attr_type;          // 0x00: Indexed attribute type (0x30 for dirs)
    U32 collation_rule;     // 0x04: Collation rule
    U32 index_block_size;   // 0x08: Index allocation entry size
    U8  clusters_per_index; // 0x0C: Clusters per index record
    U8  padding[3];         // 0x0D: Padding
    // Index header follows
    U32 entries_offset;     // 0x10: Offset to first entry
    U32 index_length;       // 0x14: Total size of entries
    U32 alloc_length;       // 0x18: Allocated size
    U8  flags;              // 0x1C: Flags (has subnodes)
    U8  padding2[3];        // 0x1D: Padding
    // Index entries follow
};

// Index Entry (in index root or allocation)
class CNTFSIndexEntry {
    U64 mft_ref;            // 0x00: MFT reference (if not last entry)
    U16 length;             // 0x08: Entry length
    U16 key_length;         // 0x0A: Key length
    U16 flags;              // 0x0C: Entry flags
    U16 padding;            // 0x0E: Padding
    // Key data follows (CNTFSFilename for $I30)
    // If SUBNODE flag: VCN of child at end - 8
};

// Index Allocation Header (0xA0 data)
class CNTFSIndexBlock {
    U32 magic;              // 0x00: 'INDX'
    U16 update_seq_offset;  // 0x04: Offset to update sequence
    U16 update_seq_count;   // 0x06: Update sequence count
    U64 lsn;                // 0x08: Log file sequence number
    U64 vcn;                // 0x10: VCN of this block
    // Index header follows (same as in Index Root at offset 0x18)
    U32 entries_offset;     // 0x18: Offset to first entry
    U32 index_length;       // 0x1C: Total size of entries
    U32 alloc_length;       // 0x20: Allocated size
    U8  flags;              // 0x24: Has subnodes
    U8  padding[3];         // 0x25: Padding
    // Update sequence array
    // Index entries follow
};

// ============================================================================
// IN-MEMORY STRUCTURES
// ============================================================================

// Run list entry (decoded)
class CNTFSRunEntry {
    U64 vcn;                // Virtual cluster number start
    U64 lcn;                // Logical cluster number (0 = sparse)
    U64 length;             // Number of clusters
    CNTFSRunEntry *next;
};

// Parsed MFT record
class CNTFSMFTRecord {
    U64 record_number;      // MFT record number
    U16 flags;              // Record flags
    U16 hard_links;         // Hard link count
    U64 base_record;        // Base record number

    // Standard info
    U64 creation_time;
    U64 modification_time;
    U64 mft_change_time;
    U64 access_time;
    U32 file_attrs;

    // From best filename
    U8  *name;              // UTF-8 filename
    U64 parent_ref;         // Parent MFT reference

    // Data attribute
    Bool data_resident;
    U8   *data_inline;      // Resident data
    U64  data_size;
    U64  alloc_size;
    CNTFSRunEntry *data_runs;   // Non-resident runs

    // For directories
    Bool is_directory;
    Bool has_index_alloc;
    CNTFSRunEntry *index_runs;
};

// NTFS filesystem state
class CNTFS {
    // Device access
    I64 device_fd;
    U8  *device_path;

    // Volume info
    U32 bytes_per_sector;
    U32 sectors_per_cluster;
    U32 bytes_per_cluster;
    U64 total_sectors;
    U64 total_clusters;
    U32 mft_record_size;
    U32 index_record_size;
    U64 mft_lcn;            // First cluster of $MFT
    U64 volume_serial;

    // Volume label
    U8  volume_label[128];

    // Cached MFT
    U8  *mft_record_buf;    // Buffer for reading MFT records

    // VFS integration
    CVFSSuperblock *vfs_sb;
    CVFSMount *vfs_mount;
    Bool read_only;
};

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

// Mount/unmount
I64 NTFSMount(CVFSMount *mnt, U8 *device, U8 *options);
I64 NTFSUnmount(CVFSMount *mnt);

// Inode operations
I64 NTFSLookup(CVFSInode *dir, U8 *name, CVFSInode **result);
I64 NTFSReadlink(CVFSInode *inode, U8 *buf, I64 size);

// File operations
I64 NTFSOpen(CVFSFile *file);
I64 NTFSClose(CVFSFile *file);
I64 NTFSRead(CVFSFile *file, U0 *buf, I64 size, I64 offset);
I64 NTFSReaddir(CVFSFile *file, U0 *buf, I64 count);

// Internal helpers
I64 NTFSReadClusters(CNTFS *fs, U64 lcn, I64 count, U0 *buf);
I64 NTFSReadMFTRecord(CNTFS *fs, U64 record_num, U0 *buf);
CNTFSMFTRecord *NTFSParseMFTRecord(CNTFS *fs, U0 *buf, U64 record_num);
U0 NTFSFreeMFTRecord(CNTFSMFTRecord *rec);
CNTFSRunEntry *NTFSDecodeRunlist(U8 *runlist, U64 base_vcn);
U0 NTFSFreeRunlist(CNTFSRunEntry *runs);
I64 NTFSReadAttribute(CNTFS *fs, CNTFSMFTRecord *rec, U0 *buf, I64 size, I64 offset);
U8 *NTFSUtf16ToUtf8(U16 *utf16, I64 len);

// Registration
U0 NTFSRegister();

#endif // _NTFS_HH
