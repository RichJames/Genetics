//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      gaoptcfg.cpp   v1.00
//
//      A class for genetic algorithm optimization
//      configuration
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include "forgeres.h"
#include "gaoptcfg.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

size_t  GAOptConfig::DefPopSize    =   50;
size_t  GAOptConfig::DefTestSize   =  101;
size_t  GAOptConfig::DefReptFreq   =    1;
size_t  GAOptConfig::DefSigDigits  =    8;
double  GAOptConfig::DefXMin       =  -10.0;
double  GAOptConfig::DefXMax       =   10.0;
double  GAOptConfig::DefYMin       =  -10.0;
double  GAOptConfig::DefYMax       =   10.0;
double  GAOptConfig::DefFitLinBase =  100.0;
double  GAOptConfig::DefFitLinDec  =   10.0;
double  GAOptConfig::DefFitLinMin  =   10.0;
float   GAOptConfig::DefWtSign     =    2.0F;
float   GAOptConfig::DefWtExp      =   13.0F;
float   GAOptConfig::DefWtMant     =   85.0F;
float   GAOptConfig::DefMuteRate   =    1.0F;
float   GAOptConfig::DefCrossRate  =    1.0F;
bool    GAOptConfig::DefCrossX     = true;
bool    GAOptConfig::DefCrossY     = true;
bool    GAOptConfig::DefCrossB     = true;
bool    GAOptConfig::DefMutateX    = true;
bool    GAOptConfig::DefMutateY    = true;
bool    GAOptConfig::DefMuteLoop   = false;
bool    GAOptConfig::DefElitist    = true;
bool    GAOptConfig::DefFitScale   = true;
FSAlgor GAOptConfig::DefFitAlgor   = FSA_WINDOW;
size_t  GAOptConfig::DefEquation   = 3;

static LPCSTR GAOptConfigDlgName = MAKEINTRESOURCE(DLG_GAOPTCFG);

static const char * GAOptConfigDlgErr =
    "Population and Generations must be greater than 9.\n"
    "Frequency must be greater than 0 and less than generations.\n"
    "Significant Digits must be between 1 and 15.\n"
    "Minimum values must be < maximums.\n"
    "Mutation and Crossover rates must be between 0 and 100%.\n"
    "With mutation looping, mutation rate must be < 100%\n"
    "Sum of mutation weights must be > 1.";

BOOL CALLBACK GAOptConfigDlgProc
    (
    HWND  dlg,
    WORD  message,
    WORD  wParam,
    DWORD lParam
    );

GAOptConfig::GAOptConfig
    (
    HINSTANCE inst,
    HWND      parent
    )
    {
    int res;

    #ifdef _WIN32
        res = DialogBox(inst,GAOptConfigDlgName,parent,FARPROC(GAOptConfigDlgProc));
    #else
        DLGPROC thunk   = MakeProcInstance(FARPROC(GAOptConfigDlgProc),inst);
        res = DialogBox(inst,GAOptConfigDlgName,parent,thunk);
        FreeProcInstance(thunk);
    #endif

    if (res)
        Valid = true;
    else
        Valid = false;

    PopSize    = DefPopSize;
    TestSize   = DefTestSize;
    ReptFreq   = DefReptFreq;
    SigDigits  = DefSigDigits;
    XMin       = DefXMin;
    XMax       = DefXMax;
    YMin       = DefYMin;
    YMax       = DefYMax;
    CrossRate  = DefCrossRate;
    CrossX     = DefCrossX;
    CrossY     = DefCrossY;
    CrossB     = DefCrossB;
    WtSign     = DefWtSign;
    WtExp      = DefWtExp;
    WtMant     = DefWtMant;
    MuteRate   = DefMuteRate;
    MutateX    = DefMutateX;
    MutateY    = DefMutateY;
    MuteLoop   = DefMuteLoop;
    Elitist    = DefElitist;
    FitScale   = DefFitScale;
    FitAlgor   = DefFitAlgor;
    FitLinBase = DefFitLinBase;
    FitLinDec  = DefFitLinDec;
    FitLinMin  = DefFitLinMin;
    Equation   = DefEquation;
    }

#ifdef __BORLANDC__
#pragma argsused
#endif

BOOL CALLBACK GAOptConfigDlgProc
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
            GAOptConfig::DlgInit(dlg);
            return TRUE;

        case WM_COMMAND:
            switch (wParam)
                {
                case IDOK:
                    if (TRUE == GAOptConfig::DlgStore(dlg))
                        EndDialog(dlg, 1);

                    break;

                case IDCANCEL:
                    EndDialog(dlg, 0);
                }

            return TRUE;
        }

    return FALSE;
    }

void GAOptConfig::DlgInit
    (
    HWND  dlg
    )
    {
    char temp[64];

    sprintf(temp,"%u",DefPopSize);
    SetDlgItemText(dlg,IDD_GAOPTCFG_POP,temp);

    sprintf(temp,"%u",DefTestSize);
    SetDlgItemText(dlg,IDD_GAOPTCFG_GEN,temp);

    sprintf(temp,"%u",DefReptFreq);
    SetDlgItemText(dlg,IDD_GAOPTCFG_FREQ,temp);

    sprintf(temp,"%u",DefSigDigits);
    SetDlgItemText(dlg,IDD_GAOPTCFG_SIGDIG,temp);

    sprintf(temp,"%.15g",DefXMin);
    SetDlgItemText(dlg,IDD_GAOPTCFG_XMIN,temp);

    sprintf(temp,"%.15g",DefXMax);
    SetDlgItemText(dlg,IDD_GAOPTCFG_XMAX,temp);

    sprintf(temp,"%.15g",DefYMin);
    SetDlgItemText(dlg,IDD_GAOPTCFG_YMIN,temp);

    sprintf(temp,"%.15g",DefYMax);
    SetDlgItemText(dlg,IDD_GAOPTCFG_YMAX,temp);

    sprintf(temp,"%.6g",DefCrossRate * 100.0F);
    SetDlgItemText(dlg,IDD_GAOPTCFG_CPROB,temp);

    if (DefCrossX)
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_CROSSX,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_CROSSX,BM_SETCHECK,0,0L);

    if (DefCrossY)
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_CROSSY,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_CROSSY,BM_SETCHECK,0,0L);

    if (DefCrossB)
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_CROSSB,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_CROSSB,BM_SETCHECK,0,0L);

    sprintf(temp,"%.6g",DefWtSign);
    SetDlgItemText(dlg,IDD_GAOPTCFG_MUTES,temp);

    sprintf(temp,"%.6g",DefWtExp);
    SetDlgItemText(dlg,IDD_GAOPTCFG_MUTEE,temp);

    sprintf(temp,"%.6g",DefWtMant);
    SetDlgItemText(dlg,IDD_GAOPTCFG_MUTEM,temp);

    sprintf(temp,"%.6g",DefMuteRate * 100.0F);
    SetDlgItemText(dlg,IDD_GAOPTCFG_MPROB,temp);

    if (DefMutateX)
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_MUTEX,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_MUTEX,BM_SETCHECK,0,0L);

    if (DefMutateY)
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_MUTEY,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_MUTEY,BM_SETCHECK,0,0L);

    if (DefMuteLoop)
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_MLOOP,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_MLOOP,BM_SETCHECK,0,0L);

    if (DefElitist)
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_ELITE,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_ELITE,BM_SETCHECK,0,0L);

    if (DefFitScale)
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_FSCALE,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_GAOPTCFG_FSCALE,BM_SETCHECK,0,0L);

    switch (DefFitAlgor)
        {
        case FSA_EXPON:
            SendDlgItemMessage(dlg,IDD_GAOPTCFG_FTEXP,BM_SETCHECK,1,0L);
            break;
        case FSA_WINDOW:
            SendDlgItemMessage(dlg,IDD_GAOPTCFG_FTWDW,BM_SETCHECK,1,0L);
            break;
        case FSA_LINEAR:
            SendDlgItemMessage(dlg,IDD_GAOPTCFG_FTLIN,BM_SETCHECK,1,0L);
        }

    sprintf(temp,"%.15g",DefFitLinBase);
    SetDlgItemText(dlg,IDD_GAOPTCFG_LNBASE,temp);

    sprintf(temp,"%.15g",DefFitLinDec);
    SetDlgItemText(dlg,IDD_GAOPTCFG_LNDEC,temp);

    sprintf(temp,"%.15g",DefFitLinMin);
    SetDlgItemText(dlg,IDD_GAOPTCFG_LNMIN,temp);

    switch (DefEquation)
        {
        case 0:
            SendDlgItemMessage(dlg,IDD_GAOPTCFG_EQF6,BM_SETCHECK,1,0L);
            break;
        case 1:
            SendDlgItemMessage(dlg,IDD_GAOPTCFG_EQF7,BM_SETCHECK,1,0L);
            break;
        case 2:
            SendDlgItemMessage(dlg,IDD_GAOPTCFG_EQF8,BM_SETCHECK,1,0L);
            break;
        case 3:
            SendDlgItemMessage(dlg,IDD_GAOPTCFG_EQCUST,BM_SETCHECK,1,0L);
        }
    }

BOOL GAOptConfig::DlgStore
    (
    HWND  dlg
    )
    {
    char temp[64];

    GetDlgItemText(dlg,IDD_GAOPTCFG_POP,temp,64);
    size_t psz = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_GAOPTCFG_GEN,temp,64);
    size_t gen = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_GAOPTCFG_FREQ,temp,64);
    size_t frq = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_GAOPTCFG_SIGDIG,temp,64);
    size_t sdig = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_GAOPTCFG_XMIN,temp,64);
    double minx = atof(temp);

    GetDlgItemText(dlg,IDD_GAOPTCFG_XMAX,temp,64);
    double maxx = atof(temp);

    GetDlgItemText(dlg,IDD_GAOPTCFG_YMIN,temp,64);
    double miny = atof(temp);

    GetDlgItemText(dlg,IDD_GAOPTCFG_YMAX,temp,64);
    double maxy = atof(temp);

    GetDlgItemText(dlg,IDD_GAOPTCFG_CPROB,temp,64);
    float crate = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_GAOPTCFG_MPROB,temp,64);
    float mrate = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_GAOPTCFG_MUTES,temp,64);
    float msgn = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_GAOPTCFG_MUTEE,temp,64);
    float mexp = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_GAOPTCFG_MUTEM,temp,64);
    float mman = float(fabs(atof(temp)));

    bool mloop;

    if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_MLOOP,BM_GETCHECK,0,0L))
        mloop = true;
    else
        mloop = false;

    if ((psz < 10)
    ||  (gen < 10)
    ||  (frq <  1)
    ||  (frq > gen)
    ||  (sdig < 1)
    ||  (sdig > 15)
    ||  (mrate > 100.0F)
    ||  (crate > 100.0F)
    ||  ((mrate > 99.0F) && (mloop))
    ||  (minx >= maxx)
    ||  (miny >= maxy)
    ||  ((msgn + mexp + mman) < 1.0F))
        {
        MessageBeep(MB_ICONHAND);
        MessageBox(NULL,GAOptConfigDlgErr,"Configuration Error",MB_OK | MB_ICONHAND);
        return FALSE;
        }

    DefPopSize   = psz;
    DefTestSize  = gen;
    DefReptFreq  = frq;
    DefSigDigits = sdig;
    DefXMin      = minx;
    DefXMax      = maxx;
    DefYMin      = miny;
    DefYMax      = maxy;
    DefCrossRate = crate / 100.0F;
    DefMuteRate  = mrate / 100.0F;
    DefWtSign    = msgn;
    DefWtExp     = mexp;
    DefWtMant    = mman;
    DefMuteLoop  = mloop;

    if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_CROSSX,BM_GETCHECK,0,0L))
        DefCrossX = true;
    else
        DefCrossX = false;

    if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_CROSSY,BM_GETCHECK,0,0L))
        DefCrossY = true;
    else
        DefCrossY = false;

    if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_CROSSB,BM_GETCHECK,0,0L))
        DefCrossB = true;
    else
        DefCrossB = false;

    if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_MUTEX,BM_GETCHECK,0,0L))
        DefMutateX = true;
    else
        DefMutateX = false;

    if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_MUTEY,BM_GETCHECK,0,0L))
        DefMutateY = true;
    else
        DefMutateY = false;

    if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_ELITE,BM_GETCHECK,0,0L))
        DefElitist = true;
    else
        DefElitist = false;

    if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_FSCALE,BM_GETCHECK,0,0L))
        DefFitScale = true;
    else
        DefFitScale = false;

    if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_FTEXP,BM_GETCHECK,0,0L))
        DefFitAlgor = FSA_EXPON;
    else
        if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_FTWDW,BM_GETCHECK,0,0L))
            DefFitAlgor = FSA_WINDOW;
        else
            DefFitAlgor = FSA_LINEAR;

    GetDlgItemText(dlg,IDD_GAOPTCFG_LNBASE,temp,64);
    DefFitLinBase = fabs(atof(temp));

    GetDlgItemText(dlg,IDD_GAOPTCFG_LNDEC,temp,64);
    DefFitLinDec = fabs(atof(temp));

    GetDlgItemText(dlg,IDD_GAOPTCFG_LNMIN,temp,64);
    DefFitLinMin = fabs(atof(temp));

    if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_EQF6,BM_GETCHECK,0,0L))
        DefEquation = 0;
    else
        if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_EQF7,BM_GETCHECK,0,0L))
            DefEquation = 1;
        else
            if (SendDlgItemMessage(dlg,IDD_GAOPTCFG_EQF8,BM_GETCHECK,0,0L))
                DefEquation = 2;
            else
                DefEquation = 3;

    return TRUE;
    }

