//==============================================================
//  MATHEMATICS LIBRARY     fpconst.h   v1.00
//
//      Definitions of various mathematical constants. These
//      constants assume that float is an IEEE single, that
//      double is an IEEE double, and that long double is
//      the 80-bit extended IEEE double used as a temporary
//      type by the 80x87 math coprocessors.
//
//      Copyright 1991-1995 Scott Robert Ladd.
//==============================================================

#ifndef FPCONST_H
#define FPCONST_H

// float constants
extern const float N_FPI;         // pi
extern const float N_F1PI;        // 1 / pi
extern const float N_F2PI;        // 2 / pi
extern const float N_FPISQR;      // pi squared
extern const float N_FPISQRT;     // square root pi
extern const float N_FPI2;        // pi / 2
extern const float N_FPI3;        // pi / 3
extern const float N_FPI4;        // pi / 4
extern const float N_FPILOG;      // log(pi)
extern const float N_FPILOG10;    // log10(pi)

extern const float N_FE;          // e
extern const float N_F1E;         // 1 / e
extern const float N_FESQR;       // e squared
extern const float N_FLOG10;      // log(10)
extern const float N_FLOG2;       // log(2)
extern const float N_FLOG10E;     // log10(e)
extern const float N_FLOG10SQE;   // log10(sqrt(e))
extern const float N_FLOG102;     // log10(2)

extern const float N_FSQRT2;      // sqrt(2)
extern const float N_FEULER;      // Euler's number
extern const float N_FEULORLOG;   // log(Euler)
extern const float N_FGOLDRATIO;  // Golden ratio

extern const float N_FDEGPERRAD;  // 180 / pi
extern const float N_FRADPERDEG;  // pi / 180

// double constants

extern const double N_DPI;         // pi
extern const double N_D1PI;        // 1 / pi
extern const double N_D2PI;        // 2 / pi
extern const double N_DPISQR;      // pi squared
extern const double N_DPISQRT;     // square root pi
extern const double N_DPI2;        // pi / 2
extern const double N_DPI3;        // pi / 3
extern const double N_DPI4;        // pi / 4
extern const double N_DPILOG;      // log(pi)
extern const double N_DPILOG10;    // log10(pi)

extern const double N_DE;          // e
extern const double N_D1E;         // 1 / e
extern const double N_DESQR;       // e squared
extern const double N_DLOG10;      // log(10)
extern const double N_DLOG2;       // log(2)
extern const double N_DLOG10E;     // log10(e)
extern const double N_DLOG10SQE;   // log10(sqrt(e))
extern const double N_DLOG102;     // log10(2)

extern const double N_DSQRT2;      // sqrt(2)
extern const double N_DEULER;      // Euler's number
extern const double N_DEULORLOG;   // log(Euler)
extern const double N_DGOLDRATIO;  // Golden ratio

extern const double N_DDEGPERRAD;  // 180 / pi
extern const double N_DRADPERDEG;  // pi / 180

// long double constants

extern const long double N_EPI;         // pi
extern const long double N_E1PI;        // 1 / pi
extern const long double N_E2PI;        // 2 / pi
extern const long double N_EPISQR;      // pi squared
extern const long double N_EPISQRT;     // square root pi
extern const long double N_EPI2;        // pi / 2
extern const long double N_EPI3;        // pi / 3
extern const long double N_EPI4;        // pi / 4
extern const long double N_EPILOG;      // log(pi)
extern const long double N_EPILOG10;    // log10(pi)

extern const long double N_EE;          // e
extern const long double N_E1E;         // 1 / e
extern const long double N_EESQR;       // e squared
extern const long double N_ELOG10;      // log(10)
extern const long double N_ELOG2;       // log(2)
extern const long double N_ELOG10E;     // log10(e)
extern const long double N_ELOG10SQE;   // log10(sqrt(e))
extern const long double N_ELOG102;     // log10(2)

extern const long double N_ESQRT2;      // sqrt(2)
extern const long double N_EEULER;      // Euler's number
extern const long double N_EEULORLOG;   // log(Euler)
extern const long double N_EGOLDRATIO;  // Golden ratio

extern const long double N_EDEGPERRAD;  // 180 / pi
extern const long double N_ERADPERDEG;  // pi / 180

#endif
