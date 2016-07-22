//==============================================================
// Diagnostic & Exception classes   diagdos.h       v1.00
//      Classes for diagnostics and exceptions under Windows
//      Copyright 1995 Scott Robert Ladd
//==============================================================

#ifndef DIAGWIN_H
#define DIAGWIN_H

#include "diagnose.h"
#include "windows.h"
#include "string.h"
#include "stdlib.h"

class DiagOutWin : public DiagOutput
        {
        public:
        DiagOutWin
            (
            const char * t
            );

        ~DiagOutWin();

                virtual void DisplayMsg
            (
            const char * msg,
            DiagLevel level = DIAG_MSG
            );

    public:
        char * Title;
        };

inline DiagOutWin::DiagOutWin
    (
    const char * t
    )
    {
    Title = strdup(t);
    }

inline DiagOutWin::~DiagOutWin()
    {
    if (Title != NULL)
        free(Title);
    }

#endif
