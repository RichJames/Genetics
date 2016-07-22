//=============================================================
//  Binary Tree Classes     treex.h     v1.00
//      Exception types for binary trees
//      Copyright 1993-1995 Scott Robert Ladd
//=============================================================

#ifndef TREEX_H
#define TREEX_H

#include "diagnose.h"

enum TreeError
    {
    BTX_ALLOC,
    BTX_NOTFOUND,
    BTX_LOCKVIOLATION,
    BTX_LOCKMAX,
    BTX_LOCKZERO
    };

class TreeEx : public ExceptionBase
    {
    public:
        TreeEx
            (
            TreeError err
            )
            {
            Error = err;
            }

        TreeError WhatsWrong()
            {
            return Error;
            }

        virtual void Explain
            (
            DiagOutput & out
            );

    private:
        TreeError Error;
    };

#endif
