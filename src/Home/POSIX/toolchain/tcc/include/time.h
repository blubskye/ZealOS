// time.h - Time functions for ZealOS TCC
// Part of the POSIX compatibility layer

#ifndef _TIME_H
#define _TIME_H

#include <stddef.h>

// Types
typedef long time_t;
typedef long clock_t;
typedef int clockid_t;
typedef int timer_t;

// Clocks per second
#define CLOCKS_PER_SEC  1000000

// Clock IDs
#define CLOCK_REALTIME              0
#define CLOCK_MONOTONIC             1
#define CLOCK_PROCESS_CPUTIME_ID    2
#define CLOCK_THREAD_CPUTIME_ID     3
#define CLOCK_MONOTONIC_RAW         4
#define CLOCK_REALTIME_COARSE       5
#define CLOCK_MONOTONIC_COARSE      6
#define CLOCK_BOOTTIME              7

// Time spec
struct timespec {
    time_t  tv_sec;     // Seconds
    long    tv_nsec;    // Nanoseconds
};

// Time value (for select, etc.)
struct timeval {
    time_t      tv_sec;     // Seconds
    long        tv_usec;    // Microseconds
};

// Broken-down time
struct tm {
    int tm_sec;     // Seconds [0,60]
    int tm_min;     // Minutes [0,59]
    int tm_hour;    // Hour [0,23]
    int tm_mday;    // Day of month [1,31]
    int tm_mon;     // Month [0,11]
    int tm_year;    // Year - 1900
    int tm_wday;    // Day of week [0,6] (Sunday = 0)
    int tm_yday;    // Day of year [0,365]
    int tm_isdst;   // Daylight saving time flag
    long tm_gmtoff; // Offset from UTC in seconds
    const char *tm_zone; // Timezone abbreviation
};

// Timer event specification
struct itimerspec {
    struct timespec it_interval;    // Timer interval
    struct timespec it_value;       // Initial expiration
};

// Time manipulation
time_t time(time_t *tloc);
double difftime(time_t time1, time_t time0);
time_t mktime(struct tm *tm);

// Time conversion
char *asctime(const struct tm *tm);
char *asctime_r(const struct tm *tm, char *buf);
char *ctime(const time_t *timep);
char *ctime_r(const time_t *timep, char *buf);
struct tm *gmtime(const time_t *timep);
struct tm *gmtime_r(const time_t *timep, struct tm *result);
struct tm *localtime(const time_t *timep);
struct tm *localtime_r(const time_t *timep, struct tm *result);

// Time formatting
size_t strftime(char *s, size_t max, const char *format, const struct tm *tm);
char *strptime(const char *s, const char *format, struct tm *tm);

// Clock functions
clock_t clock(void);
int clock_gettime(clockid_t clk_id, struct timespec *tp);
int clock_settime(clockid_t clk_id, const struct timespec *tp);
int clock_getres(clockid_t clk_id, struct timespec *res);
int clock_nanosleep(clockid_t clk_id, int flags, const struct timespec *request,
                    struct timespec *remain);

// Sleep functions
int nanosleep(const struct timespec *req, struct timespec *rem);

// Timer functions
int timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid);
int timer_delete(timer_t timerid);
int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value,
                  struct itimerspec *old_value);
int timer_gettime(timer_t timerid, struct itimerspec *curr_value);
int timer_getoverrun(timer_t timerid);

// Timezone
extern char *tzname[2];
extern long timezone;
extern int daylight;
void tzset(void);

// TIMER_ABSTIME flag for clock_nanosleep
#define TIMER_ABSTIME   1

#endif // _TIME_H
