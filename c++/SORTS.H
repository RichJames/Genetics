//-----------------------------------------------------------
//  General Purpose Tools
//-----------------------------------------------------------
//
//      sorts.h           v1.00
//
//      A template defining simple sorting tools
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#ifndef SORTS_H
#define SORTS_H

//---------------
// sort two items
//---------------

template
    <
    class  T
    >
    inline void SortTwo
        (
        T & a,
        T & b
        )
        {
        if (a > b)
            {
            T t = a;
            a = b;
            b = t;
            }
        }

//-----------------
// sort three items
//-----------------

template
    <
    class  T
    >
    inline void SortThree
        (
        T & a,
        T & b,
        T & c
        )
        {
        SortTwo(a,b);
        SortTwo(a,c);
        SortTwo(b,c);
        }

//---------------------------------------
// shell sort an array in ascending order
//---------------------------------------

template
    <
    class  T
    >
    void ShellSortAscending
        (
        T * a,
        size_t n
        )
        {
        size_t inc, i, j;
        T t;

        // algorithm relies on one-based arrays
        --a;

        for (inc = 1; inc <= n / 9; inc = 3 * inc + 1) ;

        for ( ; inc > 0; inc /= 3)
            {
            for (i = inc + 1; i <= n; i += inc)
                {
                t = a[i];
                j = i;

                while ((j > inc) && (a[j - inc] > t))
                    {
                    a[j] = a[j - inc];
                    j -= inc;
                    }

                a[j] = t;
                }
            }
        }

//----------------------------------------
// shell sort an array in descending order
//----------------------------------------

template
    <
    class  T
    >
    void ShellSortDescending
        (
        T * a,
        size_t n
        )
        {
        size_t inc, i, j;
        T t;

        // algorithm relies on one-based arrays
        --a;

        for (inc = 1; inc <= n / 9; inc = 3 * inc + 1) ;

        for ( ; inc > 0; inc /= 3)
            {
            for (i = inc + 1; i <= n; i += inc)
                {
                t = a[i];
                j = i;

                while ((j > inc) && (a[j - inc] < t))
                    {
                    a[j] = a[j - inc];
                    j -= inc;
                    }

                a[j] = t;
                }
            }
        }

//--------------------------------------------------
// binary search an ascending array, returning index
//--------------------------------------------------

class BinSearchEx { };

template
    <
    class  T
    >
    size_t BinSearch
        (
        T target,
        const T * a,
        size_t n
        )
        {
        // assumes that array is sorted in ascending order!
        size_t l = 0;
        size_t r = n - 1;
        size_t i;

        while (r >= l)
            {
            i = (l + r) / 2;

            if (a[i] == target)
                return i;

            if (target < a[i])
                r = i - 1;
            else
                l = i + 1;
            }

        throw BinSearchEx();

        #ifdef _MSC_VER
            return 0;
        #endif
        }

//----------------------------------------------------
// binary search an ascending array, returning boolean
//----------------------------------------------------

template
    <
    class  T
    >
    bool BinFind
        (
        T target,
        const T * a,
        size_t n
        )
        {
        // assumes that array is sorted in ascending order!
        size_t l = 0;
        size_t r = n - 1;
        size_t i;

        while (r >= l)
            {
            i = (l + r) / 2;

            if (a[i] == target)
                return true;

            if (target < a[i])
                r = i - 1;
            else
                l = i + 1;
            }

        return false;
        }

#endif

