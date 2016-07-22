//-------------------------------------
// ALife -- An Artificial Life Universe
//-------------------------------------
//
//      fontutil.cpp    v1.00
//
//      Defines some useful font functions.
//
//-------------------------------------------------------
// Copyright 1995 Scott Robert Ladd. All Rights Reserved.
//-------------------------------------------------------

#include "fontutil.h"
#include "commdlg.h"
#include "stdio.h"
#include "string.h"

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
    )
    {
    int n;
    static char buffer[1024];

    #ifdef __BORLANDC__
        n = vsprintf(buffer,fmt,(void *)(&fmt + 1));
    #else
        n = vsprintf(buffer,fmt,(char *)(&fmt + 1));
    #endif

    TextOut(dc,x,y,buffer,n);
    }

//---------------------------------------------------
// function: ClearLogFont
//      Assigns "blank" values to a LOGFONT structure
//---------------------------------------------------

void ClearLogFont
    (
    LOGFONT & lf
    )
    {
    lf.lfHeight         = 0;
    lf.lfWidth          = 0;
    lf.lfEscapement     = 0;
    lf.lfOrientation    = 0;
    lf.lfWeight         = FW_NORMAL;
    lf.lfItalic         = 0;
    lf.lfUnderline      = 0;
    lf.lfStrikeOut      = 0;
    lf.lfCharSet        = DEFAULT_CHARSET;
    lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
    lf.lfQuality        = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    lf.lfFaceName[0]    = 0;
    }

//----------------------------------------
//  class: Font
//      Defines a class for handling fonts
//----------------------------------------

// internal copy functions
void Font::Copy
    (
    const Font & f
    )
    {
    LFont = f.LFont;

    if (f.HFont != NULL)
        {
        HDC dc;

        dc = GetDC(NULL);
        HFont = CreateFontIndirect(&LFont);
        ReleaseDC(NULL,dc);
        }
    }

// internal "set to font" function
void Font::SetTo
    (
    char * name,
    int    pts,
    BOOL   bold,
    BOOL   ital,
    HDC    dc
    )
    {
    BOOL delDC = FALSE;

    if (name == NULL)
        return;

    if (dc == NULL)
        {
        dc = GetDC(NULL);
        delDC = TRUE;
        }

    ClearLogFont(LFont);

    strncpy(LFont.lfFaceName,name,LF_FACESIZE - 1);

    LFont.lfHeight   = PtsToPixels(dc,pts);

    if (bold)
        LFont.lfWeight = FW_BOLD;

    if (ital)
        LFont.lfItalic = 1;

    HFont = CreateFontIndirect(&LFont);

    if (delDC)
        ReleaseDC(NULL,dc);
    }

// get font from dialog box
void Font::FontDialog()
    {
    CHOOSEFONT cf;

    cf.lStructSize    = sizeof(CHOOSEFONT);
    cf.hwndOwner      = NULL;
    cf.hDC            = NULL;
    cf.lpLogFont      = &LFont;
    cf.iPointSize     = 0;
    cf.Flags          = CF_FORCEFONTEXIST | CF_INITTOLOGFONTSTRUCT;
    cf.rgbColors      = 0;
    cf.lCustData      = 0;
    cf.lpfnHook       = NULL;
    cf.lpTemplateName = NULL;
    cf.hInstance      = NULL;
    cf.lpszStyle      = NULL;
    cf.nFontType      = 0;
    cf.nSizeMin       = 0;
    cf.nSizeMax       = 0;

    ChooseFont(&cf);
    }

// uses the "select font" common dialog box
Font::Font()
    {
    HFont = NULL;
    FontDialog();
    HFont = CreateFontIndirect(&LFont);
    }

// creates a specific font in a given size
Font::Font
    (
    char * name,
    int    pts,
    BOOL   bold,
    BOOL   ital,
    HDC    dc
    )
    {
    SetTo(name,pts,bold,ital,dc);
    }

// copy constructor
Font::Font
    (
    const Font & f
    )
    {
    Copy(f);
    }

// destructor
Font::~Font()
    {
    if (HFont != NULL)
        DeleteObject(HFont);
    }

// assignment
void Font::operator =
    (
    const Font & f
    )
    {
    if (HFont != NULL)
        DeleteObject(HFont);

    Copy(f);
    }

// use common dialog to change font
void Font::Change()
    {
    if (HFont != NULL)
        DeleteObject(HFont);

    FontDialog();

    HFont = CreateFontIndirect(&LFont);
    }

void Font::Change
    (
    char * name,
    int    pts,
    BOOL   bold,
    BOOL   ital,
    HDC    dc
    )
    {
    if (HFont != NULL)
        DeleteObject(HFont);

    SetTo(name,pts,bold,ital,dc);
    }

