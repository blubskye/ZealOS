// stdio.h - Standard I/O header for ZealOS TCC
// Part of the POSIX compatibility layer

#ifndef _STDIO_H
#define _STDIO_H

#include <stddef.h>
#include <stdarg.h>

// File structure (opaque)
typedef struct _FILE FILE;

// Standard streams
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

// Seek origins
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

// Buffer modes
#define _IOFBF      0   // Full buffering
#define _IOLBF      1   // Line buffering
#define _IONBF      2   // No buffering

// Buffer size
#define BUFSIZ      8192

// EOF marker
#define EOF         (-1)

// Filename limits
#define FILENAME_MAX    4096
#define FOPEN_MAX       256
#define TMP_MAX         238328
#define L_tmpnam        20

// File operations
FILE *fopen(const char *path, const char *mode);
FILE *freopen(const char *path, const char *mode, FILE *stream);
FILE *fdopen(int fd, const char *mode);
int fclose(FILE *stream);
int fflush(FILE *stream);

// File positioning
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
void rewind(FILE *stream);
int fgetpos(FILE *stream, long *pos);
int fsetpos(FILE *stream, const long *pos);

// Character I/O
int fgetc(FILE *stream);
int getc(FILE *stream);
int getchar(void);
int ungetc(int c, FILE *stream);
int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);

// String I/O
char *fgets(char *s, int size, FILE *stream);
char *gets(char *s);  // Deprecated
int fputs(const char *s, FILE *stream);
int puts(const char *s);

// Binary I/O
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

// Formatted I/O
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
int vprintf(const char *format, va_list ap);
int vfprintf(FILE *stream, const char *format, va_list ap);
int vsprintf(char *str, const char *format, va_list ap);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);

int scanf(const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int sscanf(const char *str, const char *format, ...);
int vscanf(const char *format, va_list ap);
int vfscanf(FILE *stream, const char *format, va_list ap);
int vsscanf(const char *str, const char *format, va_list ap);

// Error handling
int feof(FILE *stream);
int ferror(FILE *stream);
void clearerr(FILE *stream);
void perror(const char *s);

// File descriptors
int fileno(FILE *stream);

// Buffering
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
void setbuf(FILE *stream, char *buf);
void setbuffer(FILE *stream, char *buf, size_t size);
void setlinebuf(FILE *stream);

// Temporary files
FILE *tmpfile(void);
char *tmpnam(char *s);
char *tempnam(const char *dir, const char *prefix);

// File removal
int remove(const char *pathname);
int rename(const char *oldpath, const char *newpath);

// Pipes
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);

#endif // _STDIO_H
