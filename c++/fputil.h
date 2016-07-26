//==============================================================
//  Mathematics     fputil.h    v1.00
//      Various floating-point utilities
//  Coyright 1995 Scott Robert Ladd
//==============================================================

#ifndef FPUTIL_H
#define FPUTIL_H

#include "stddef.h"
#include "math.h"
#include "diagnose.h"

//--------------------
// math exception type
//--------------------

class MathLibEx : public ExceptionBase
    {
    public:
        MathLibEx
            (
            const exception * e
            );

		virtual void Explain
            (
            DiagOutput & out
            );

    private:
        exception EData;
    };

//-----------------------
// Round to nearest value
//-----------------------

double ToNearest(double x);
#ifndef __WATCOMC__
long double ToNearest(long double x);
#endif

//--------------------------------------------
// set number of significant digits in a value
//--------------------------------------------

double SigDig(double x, size_t n);

#ifndef __WATCOMC__
long double SigDig(long double x, size_t n);
#endif

//------------------------------------------------------
// lowest common denominator and greatest common multiple
//------------------------------------------------------

unsigned long LCM(unsigned long x, unsigned long y);
unsigned long GCD(unsigned long x, unsigned long y);

//----------------------
// extra trig functions!
//----------------------

inline double asinh(const double& x)
    {
    return log(x + sqrt(x * x + 1.0));
    }

inline double acosh(const double& x)
    {
    return log(x + sqrt(x * x - 1.0));
    }

inline double atanh(const double& x)
    {
    return log((1.0 + x) / (1.0 - x)) / 2.0;
    }

#ifndef __WATCOMC__

inline long double asinhl(const long double & x)
    {
    return logl(x + sqrtl(x * x + 1.0L));
    }

inline long double acoshl(const long double & x)
    {
    return logl(x + sqrtl(x * x - 1.0L));
    }

inline long double atanhl(const long double & x)
    {
    return logl((1.0L + x) / (1.0L - x)) / 2.0L;
    }

#endif // __WATCOMC__

#endif
