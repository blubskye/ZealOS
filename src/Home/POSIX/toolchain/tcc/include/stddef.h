// stddef.h - Standard definitions for ZealOS TCC
// Part of the POSIX compatibility layer

#ifndef _STDDEF_H
#define _STDDEF_H

// NULL pointer
#ifndef NULL
#define NULL ((void*)0)
#endif

// Size type
typedef unsigned long size_t;
typedef long ssize_t;

// Pointer difference
typedef long ptrdiff_t;

// Wide character
typedef int wchar_t;

// Max alignment type
typedef long double max_align_t;

// Offset of member in struct
#define offsetof(type, member) ((size_t)&((type*)0)->member)

#endif // _STDDEF_H
