//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      evoptcfg.h     v1.00
//
//      A class for configuring evolving state machines.
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include "forgeres.h"
#include "evoptcfg.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

size_t EVOptConfig::DefPopSize    = 100;
size_t EVOptConfig::DefTestSize   = 100;
size_t EVOptConfig::DefTestLen    =   5;
size_t EVOptConfig::DefReptFreq   =   1;
size_t EVOptConfig::DefMaxState   =   4;
float  EVOptConfig::DefMuteRate   =   0.5F;
float  EVOptConfig::DefWeightO    =  10.0F;
float  EVOptConfig::DefWeightT    =  10.0F;
float  EVOptConfig::DefWeightA    =  10.0F;
float  EVOptConfig::DefWeightD    =  10.0F;
float  EVOptConfig::DefWeightN    =  10.0F;
double EVOptConfig::DefPayoffDD   =   1.0000;
double EVOptConfig::DefPayoffDC   =   5.0000;
double EVOptConfig::DefPayoffCD   =   0.0000;
double EVOptConfig::DefPayoffCC   =   3.0000;
EVMove EVOptConfig::DefFirstMove  = EVM_RANDOM;
bool   EVOptConfig::DefFitScale   =  true;
double EVOptConfig::DefFitLinBase = 100.0;
double EVOptConfig::DefFitLinDec  =  10.0;
double EVOptConfig::DefFitLinMin  =   1.0;

static LPCSTR EVOptConfigDlgName = MAKEINTRESOURCE(DLG_EVOPTCFG);

static const char * EVOptConfigDlgErr =
    "Population and Generations must be >= 10.\n"
    "Report frequency must be > 0.\n"
    "MuteRate Rate must be <= 100.0\n"
    "Test Length must be greater than 2.\n"
    "Max State must be between 3 and 8.\n"
    "Sum of mutation weights must be > 1.";

BOOL CALLBACK EVOptConfigDlgProc
    (
    HWND  dlg,
    WORD  message,
    WORD  wParam,
    DWORD lParam
    );

EVOptConfig::EVOptConfig
    (
    HINSTANCE inst,
    HWND      parent
    )
    {
    int res;

    #ifdef _WIN32
        res = DialogBox(inst,EVOptConfigDlgName,parent,FARPROC(EVOptConfigDlgProc));
    #else
        DLGPROC thunk   = MakeProcInstance(FARPROC(EVOptConfigDlgProc),inst);
        res = DialogBox(inst,EVOptConfigDlgName,parent,thunk);
        FreeProcInstance(thunk);
    #endif

    if (res)
        Valid = true;
    else
        Valid = false;

    PopSize    = DefPopSize;
    TestSize   = DefTestSize;
    TestLen    = DefTestLen;
    ReptFreq   = DefReptFreq;
    MaxState   = DefMaxState;
    MuteRate   = DefMuteRate;
    WeightO    = DefWeightO;
    WeightT    = DefWeightT;
    WeightA    = DefWeightA;
    WeightD    = DefWeightD;
    WeightN    = DefWeightN;
    PayoffDD   = DefPayoffDD;
    PayoffDC   = DefPayoffDC;
    PayoffCD   = DefPayoffCD;
    PayoffCC   = DefPayoffCC;
    FirstMove  = DefFirstMove;
    FitScale   = DefFitScale;
    FitLinBase = DefFitLinBase;
    FitLinDec  = DefFitLinDec;
    FitLinMin  = DefFitLinMin;
    }

#ifdef __BORLANDC__
#pragma argsused
#endif

BOOL CALLBACK EVOptConfigDlgProc
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
            EVOptConfig::DlgInit(dlg);
            return TRUE;

        case WM_COMMAND:
            switch (wParam)
                {
                case IDOK:
                    if (TRUE == EVOptConfig::DlgStore(dlg))
                        EndDialog(dlg, 1);

                    break;

                case IDCANCEL:
                    EndDialog(dlg, 0);
                }

            return TRUE;
        }

    return FALSE;
    }

void EVOptConfig::DlgInit
    (
    HWND  dlg
    )
    {
    char temp[64];

    sprintf(temp,"%u",DefPopSize);
    SetDlgItemText(dlg,IDD_EVOPTCFG_POP,temp);

    sprintf(temp,"%u",DefTestSize);
    SetDlgItemText(dlg,IDD_EVOPTCFG_GEN,temp);

    sprintf(temp,"%u",DefReptFreq);
    SetDlgItemText(dlg,IDD_EVOPTCFG_FREQ,temp);

    sprintf(temp,"%u",DefTestLen);
    SetDlgItemText(dlg,IDD_EVOPTCFG_LEN,temp);

    sprintf(temp,"%u",DefMaxState);
    SetDlgItemText(dlg,IDD_EVOPTCFG_MAX,temp);

    sprintf(temp,"%.6g",DefMuteRate * 100.0);
    SetDlgItemText(dlg,IDD_EVOPTCFG_MRATE,temp);

    sprintf(temp,"%.6g",DefWeightO);
    SetDlgItemText(dlg,IDD_EVOPTCFG_WTO,temp);

    sprintf(temp,"%.6g",DefWeightT);
    SetDlgItemText(dlg,IDD_EVOPTCFG_WTT,temp);

    sprintf(temp,"%.6g",DefWeightA);
    SetDlgItemText(dlg,IDD_EVOPTCFG_WTA,temp);

    sprintf(temp,"%.6g",DefWeightD);
    SetDlgItemText(dlg,IDD_EVOPTCFG_WTD,temp);

    sprintf(temp,"%.6g",DefWeightN);
    SetDlgItemText(dlg,IDD_EVOPTCFG_WTN,temp);

    sprintf(temp,"%.15g",DefPayoffDD);
    SetDlgItemText(dlg,IDD_EVOPTCFG_PAYDD,temp);

    sprintf(temp,"%.15g",DefPayoffDC);
    SetDlgItemText(dlg,IDD_EVOPTCFG_PAYDC,temp);

    sprintf(temp,"%.15g",DefPayoffCD);
    SetDlgItemText(dlg,IDD_EVOPTCFG_PAYCD,temp);

    sprintf(temp,"%.15g",DefPayoffCC);
    SetDlgItemText(dlg,IDD_EVOPTCFG_PAYCC,temp);

    switch (DefFirstMove)
        {
        case EVM_DEFECT:
            SendDlgItemMessage(dlg,IDD_EVOPTCFG_M1D,BM_SETCHECK,1,0L);
            break;
        case EVM_COOPER:
            SendDlgItemMessage(dlg,IDD_EVOPTCFG_M1C,BM_SETCHECK,1,0L);
            break;
        case EVM_RANDOM:
            SendDlgItemMessage(dlg,IDD_EVOPTCFG_M1R,BM_SETCHECK,1,0L);
        }

    if (DefFitScale)
        SendDlgItemMessage(dlg,IDD_EVOPTCFG_FSCALE,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_EVOPTCFG_FSCALE,BM_SETCHECK,0,0L);

    sprintf(temp,"%.15g",DefFitLinBase);
    SetDlgItemText(dlg,IDD_EVOPTCFG_LNBASE,temp);

    sprintf(temp,"%.15g",DefFitLinDec);
    SetDlgItemText(dlg,IDD_EVOPTCFG_LNDEC,temp);

    sprintf(temp,"%.15g",DefFitLinMin);
    SetDlgItemText(dlg,IDD_EVOPTCFG_LNMIN,temp);
    }

BOOL EVOptConfig::DlgStore
    (
    HWND  dlg
    )
    {
    char temp[64];

    GetDlgItemText(dlg,IDD_EVOPTCFG_POP,temp,64);
    size_t psz = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_EVOPTCFG_GEN,temp,64);
    size_t gen = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_EVOPTCFG_FREQ,temp,64);
    size_t frq = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_EVOPTCFG_LEN,temp,64);
    size_t len = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_EVOPTCFG_MAX,temp,64);
    size_t mst = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_EVOPTCFG_MRATE,temp,64);
    float mrate = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_EVOPTCFG_WTO,temp,64);
    float wto = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_EVOPTCFG_WTT,temp,64);
    float wtt = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_EVOPTCFG_WTA,temp,64);
    float wta = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_EVOPTCFG_WTD,temp,64);
    float wtd = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_EVOPTCFG_WTN,temp,64);
    float wtn = float(fabs(atof(temp)));

    if ((psz < 10)
    ||  (gen < 10)
    ||  (len <  3)
    ||  (frq <  1)
    ||  (frq > gen)
    ||  (mst <  3)
    ||  (mst >  8)
    ||  (mrate > 100.0F)
    ||  ((wto + wtt + wta + wtd + wtn) < 1.0F))
        {
        MessageBeep(MB_ICONHAND);
        MessageBox(NULL,EVOptConfigDlgErr,"Configuration Error",MB_OK | MB_ICONHAND);
        return FALSE;
        }

    DefPopSize    = psz;
    DefTestSize   = gen;
    DefReptFreq  = frq;
    DefTestLen    = len;
    DefMaxState   = mst;
    DefMuteRate   = mrate / 100.0F;
    DefWeightO    = wto;
    DefWeightT    = wtt;
    DefWeightA    = wta;
    DefWeightD    = wtd;
    DefWeightN    = wtn;

    GetDlgItemText(dlg,IDD_EVOPTCFG_PAYDD,temp,64);
    DefPayoffDD = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_EVOPTCFG_PAYDC,temp,64);
    DefPayoffDC = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_EVOPTCFG_PAYCD,temp,64);
    DefPayoffCD = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_EVOPTCFG_PAYCC,temp,64);
    DefPayoffCC = float(fabs(atof(temp)));

    if (SendDlgItemMessage(dlg,IDD_EVOPTCFG_M1D,BM_GETCHECK,0,0L))
        DefFirstMove = EVM_DEFECT;
    else
        if (SendDlgItemMessage(dlg,IDD_EVOPTCFG_M1C,BM_GETCHECK,0,0L))
            DefFirstMove = EVM_COOPER;
        else
            DefFirstMove = EVM_RANDOM;

    if (SendDlgItemMessage(dlg,IDD_EVOPTCFG_FSCALE,BM_GETCHECK,0,0L))
        DefFitScale = true;
    else
        DefFitScale = false;

    GetDlgItemText(dlg,IDD_EVOPTCFG_LNBASE,temp,64);
    DefFitLinBase = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_EVOPTCFG_LNDEC,temp,64);
    DefFitLinDec = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_EVOPTCFG_LNMIN,temp,64);
    DefFitLinMin = float(fabs(atof(temp)));

    return TRUE;
    }

