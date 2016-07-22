//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      pdoptcfg.cpp     v1.00
//
//      A class for configuring Prisoner's Dilemma simulation
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include "forgeres.h"
#include "pdoptcfg.h"
#include "commdlg.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

size_t PDOptConfig::DefPopSize       =  128;
size_t PDOptConfig::DefTestSize      =  200;
size_t PDOptConfig::DefReptFreq      =   10;
size_t PDOptConfig::DefStartLvl      =    1;
size_t PDOptConfig::DefMaxLevel      =    2;
bool   PDOptConfig::DefCrossover     = true;
float  PDOptConfig::DefCrossRate     = 0.0100F;
bool   PDOptConfig::DefMutation      = true;
float  PDOptConfig::DefMuteRate      = 0.0100F;
bool   PDOptConfig::DefDoubling      = false;
float  PDOptConfig::DefDblRate       = 0.0001F;
bool   PDOptConfig::DefAveraging     = false;
bool   PDOptConfig::DefFairStart     = false;
bool   PDOptConfig::DefRandom1st     = false;
double PDOptConfig::DefPayoffDD      = 1.0000;
double PDOptConfig::DefPayoffDC      = 5.0000;
double PDOptConfig::DefPayoffCD      = 0.0000;
double PDOptConfig::DefPayoffCC      = 3.0000;
bool   PDOptConfig::DefFileOut       = false;
char   PDOptConfig::DefFileName[256] = "pdoutput.txt\0";

static LPCSTR PDOptConfigDlgName = MAKEINTRESOURCE(DLG_PDOPTCFG);

static const char * PDOptConfigDlgErr =
    "Population and Generations must be greater than 9.\n"
    "Frequency must be greater than 0 and less than generations.\n"
    "Starting level muct be between 1 and 3.\n"
    "Maximum level must be between 2 and 4.\n"
    "Fair Start is incompatible with Starting Level > 2\n"
    "Percentages must be between 0 and 100%.";

BOOL CALLBACK PDOptConfigDlgProc
    (
    HWND  dlg,
    WORD  message,
    WORD  wParam,
    DWORD lParam
    );

PDOptConfig::PDOptConfig
    (
    HINSTANCE inst,
    HWND      parent
    )
    {
    int res;

    #ifdef _WIN32
        res = DialogBox(inst,PDOptConfigDlgName,parent,FARPROC(PDOptConfigDlgProc));
    #else
        DLGPROC thunk   = MakeProcInstance(FARPROC(PDOptConfigDlgProc),inst);
        res = DialogBox(inst,PDOptConfigDlgName,parent,thunk);
        FreeProcInstance(thunk);
    #endif

    if (res)
        Valid = true;
    else
        Valid = false;

    PopSize   = DefPopSize;
    TestSize  = DefTestSize;
    ReptFreq  = DefReptFreq;
    StartLvl  = DefStartLvl;
    MaxLevel  = DefMaxLevel;
    Crossover = DefCrossover;
    CrossRate = DefCrossRate;
    Mutation  = DefMutation;
    MuteRate  = DefMuteRate;
    Doubling  = DefDoubling;
    DblRate   = DefDblRate;
    Averaging = DefAveraging;
    FairStart = DefFairStart;
    Random1st = DefRandom1st;
    PayoffDD  = DefPayoffDD;
    PayoffDC  = DefPayoffDC;
    PayoffCD  = DefPayoffCD;
    PayoffCC  = DefPayoffCC;
    FileOut   = DefFileOut;
    memcpy(FileName,DefFileName,256);
    }

#ifdef __BORLANDC__
#pragma argsused
#endif

BOOL CALLBACK PDOptConfigDlgProc
    (
    HWND  dlg,
    WORD  message,
    WORD  wParam,
    DWORD lParam
    )
    {
    switch (message)
        {
        case WM_INITDIALOG:
            PDOptConfig::DlgInit(dlg);
            return TRUE;

        case WM_COMMAND:
            switch (wParam)
                {
                case IDD_PDOPTCFG_SETFILE:
                    PDOptConfig::DlgSetFileName(dlg);
                    break;

                case IDOK:
                    if (TRUE == PDOptConfig::DlgStore(dlg))
                        EndDialog(dlg, 1);

                    break;

                case IDCANCEL:
                    EndDialog(dlg, 0);
                }

            return TRUE;
        }

    return FALSE;
    }

void PDOptConfig::DlgSetFileName
    (
    HWND  dlg
    )
    {
    static const size_t MaxName = 128;

    static const char * filters[] =
        {
        "Test files", "*.txt",
        "All files",  "*.*\x00"
        };

    // common dialog box for save
    char fname[MaxName];
    char ftitle[MaxName];
    OPENFILENAME ofn;

    // initialize names
    GetDlgItemText(dlg,IDD_PDOPTCFG_FNAME,fname,MaxName);
    ftitle[0] = 0;

    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = dlg;
    ofn.hInstance         = NULL;
    ofn.lpstrFilter       = filters[0];
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter    = 0;
    ofn.nFilterIndex      = 0;
    ofn.lpstrFile         = fname;
    ofn.nMaxFile          = MaxName;
    ofn.lpstrFileTitle    = ftitle;
    ofn.nMaxFileTitle     = MaxName;
    ofn.lpstrInitialDir   = NULL; // current directory
    ofn.lpstrTitle        = NULL; // default title
    ofn.nFileOffset       = 0;
    ofn.nFileExtension    = 0;
    ofn.lpstrDefExt       = "txt";
    ofn.lCustData         = NULL;
    ofn.lpfnHook          = NULL;
    ofn.lpTemplateName    = NULL;
    ofn.Flags             = OFN_PATHMUSTEXIST |
                            OFN_FILEMUSTEXIST |
                            OFN_OVERWRITEPROMPT |
                            OFN_HIDEREADONLY;

    if (FALSE == GetSaveFileName(&ofn))
        return;

    AnsiLower(fname);
    AnsiLower(ftitle);

    SetDlgItemText(dlg,IDD_PDOPTCFG_FNAME,fname);
    }

void PDOptConfig::DlgInit
    (
    HWND  dlg
    )
    {
    char temp[64];

    sprintf(temp,"%u",DefPopSize);
    SetDlgItemText(dlg,IDD_PDOPTCFG_POP,temp);

    sprintf(temp,"%u",DefTestSize);
    SetDlgItemText(dlg,IDD_PDOPTCFG_GEN,temp);

    sprintf(temp,"%u",DefReptFreq);
    SetDlgItemText(dlg,IDD_PDOPTCFG_FREQ,temp);

    sprintf(temp,"%u",DefStartLvl);
    SetDlgItemText(dlg,IDD_PDOPTCFG_START,temp);

    sprintf(temp,"%u",DefMaxLevel);
    SetDlgItemText(dlg,IDD_PDOPTCFG_MAXLVL,temp);

    if (DefCrossover)
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_CROSS,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_CROSS,BM_SETCHECK,0,0L);

    sprintf(temp,"%.6g",DefCrossRate * 100.0F);
    SetDlgItemText(dlg,IDD_PDOPTCFG_CRATE,temp);

    if (DefMutation)
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_MUTE,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_MUTE,BM_SETCHECK,0,0L);

    sprintf(temp,"%.6g",DefMuteRate * 100.0F);
    SetDlgItemText(dlg,IDD_PDOPTCFG_MRATE,temp);

    if (DefDoubling)
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_DBLING,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_DBLING,BM_SETCHECK,0,0L);

    sprintf(temp,"%.6g",DefDblRate * 100.0F);
    SetDlgItemText(dlg,IDD_PDOPTCFG_DRATE,temp);

    if (DefAveraging)
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_AVG,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_AVG,BM_SETCHECK,0,0L);

    if (DefFairStart)
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_FAIR,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_FAIR,BM_SETCHECK,0,0L);

    if (DefRandom1st)
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_RAND1,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_RAND1,BM_SETCHECK,0,0L);

    sprintf(temp,"%.15g",DefPayoffDD);
    SetDlgItemText(dlg,IDD_PDOPTCFG_PAYDD,temp);

    sprintf(temp,"%.15g",DefPayoffDC);
    SetDlgItemText(dlg,IDD_PDOPTCFG_PAYDC,temp);

    sprintf(temp,"%.15g",DefPayoffCD);
    SetDlgItemText(dlg,IDD_PDOPTCFG_PAYCD,temp);

    sprintf(temp,"%.15g",DefPayoffCC);
    SetDlgItemText(dlg,IDD_PDOPTCFG_PAYCC,temp);

    if (DefFileOut)
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_FILE,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_PDOPTCFG_FILE,BM_SETCHECK,0,0L);

    SetDlgItemText(dlg,IDD_PDOPTCFG_FNAME,DefFileName);
    }

BOOL PDOptConfig::DlgStore
    (
    HWND  dlg
    )
    {
    char temp[64];

    GetDlgItemText(dlg,IDD_PDOPTCFG_POP,temp,64);
    size_t psz = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_PDOPTCFG_GEN,temp,64);
    size_t gen = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_PDOPTCFG_FREQ,temp,64);
    size_t frq = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_PDOPTCFG_START,temp,64);
    size_t slvl = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_PDOPTCFG_MAXLVL,temp,64);
    size_t mlvl = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_PDOPTCFG_CRATE,temp,64);
    float crate = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_PDOPTCFG_MRATE,temp,64);
    float mrate = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_PDOPTCFG_DRATE,temp,64);
    float drate = float(fabs(atof(temp)));

    bool fair = false;
    if (SendDlgItemMessage(dlg,IDD_PDOPTCFG_FAIR,BM_GETCHECK,0,0L))
        fair = true;

    if ((psz < 10)
    ||  (gen < 10)
    ||  (frq <  1)
    ||  (frq > gen)
    ||  (slvl < 1)
    ||  (slvl > 3)
    ||  (fair && (slvl > 2))
    ||  (mlvl < 2)
    ||  (mlvl > 4)
    ||  (mrate > 100.0F)
    ||  (crate > 100.0F)
    ||  (drate > 100.0F))
        {
        MessageBeep(MB_ICONHAND);
        MessageBox(NULL,PDOptConfigDlgErr,"Configuration Error",MB_OK | MB_ICONHAND);
        return FALSE;
        }

    DefPopSize   = psz;
    DefTestSize  = gen;
    DefReptFreq  = frq;
    DefStartLvl  = slvl;
    DefMaxLevel  = mlvl;
    DefCrossRate = crate / 100.0F;
    DefMuteRate  = mrate / 100.0F;
    DefDblRate   = drate / 100.0F;
    DefFairStart = fair;

    if (SendDlgItemMessage(dlg,IDD_PDOPTCFG_CROSS,BM_GETCHECK,0,0L))
        DefCrossover = true;
    else
        DefCrossover = false;

    if (SendDlgItemMessage(dlg,IDD_PDOPTCFG_MUTE,BM_GETCHECK,0,0L))
        DefMutation = true;
    else
        DefMutation = false;

    if (SendDlgItemMessage(dlg,IDD_PDOPTCFG_DBLING,BM_GETCHECK,0,0L))
        DefDoubling = true;
    else
        DefDoubling = false;

    if (SendDlgItemMessage(dlg,IDD_PDOPTCFG_AVG,BM_GETCHECK,0,0L))
        DefAveraging = true;
    else
        DefAveraging = false;

    if (SendDlgItemMessage(dlg,IDD_PDOPTCFG_RAND1,BM_GETCHECK,0,0L))
        DefRandom1st = true;
    else
        DefRandom1st = false;

    GetDlgItemText(dlg,IDD_PDOPTCFG_PAYDD,temp,64);
    DefPayoffDD = fabs(atof(temp));

    GetDlgItemText(dlg,IDD_PDOPTCFG_PAYDC,temp,64);
    DefPayoffDC = fabs(atof(temp));

    GetDlgItemText(dlg,IDD_PDOPTCFG_PAYCD,temp,64);
    DefPayoffCD = fabs(atof(temp));

    GetDlgItemText(dlg,IDD_PDOPTCFG_PAYCC,temp,64);
    DefPayoffCC = fabs(atof(temp));

    if (SendDlgItemMessage(dlg,IDD_PDOPTCFG_FILE,BM_GETCHECK,0,0L))
        DefFileOut = true;
    else
        DefFileOut = false;

    GetDlgItemText(dlg,IDD_PDOPTCFG_FNAME,DefFileName,256);

    return TRUE;
    }
