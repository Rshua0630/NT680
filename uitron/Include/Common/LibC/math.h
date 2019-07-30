#ifndef _NVT_LIBC_MATH_H
#define _NVT_LIBC_MATH_H

/* Useful constants.  */

# ifndef HUGE_VAL
#  define HUGE_VAL (__builtin_huge_val())
# endif

# ifndef HUGE_VALF
#  define HUGE_VALF (__builtin_huge_valf())
# endif

# ifndef HUGE_VALL
#  define HUGE_VALL (__builtin_huge_vall())
# endif

# ifndef INFINITY
#  define INFINITY (__builtin_inff())
# endif

# ifndef NAN
#  define NAN (__builtin_nanf(""))
# endif

//#define MAXFLOAT    3.40282347e+38F
#define MAXDOUBLE   1.79769313486231470e+308
#define MAXFLOAT    ((float)3.40282346638528860e+38)
#define MINDOUBLE   4.94065645841246544e-324
#define MINFLOAT    ((float)1.40129846432481707e-45)

#define M_E         2.7182818284590452354
#define M_LOG2E     1.4426950408889634074
#define M_LOG10E    0.43429448190325182765
#define M_LN2       0.69314718055994530942
#define M_LN10      2.30258509299404568402
#define M_PI        3.14159265358979323846
#define M_TWOPI     (M_PI * 2.0)
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.78539816339744830962
#define M_3PI_4     2.3561944901923448370E0
#define M_SQRTPI    1.77245385090551602792981
#define M_1_PI      0.31830988618379067154
#define M_2_PI      0.63661977236758134308
#define M_2_SQRTPI  1.12837916709551257390
#define M_SQRT2     1.41421356237309504880
#define M_SQRT1_2   0.70710678118654752440
#define M_LN2LO     1.9082149292705877000E-10
#define M_LN2HI     6.9314718036912381649E-1
#define M_SQRT3     1.73205080756887719000
#define M_IVLN10    0.43429448190325182765   /* 1 / log(10) */
#define M_LOG2_E    0.693147180559945309417  /* Natural log of 2 */
#define M_INVLN2    1.4426950408889633870E0  /* 1 / log(2) */

//NOTE:
//given an value=integer+fractional=sign*mantissa*(2^exponent)
//than:
//fabs(value)=remove sign= mantissa*(2^exponent)
//fmod(x,y)=integer of x/y = x%y.
//modf(value,&integer)=fractional
//ceil(value)=smallest integer no less than value
//floor(value)=largest integer not greater than value
//ldexp(mantissa,exponent)=value=x*(2^y)
//frexp(value,&exponent)=mantissa (NOTE: adjust mantissa to 0.5~1, calc exponent)
//exp(x)=e^x
//pow(x,y)=x^y
//sqrt(x^2)=x (NOTE:(x^2)>=0)
//log(e^x)=x  (NOTE:(e^x)>0)
//log10(10^x)=x (NOTE:(10^x)>0)

//NOTE:
//given an circle points of degree theta
//if its position is (dx,dy), distance to orig is (radius)
//the triangle edges = (dx,dy,radius)
//radius = sqrt(dx*dx+dy*dy)
//then:
//sin(theta)=dy/radius
//cos(theta)=dx/radius
//tan(theta)=dy/dx
//asin(dy/radius)=sin^-1(dy/radius)=theta
//acos(dx/radius)=cos^-1(dx/radius)=theta
//atan(dy/dx)=tan^-1(dy/dx)=theta
//atan2(dx,dy)=tan^-1(dy/dx)=theta

//NOTE:
//given an circle points of degree theta -- on complexity plane
//if its position is (dx,dy), distance to orig is (radius)
//the triangle edges = (dx,dy,radius)
//radius = sqrt(dx*dx-dy*dy)
//then:
//sinh(x)=-i*sin(i*x)=(exp(x)-exp(-x))/2
//cosh(x)=   cos(i*x)=(exp(x)+exp(-x))/2
//tanh(x)=-i*tan(i*x)=(exp(x)-exp(-x))/(exp(x)-exp(-x))


/* Reentrant ANSI C functions.  */

extern double atan(double);
extern double cos(double);
extern double sin(double);
extern double tan(double);
extern double tanh(double);
extern double frexp(double, int *);
extern double modf(double, double *);
extern double ceil(double);
extern double fabs(double);
extern double floor(double);

/* Non reentrant ANSI C functions.  */

#ifndef _REENT_ONLY
extern double acos(double);
extern double asin(double);
extern double atan2(double, double);
extern double cosh(double);
extern double sinh(double);
extern double exp(double);
extern double ldexp(double, int);
extern double log(double);
extern double log10(double);
extern double pow(double, double);
extern double sqrt(double);
extern double fmod(double, double);
#endif /* ! defined (_REENT_ONLY) */

#define log2(x) (log (x) / M_LOG2_E)

/* Reentrant ANSI C functions.  */

extern float atanf(float);
extern float cosf(float);
extern float sinf(float);
extern float tanf(float);
extern float tanhf(float);
extern float frexpf(float, int *);
extern float modff(float, float *);
extern float ceilf(float);
extern float fabsf(float);
extern float floorf(float);

/* Non reentrant ANSI C functions.  */

#ifndef _REENT_ONLY
extern float acosf(float);
extern float asinf(float);
extern float atan2f(float, float);
extern float coshf(float);
extern float sinhf(float);
extern float expf(float);
extern float ldexpf(float, int);
extern float logf(float);
extern float log10f(float);
extern float powf(float, float);
extern float sqrtf(float);
extern float fmodf(float, float);
#endif /* ! defined (_REENT_ONLY) */

#define log2f(x) (logf (x) / (float) M_LOG2_E)

extern int __errno; //for math lib linking

#if 1
struct exception {
	int type;
	char *name;
	double arg1;
	double arg2;
	double retval;
	int err;
};

extern int matherr(struct exception *e);

/* Values for the type field of struct exception.  */

#define DOMAIN 1
#define SING 2
#define OVERFLOW 3
#define UNDERFLOW 4
#define TLOSS 5
#define PLOSS 6
#endif

#endif //_NVT_CLIB_MATH_H
