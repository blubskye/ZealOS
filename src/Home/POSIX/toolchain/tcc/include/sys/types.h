// sys/types.h - System types for ZealOS TCC
// Part of the POSIX compatibility layer

#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#include <stddef.h>
#include <stdint.h>

// Basic types
typedef int             pid_t;
typedef unsigned int    uid_t;
typedef unsigned int    gid_t;
typedef long            off_t;
typedef unsigned long   ino_t;
typedef unsigned int    mode_t;
typedef unsigned long   nlink_t;
typedef long            blksize_t;
typedef long            blkcnt_t;
typedef unsigned int    dev_t;
typedef long            time_t;
typedef long            suseconds_t;
typedef long            clock_t;
typedef unsigned long   fsblkcnt_t;
typedef unsigned long   fsfilcnt_t;
typedef int             id_t;
typedef int             key_t;
typedef unsigned long   pthread_t;
typedef unsigned int    socklen_t;
typedef int64_t         loff_t;
typedef int64_t         off64_t;

// Additional BSD types
typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;
typedef unsigned int    uint;
typedef unsigned short  ushort;

// Fixed-size types (also in stdint.h)
typedef uint8_t         u_int8_t;
typedef uint16_t        u_int16_t;
typedef uint32_t        u_int32_t;
typedef uint64_t        u_int64_t;

// Register type
typedef long            register_t;

#endif // _SYS_TYPES_H
