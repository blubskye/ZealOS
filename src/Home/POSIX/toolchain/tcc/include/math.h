// math.h - Math functions for ZealOS TCC
// Part of the POSIX compatibility layer

#ifndef _MATH_H
#define _MATH_H

// Math constants
#define M_E         2.71828182845904523536
#define M_LOG2E     1.44269504088896340736
#define M_LOG10E    0.434294481903251827651
#define M_LN2       0.693147180559945309417
#define M_LN10      2.30258509299404568402
#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.785398163397448309616
#define M_1_PI      0.318309886183790671538
#define M_2_PI      0.636619772367581343076
#define M_2_SQRTPI  1.12837916709551257390
#define M_SQRT2     1.41421356237309504880
#define M_SQRT1_2   0.707106781186547524401

// Special values
#define HUGE_VAL    (__builtin_huge_val())
#define HUGE_VALF   (__builtin_huge_valf())
#define HUGE_VALL   (__builtin_huge_vall())
#define INFINITY    (__builtin_inff())
#define NAN         (__builtin_nanf(""))

// Classification macros
#define FP_NAN       0
#define FP_INFINITE  1
#define FP_ZERO      2
#define FP_SUBNORMAL 3
#define FP_NORMAL    4

// Classification functions
int fpclassify(double x);
int isfinite(double x);
int isinf(double x);
int isnan(double x);
int isnormal(double x);
int signbit(double x);

// Trigonometric functions
double sin(double x);
double cos(double x);
double tan(double x);
double asin(double x);
double acos(double x);
double atan(double x);
double atan2(double y, double x);

float sinf(float x);
float cosf(float x);
float tanf(float x);
float asinf(float x);
float acosf(float x);
float atanf(float x);
float atan2f(float y, float x);

// Hyperbolic functions
double sinh(double x);
double cosh(double x);
double tanh(double x);
double asinh(double x);
double acosh(double x);
double atanh(double x);

float sinhf(float x);
float coshf(float x);
float tanhf(float x);
float asinhf(float x);
float acoshf(float x);
float atanhf(float x);

// Exponential and logarithmic functions
double exp(double x);
double exp2(double x);
double expm1(double x);
double log(double x);
double log10(double x);
double log2(double x);
double log1p(double x);
double frexp(double x, int *exp);
double ldexp(double x, int exp);
double logb(double x);
int ilogb(double x);
double scalbn(double x, int n);
double scalbln(double x, long n);

float expf(float x);
float exp2f(float x);
float expm1f(float x);
float logf(float x);
float log10f(float x);
float log2f(float x);
float log1pf(float x);
float frexpf(float x, int *exp);
float ldexpf(float x, int exp);
float logbf(float x);
int ilogbf(float x);
float scalbnf(float x, int n);
float scalblnf(float x, long n);

// Power functions
double pow(double x, double y);
double sqrt(double x);
double cbrt(double x);
double hypot(double x, double y);

float powf(float x, float y);
float sqrtf(float x);
float cbrtf(float x);
float hypotf(float x, float y);

// Error and gamma functions
double erf(double x);
double erfc(double x);
double tgamma(double x);
double lgamma(double x);

float erff(float x);
float erfcf(float x);
float tgammaf(float x);
float lgammaf(float x);

// Rounding functions
double ceil(double x);
double floor(double x);
double trunc(double x);
double round(double x);
long lround(double x);
long long llround(double x);
double nearbyint(double x);
double rint(double x);
long lrint(double x);
long long llrint(double x);

float ceilf(float x);
float floorf(float x);
float truncf(float x);
float roundf(float x);
long lroundf(float x);
long long llroundf(float x);
float nearbyintf(float x);
float rintf(float x);
long lrintf(float x);
long long llrintf(float x);

// Remainder functions
double fmod(double x, double y);
double remainder(double x, double y);
double remquo(double x, double y, int *quo);

float fmodf(float x, float y);
float remainderf(float x, float y);
float remquof(float x, float y, int *quo);

// Manipulation functions
double copysign(double x, double y);
double nan(const char *tagp);
double nextafter(double x, double y);
double nexttoward(double x, long double y);

float copysignf(float x, float y);
float nanf(const char *tagp);
float nextafterf(float x, float y);
float nexttowardf(float x, long double y);

// Maximum, minimum, and positive difference
double fdim(double x, double y);
double fmax(double x, double y);
double fmin(double x, double y);

float fdimf(float x, float y);
float fmaxf(float x, float y);
float fminf(float x, float y);

// Other functions
double fabs(double x);
double fma(double x, double y, double z);
double modf(double x, double *iptr);

float fabsf(float x);
float fmaf(float x, float y, float z);
float modff(float x, float *iptr);

// Bessel functions (POSIX)
double j0(double x);
double j1(double x);
double jn(int n, double x);
double y0(double x);
double y1(double x);
double yn(int n, double x);

#endif // _MATH_H
