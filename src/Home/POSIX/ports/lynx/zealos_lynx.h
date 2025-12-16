// zealos_lynx.h - ZealOS-specific configuration for Lynx
// This header provides ZealOS compatibility definitions for the Lynx browser
//
// To build Lynx for ZealOS:
// 1. Copy this file to the lynx source directory as lynx_cfg.h
// 2. Use the ZealOS POSIX layer with curses support
// 3. Build with the ZealOS TCC toolchain or cross-compile

#ifndef ZEALOS_LYNX_H
#define ZEALOS_LYNX_H

// ============================================================================
// PLATFORM IDENTIFICATION
// ============================================================================

#define ZEALOS 1
#define UNIX 1              // POSIX-like behavior
#define HAVE_CONFIG_H 0     // Using this manual config instead

// ============================================================================
// COMPILER FEATURES
// ============================================================================

#define HAVE_STDARG_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_UNISTD_H 1
#define HAVE_FCNTL_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_DIRENT_H 1
#define HAVE_ERRNO_H 1
#define HAVE_LIMITS_H 1
#define HAVE_MEMORY_H 1
#define HAVE_STDBOOL_H 1
#define HAVE_STDINT_H 1

// Time headers
#define HAVE_TIME_H 1
#define HAVE_SYS_TIME_H 1
#define TIME_WITH_SYS_TIME 1

// ============================================================================
// STRING FUNCTIONS
// ============================================================================

#define HAVE_STRCHR 1
#define HAVE_STRRCHR 1
#define HAVE_STRSTR 1
#define HAVE_STRCASECMP 1
#define HAVE_STRNCASECMP 1
#define HAVE_STRCASESTR 1
#define HAVE_STRDUP 1
#define HAVE_STRERROR 1
#define HAVE_SNPRINTF 1
#define HAVE_VSNPRINTF 1
#define HAVE_STRTOL 1
#define HAVE_STRTOUL 1
#define HAVE_ATOI 1
#define HAVE_ATOL 1

// Obsolete but lynx uses them
#define HAVE_INDEX 0
#define HAVE_RINDEX 0
#define index strchr
#define rindex strrchr

// ============================================================================
// MEMORY FUNCTIONS
// ============================================================================

#define HAVE_MALLOC 1
#define HAVE_REALLOC 1
#define HAVE_CALLOC 1
#define HAVE_FREE 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_MEMSET 1
#define HAVE_MEMCMP 1
#define HAVE_ALLOCA 0       // Don't use alloca

// ============================================================================
// I/O FUNCTIONS
// ============================================================================

#define HAVE_FOPEN 1
#define HAVE_FCLOSE 1
#define HAVE_FREAD 1
#define HAVE_FWRITE 1
#define HAVE_FGETS 1
#define HAVE_FPUTS 1
#define HAVE_FSEEK 1
#define HAVE_FTELL 1
#define HAVE_FFLUSH 1
#define HAVE_FEOF 1
#define HAVE_FERROR 1
#define HAVE_CLEARERR 1
#define HAVE_FILENO 1

// File descriptors
#define HAVE_OPEN 1
#define HAVE_CLOSE 1
#define HAVE_READ 1
#define HAVE_WRITE 1
#define HAVE_LSEEK 1
#define HAVE_DUP 1
#define HAVE_DUP2 1
#define HAVE_PIPE 1

// File info
#define HAVE_STAT 1
#define HAVE_FSTAT 1
#define HAVE_LSTAT 1
#define HAVE_ACCESS 1
#define HAVE_UNLINK 1
#define HAVE_RENAME 1
#define HAVE_MKDIR 1
#define HAVE_RMDIR 1
#define HAVE_GETCWD 1
#define HAVE_CHDIR 1
#define HAVE_TRUNCATE 1
#define HAVE_FTRUNCATE 1

// Directory operations
#define HAVE_OPENDIR 1
#define HAVE_READDIR 1
#define HAVE_CLOSEDIR 1
#define HAVE_REWINDDIR 1

// ============================================================================
// PROCESS FUNCTIONS
// ============================================================================

#define HAVE_FORK 1
#define HAVE_VFORK 0        // Use fork
#define HAVE_EXECVE 1
#define HAVE_WAITPID 1
#define HAVE_GETPID 1
#define HAVE_GETPPID 1
#define HAVE_GETUID 1
#define HAVE_GETEUID 1
#define HAVE_GETGID 1
#define HAVE_GETEGID 1
#define HAVE_SETUID 0       // Not supported
#define HAVE_SETEUID 0
#define HAVE_SETGID 0
#define HAVE_SETEGID 0
#define HAVE_SYSTEM 1
#define HAVE_POPEN 1
#define HAVE_PCLOSE 1

// ============================================================================
// SIGNAL HANDLING
// ============================================================================

#define HAVE_SIGNAL 1
#define HAVE_SIGACTION 1
#define HAVE_SIGPROCMASK 1
#define HAVE_SIGEMPTYSET 1
#define HAVE_SIGADDSET 1
#define HAVE_SIGSETJMP 0    // Not fully supported

// Signals available
#define HAVE_SIGINT 1
#define HAVE_SIGTERM 1
#define HAVE_SIGPIPE 1
#define HAVE_SIGCHLD 1
#define HAVE_SIGWINCH 1
#define HAVE_SIGTSTP 0      // Job control not fully supported

// ============================================================================
// NETWORK FUNCTIONS
// ============================================================================

#define HAVE_SOCKET 1
#define HAVE_CONNECT 1
#define HAVE_BIND 1
#define HAVE_LISTEN 1
#define HAVE_ACCEPT 1
#define HAVE_SEND 1
#define HAVE_RECV 1
#define HAVE_SENDTO 1
#define HAVE_RECVFROM 1
#define HAVE_GETSOCKOPT 1
#define HAVE_SETSOCKOPT 1
#define HAVE_GETSOCKNAME 1
#define HAVE_GETPEERNAME 1
#define HAVE_SHUTDOWN 1
#define HAVE_SELECT 1
#define HAVE_POLL 1

// Name resolution
#define HAVE_GETHOSTBYNAME 1
#define HAVE_GETHOSTBYADDR 1
#define HAVE_GETADDRINFO 1
#define HAVE_FREEADDRINFO 1
#define HAVE_GETNAMEINFO 1
#define HAVE_GETSERVBYNAME 1
#define HAVE_GETSERVBYPORT 1
#define HAVE_INET_ATON 1
#define HAVE_INET_NTOA 1
#define HAVE_INET_PTON 1
#define HAVE_INET_NTOP 1

// Network headers
#define HAVE_NETDB_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_SYS_SOCKET_H 1

// ============================================================================
// TERMINAL / CURSES
// ============================================================================

// Use our curses implementation
#define HAVE_CURSES_H 0     // Not standard curses.h
#define HAVE_NCURSES_H 0
#define HAVE_NCURSES_NCURSES_H 0
#define USE_SLANG 0

// ZealOS curses features
#define COLOR_CURSES 1
#define FANCY_CURSES 1
#define HAVE_CBREAK 1
#define HAVE_KEYPAD 1
#define HAVE_NODELAY 1
#define HAVE_RESIZETERM 0   // Not yet implemented
#define HAVE_USE_DEFAULT_COLORS 0
#define HAVE_NEWTERM 0
#define HAVE_TTYTYPE 0
#define HAVE_TYPE_CHTYPE 1

// Terminal I/O
#define HAVE_TERMIOS_H 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_TCGETATTR 1
#define HAVE_TCSETATTR 1
#define HAVE_ISATTY 1
#define HAVE_TTYNAME 1

// Window size
#define HAVE_SIZECHANGE 1

// ============================================================================
// TIME FUNCTIONS
// ============================================================================

#define HAVE_TIME 1
#define HAVE_LOCALTIME 1
#define HAVE_GMTIME 1
#define HAVE_MKTIME 1
#define HAVE_STRFTIME 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_USLEEP 1
#define HAVE_NANOSLEEP 1
#define HAVE_DIFFTIME 1

// ============================================================================
// USER DATABASE
// ============================================================================

#define HAVE_PWD_H 1
#define HAVE_GETPWUID 1
#define HAVE_GETPWNAM 1
#define HAVE_GRP_H 0        // Group database not implemented
#define HAVE_GETGRGID 0
#define HAVE_GETGRNAM 0

// ============================================================================
// LOCALE / INTERNATIONALIZATION
// ============================================================================

#define HAVE_LOCALE_H 1
#define HAVE_SETLOCALE 1
#define HAVE_LANGINFO_H 0
#define HAVE_NL_LANGINFO 0
#define HAVE_GETTEXT 0
#define HAVE_LIBINTL_H 0
#define LOCALE 1

// Character set
#define DEFAULT_ASSUME_CHARSET "iso-8859-1"

// ============================================================================
// SSL/TLS
// ============================================================================

// ZealOS has native TLS 1.3 support
#define USE_SSL 1
#define USE_ZEALOS_TLS 1    // Use our TLS implementation
#define USE_OPENSSL 0
#define USE_GNUTLS 0

// ============================================================================
// COMPRESSION
// ============================================================================

#define HAVE_ZLIB 0         // Not yet available
#define HAVE_BZLIB 0
#define USE_ZLIB 0
#define USE_BZLIB 0
#define USE_BROTLI 0

// ============================================================================
// LYNX FEATURES
// ============================================================================

// Enable features
#define USE_COLOR_STYLE 1
#define USE_SCROLLBAR 1
#define USE_PRETTYSRC 1
#define USE_SOURCE_CACHE 1
#define USE_JUSTIFY_ELTS 1
#define USE_EXTERNALS 1
#define USE_SESSIONS 1
#define USE_PERSISTENT_COOKIES 1

// Protocols
#define DISABLE_NEWS 0
#define DISABLE_FTP 0
#define DISABLE_FINGER 0
#define DISABLE_GOPHER 0

// Disable features that need more work
#define DISABLE_BIBP 1
#define USE_IDNA 0
#define USE_BLINK 0

// ============================================================================
// PATHS
// ============================================================================

#define LYNX_CFG_FILE "/Home/POSIX/etc/lynx.cfg"
#define LYNX_LSS_FILE "/Home/POSIX/etc/lynx.lss"
#define HELPFILE "file://localhost/Home/POSIX/share/lynx_help/lynx_help_main.html"
#define DEFAULT_INDEX_FILE "file://localhost/Home/POSIX/share/lynx_help/index.html"
#define GLOBAL_MAILCAP "/Home/POSIX/etc/mailcap"
#define GLOBAL_EXTENSION_MAP "/Home/POSIX/etc/mime.types"

// Temporary files
#define TEMP_SPACE "/tmp/"
#define LYNX_TEMP_SPACE "/tmp/"

// ============================================================================
// MISCELLANEOUS
// ============================================================================

#define HAVE_UTMP 0
#define HAVE_GETLOGIN 1
#define HAVE_CUSERID 0
#define HAVE_TGETENT 0      // No termcap database
#define HAVE_PUTENV 1
#define HAVE_SETENV 1
#define HAVE_UNSETENV 1
#define HAVE_GETENV 1
#define HAVE_ATEXIT 1

// Types
#define HAVE_TYPE_UNIONWAIT 0
#define RETSIGTYPE void
#define SIZEOF_INT 4
#define SIZEOF_LONG 8
#define SIZEOF_SHORT 2

// Byte order (x86_64 is little endian)
#define WORDS_BIGENDIAN 0

// ============================================================================
// CURSES WRAPPER FOR ZEALOS
// ============================================================================

// If building with ZealOS curses, include it
#ifdef ZEALOS
// The ZealOS curses.ZC provides these already
// Just make sure the necessary types are available

#ifndef WINDOW
// Forward declare if needed - full definition in curses.ZC
typedef struct _WINDOW WINDOW;
#endif

// Ensure chtype is defined
#ifndef chtype
typedef unsigned int chtype;
#endif

#endif // ZEALOS

#endif // ZEALOS_LYNX_H
