// string.h - String operations header for ZealOS TCC
// Part of the POSIX compatibility layer

#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

// Copying
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);

// Concatenation
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t n);

// Comparison
int memcmp(const void *s1, const void *s2, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
int strcoll(const char *s1, const char *s2);
size_t strxfrm(char *dest, const char *src, size_t n);

// Searching
void *memchr(const void *s, int c, size_t n);
char *strchr(const char *s, int c);
size_t strcspn(const char *s, const char *reject);
char *strpbrk(const char *s, const char *accept);
char *strrchr(const char *s, int c);
size_t strspn(const char *s, const char *accept);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *str, const char *delim);
char *strtok_r(char *str, const char *delim, char **saveptr);

// Miscellaneous
void *memset(void *s, int c, size_t n);
char *strerror(int errnum);
size_t strlen(const char *s);
size_t strnlen(const char *s, size_t maxlen);

// BSD extensions
char *strdup(const char *s);
char *strndup(const char *s, size_t n);
size_t strlcpy(char *dst, const char *src, size_t size);
size_t strlcat(char *dst, const char *src, size_t size);

// Case-insensitive comparison
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);

// GNU extensions
void *mempcpy(void *dest, const void *src, size_t n);
void *memrchr(const void *s, int c, size_t n);
char *stpcpy(char *dest, const char *src);
char *stpncpy(char *dest, const char *src, size_t n);

// Explicit memory operations
void explicit_bzero(void *s, size_t n);

#endif // _STRING_H
