//=============================================================
//  Binary Tree Classes     treex.h     v1.00
//      Exception types for binary trees
//      Copyright 1993-1995 Scott Robert Ladd
//=============================================================

#include "treex.h"

void TreeEx::Explain(DiagOutput & out)
    {
    switch (Error)
        {
        case BTX_ALLOC:
            out.DisplayMsg("Can't allocate memory for binary tree",
                            DIAG_ERROR);
            break;
        case BTX_NOTFOUND:
            out.DisplayMsg("Binary tree look-up failed",
                            DIAG_WARNING);
            break;
        case BTX_LOCKVIOLATION:
            out.DisplayMsg("Attempted change in locked binary tree",
                            DIAG_WARNING);
            break;
        case BTX_LOCKMAX:
            out.DisplayMsg("Too many lock increments for binary tree",
                            DIAG_WARNING);
            break;
        case BTX_LOCKZERO:
            out.DisplayMsg("Too many lock decrements for binary tree",
                            DIAG_WARNING);
            break;
        default:
            out.DisplayMsg("Unknown binary tree exception",
                            DIAG_FATAL);
        }
    }


