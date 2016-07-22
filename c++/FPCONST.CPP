//==============================================================
//  MATHEMATICS LIBRARY     fpconst.cpp   v1.00
//
//      Definitions of various mathematical constants. These
//      constants assume that float is an IEEE single, that
//      double is an IEEE double, and that long double is
//      the 80-bit extended IEEE double used as a temporary
//      type by the 80x87 math coprocessors.
//
//      Copyright 1991-1995 Scott Robert Ladd.
//==============================================================

#include "fpconst.h"

#ifdef __BORLANDC__
#pragma warn -use
#endif

// float constants
const float N_FPI         =  3.141593F;     // pi
const float N_F1PI        =  0.318310F;     // 1 / pi
const float N_F2PI        =  0.636620F;     // 2 / pi
const float N_FPISQR      =  9.869604F;     // pi squared
const float N_FPISQRT     =  1.772454F;     // square root pi
const float N_FPI2        =  1.570796F;     // pi / 2
const float N_FPI3        =  1.047198F;     // pi / 3
const float N_FPI4        =  0.785398F;     // pi / 4
const float N_FPILOG      =  1.144730F;     // log(pi)
const float N_FPILOG10    =  0.497150F;     // log10(pi)

const float N_FE          =  2.718282F;     // e
const float N_F1E         =  0.367879F;     // 1 / e
const float N_FESQR       =  7.389056F;     // e squared
const float N_FLOG10      =  2.302585F;     // log(10)
const float N_FLOG2       =  0.693147F;     // log(2)
const float N_FLOG10E     =  0.434294F;     // log10(e)
const float N_FLOG10SQE   =  0.217147F;     // log10(sqrt(e))
const float N_FLOG102     =  0.301030F;     // log10(2)

const float N_FSQRT2      =  1.414214F;     // sqrt(2)
const float N_FEULER      =  0.577216F;     // Euler's number
const float N_FEULORLOG   = -0.549539F;     // log(Euler)
const float N_FGOLDRATIO  =  1.618034F;     // Golden ratio

const float N_FDEGPERRAD  = 57.2958F;       // 180 / pi
const float N_FRADPERDEG  =  0.0174533F;    // pi / 180

// double constants
const double N_DPI         =  3.141592653589793238;
const double N_D1PI        =  0.3183098861837907;
const double N_D2PI        =  0.6366197723675813;
const double N_DPISQR      =  9.869604401089359;
const double N_DPISQRT     =  1.77245385090552;
const double N_DPI2        =  1.570796326794897;
const double N_DPI3        =  1.04719755119660;
const double N_DPI4        =  0.7853981633974483;
const double N_DPILOG      =  1.144729885849400;
const double N_DPILOG10    =  0.497149872694134;

const double N_DE          =  2.718281828459045;
const double N_D1E         =  0.367879441171442;
const double N_DESQR       =  7.389056098930650;
const double N_DLOG10      =  2.302585092994046;
const double N_DLOG2       =  0.6931471805599453;
const double N_DLOG10E     =  0.4342944819032518;
const double N_DLOG10SQE   =  0.2171472409516259;
const double N_DLOG102     =  0.301029995663981;

const double N_DSQRT2      =  1.414213562373095;
const double N_DEULER      =  0.5772156649015329;
const double N_DEULORLOG   = -0.5495393129816448;
const double N_DGOLDRATIO  =  1.618033988749895;

const double N_DDEGPERRAD  = 57.2957795130823;
const double N_DRADPERDEG  =  0.0174532925199433;

// long double constants
const long double N_EPI         =  3.141592653589793238L;
const long double N_E1PI        =  0.3183098861837906715L;
const long double N_E2PI        =  0.6366197723675813431L;
const long double N_EPISQR      =  9.869604401089358619L;
const long double N_EPISQRT     =  1.77245385090551603L;
const long double N_EPI2        =  1.570796326794896619L;
const long double N_EPI3        =  1.04719755119659775L;
const long double N_EPI4        =  0.7853981633974483097L;
const long double N_EPILOG      =  1.144729885849400174L;
const long double N_EPILOG10    =  0.497149872694133854L;

const long double N_EE          =  2.718281828459045235L;
const long double N_E1E         =  0.367879441171442322L;
const long double N_EESQR       =  7.389056098930650227L;
const long double N_ELOG10      =  2.302585092994045684L;
const long double N_ELOG2       =  0.6931471805599453094L;
const long double N_ELOG10E     =  0.4342944819032518277L;
const long double N_ELOG10SQE   =  0.217147240951625914L;
const long double N_ELOG102     =  0.301029995663981195L;

const long double N_ESQRT2      =  1.414213562373095049L;
const long double N_EEULER      =  0.5772156649015328606L;
const long double N_EEULORLOG   = -0.5495393129816448223L;
const long double N_EGOLDRATIO  =  1.618033988749894848L;

const long double N_EDEGPERRAD  = 57.2957795130823209L;
const long double N_ERADPERDEG  =  0.0174532925199432957L;

#ifdef __BORLANDC__
#pragma warn .use
#endif


