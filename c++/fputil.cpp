//==============================================================
//  Mathematics     fputil.cpp  v1.00
//      Various floating-point utilities
//  Coyright 1995 Scott Robert Ladd
//==============================================================

#include "fputil.h"
#include "float.h"
#include "stdio.h"

//--------------------
// math exception trap
//--------------------

int _matherr
    (
    exception * e
    )
    {
    throw MathLibEx(e);

    #ifdef _MSC_VER
    return 0;
    #endif
    }

MathLibEx::MathLibEx
    (
    const exception * e
    )
    {
    if (e == NULL)
        {
        EData.type   = 0;
        EData.name   = "???";
        EData.arg1   = 0.0;
        EData.arg2   = 0.0;
        EData.retval = 0.0;
        }
    else
        EData = *e;
    }

void MathLibEx::Explain
    (
    DiagOutput & out
    )
    {
    static const char * ErrNames[] =
        {
        "Unknown",
        "Domain",
        "Singularity",
        "Overflow",
        "Underflow",
        "Total Loss of Precision",
        "Partial Loss of Precision",
        "Stack Fault"
        };

    char buffer[128];

    sprintf(buffer,"Math error %s in %s, "
                   "a1 = %g, a2 = %g, ret = %g",
                   ErrNames[EData.type], EData.name,
                   EData.arg1, EData.arg2, EData.retval);

    out.DisplayMsg(buffer,DIAG_FATAL);
    }

//-----------------------
// Round to nearest value
//-----------------------

double ToNearest(double x)
    {
    double i, f, dummy;

    f = fabs(modf(x,&i));

    if (f == 0.0)
        return i;

    if (f == 0.5)
        {
        if (modf(i / 2.0, &dummy) != 0.0)
            {
            if (x < 0.0)
                i -= 1.0;
            else
                i += 1.0;
            }
        }
    else
        {
        if (f > 0.5)
            {
            if (x < 0.0)
                i -= 1.0;
            else
                i += 1.0;
            }
        }

    return i;
    }

#ifndef __WATCOMC__

long double ToNearest(long double x)
    {
    double i, f, dummy;

    f = fabs(modf(x,&i));

    if (f == 0.0L)
        return i;

    if (f == 0.5L)
        {
        if (modf(i / 2.0L, &dummy) != 0.0L)
            {
            if (x < 0.0L)
                i -= 1.0L;
            else
                i += 1.0L;
            }
        }
    else
        {
        if (f > 0.5L)
            {
            if (x < 0.0L)
                i -= 1.0L;
            else
                i += 1.0L;
            }
        }

    return i;
    }

#endif // __WATCOMC__

//--------------------------------------------
// set number of significant digits in a value
//--------------------------------------------

double SigDig
    (
    double x,
    size_t n
    )
    {
    double s, result;

    if (fabs(x) < 1.0E-300)
        result = 0.0;
    else
        {
        if ((n == 0U) || (n > DBL_DIG))
            result = x;
        else
            {
            #ifdef __BORLANDC__
                s = pow10((int)n - 1 - (int)floor(log10(fabs(x))));
            #else
                s = pow(10.0,double((int)n - 1 - (int)floor(log10(fabs(x)))));
            #endif
            result = ToNearest(x * s) / s;
            }
        }

    return result;
    }

#ifndef __WATCOMC__

long double SigDig(long double x, size_t n)
    {
    long double shift, result;

    if ((n == 0u) || (n > LDBL_DIG))
        result = x;
    else
        {
        --n;

        shift = powl(10.0L,(double)n - floorl(log10l(fabsl(x))));

        result = ToNearest(x * shift) / shift;
        }

    return result;
    }

#endif // __WATCOMC__

//------------------------------------------------------
// lowest common denominator and greatest common multiple
//------------------------------------------------------

unsigned long LCM(unsigned long x, unsigned long y)
    {
    unsigned long s, l;

    if (x == y)
        return x;

    if (x < y)
        {
        s = x;
        l = y;
        }
    else
        {
        l = x;
        s = y;
        }

    return (l / GCD(s,l)) * s;
    }

unsigned long GCD(unsigned long x, unsigned long y)
    {
    unsigned long temp;

    while (y != 0ul)
        {
        temp = x % y;
        x = y;
        y = temp;
        }

    return x;
    }


