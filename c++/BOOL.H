//-----------------------------------------------------------
//  Standard C++ Library
//-----------------------------------------------------------
//
//      bool.h     v1.00
//
//      Defines a rough equivalent of the ANSI C++ proposed
//      "bool" boolean type.
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#ifndef CPPLIB_BOOL_H
#define CPPLIB_BOOL_H

#include "iostream.h"

enum { false, true };

class bool
    {
    public:
        // constructors
        bool(int b = 0) { Val = (b ? true : false); }

        // assignment
        bool operator = (bool b) { return (Val = b.Val); }

        // conversions
        operator int() const { return Val; }

        // comparisons
        bool operator == (bool b) const { return Val == b.Val; }
        bool operator != (bool b) const { return Val != b.Val; }

        // stream output operator
        friend ostream & operator << (ostream & os, bool b);

    private:
        int Val;
    };

inline ostream & operator << (ostream & os, bool b)
    {
    os << (b ? "true" : "false");
    return os;
    }

#endif

