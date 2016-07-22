//-------------------------------------
// ALife -- An Artificial Life Universe
//-------------------------------------
//
//      fontutil.h      v1.00
//
//      Defines some useful font functions.
//
//-------------------------------------------------------
// Copyright 1995 Scott Robert Ladd. All Rights Reserved.
//-------------------------------------------------------

#ifndef AL_FONTUTIL_H
#define AL_FONTUTIL_H

#include "windows.h"
#include "stddef.h"

//------------------------------------------------------
// function: PtsToPixels
//      Converts a given number of points to a number of
//      pixels for the specified context.
//------------------------------------------------------

inline int PtsToPixels
    (
    HDC dc,
    int pts
    )
    {
    return MulDiv(-(pts),GetDeviceCaps(dc,LOGPIXELSY),72);
    }

//-----------------------------------------------------------
// function: PrintAt
//      Draws a piece of text at a specific location in a DC.
//      Acts as a shell for a "printf" call.
//-----------------------------------------------------------

void PrintAt
    (
    HDC dc,
    int x,
    int y,
    LPSTR fmt,
    ...
    );

//---------------------------------------------------
// function: ClearLogFont
//      Assigns "blank" values to a LOGFONT structure
//---------------------------------------------------

void ClearLogFont
    (
    LOGFONT & lf
    );

//----------------------------------------
//  class: Font
//      Defines a class for handling fonts
//----------------------------------------

class Font
    {
    public:
        // uses the "select font" common dialog box
        Font();

        // creates a specific font in a given size
        Font
            (
            char * name,
            int    pts,
            BOOL   bold,
            BOOL   ital,
            HDC    dc = NULL
            );

        // copy constructor
        Font
            (
            const Font & f
            );

        // destructor
        ~Font();

        // assignment
        void operator =
            (
            const Font & f
            );

        // use common dialog to change font
        void Change();

        // change to specific font
        void Change
            (
            char * name,
            int    pts,
            BOOL   bold,
            BOOL   ital,
            HDC    dc
            );

        // return the Windows font handle
        HFONT GetFont();

    private:
        LOGFONT LFont;
        HFONT   HFont;

        void Font::Copy
            (
            const Font & f
            );

        void Font::SetTo
            (
            char * name,
            int    pts,
            BOOL   bold,
            BOOL   ital,
            HDC    dc
            );

        void FontDialog();
    };

// return the Windows font handle
inline HFONT Font::GetFont()
    {
    return HFont;
    }

#endif

