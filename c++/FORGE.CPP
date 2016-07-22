//-----------------------------------------------------------
//  program FOR Genetic algorithm Experimentation (FORGE)
//-----------------------------------------------------------
//
//      forge.cpp     v1.00
//
//      Main application module
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include "windows.h"  // Windows definitions
#include "limits.h"   // limits of types
#include "stdio.h"    // standard file I/O functions
#include "stdlib.h"   // misc. library functions
#include "string.h"   // C-style string functions
#include "strstrea.h" // strstream definitions
#include "iomanip.h"  // stream manipulators
#include "diagwin.h"  // diagnostic display class for windows
#include "fontutil.h" // font utilities and classes
#include "forgeres.h" // resource constants
#include "bool.h"     // boolean type

//-----------------
// type definitions
//-----------------

enum ErrorType
    {
    ET_CRASH,
    ET_ERROR,
    ET_WARNING
    };

//------------------------
// global data
//------------------------

// constants
const char * ForgeClassName = "ForgeWindowClass";
const char * AppName        = "Forge";
const char * AppVersion     = "v1.01a";
const LPCSTR MenuForge      = MAKEINTRESOURCE(MENU_FORGE);
const LPCSTR IconForge      = MAKEINTRESOURCE(ICON_FORGE);
const LPCSTR NameDlgAbout   = MAKEINTRESOURCE(DLG_ABOUT);
const int    DisplayID      = 1;
const DWORD  DisplayStyle   = WS_CHILD |
                              WS_VSCROLL |
                              WS_HSCROLL |
                              ES_MULTILINE |
                              ES_READONLY |
                              ES_LEFT |
                              ES_AUTOHSCROLL |
                              ES_AUTOVSCROLL;

// variables
HANDLE     ThisInstance = NULL;
HWND       Display   = NULL;
HWND       MainWdw   = NULL;
HACCEL     MainAccel = NULL;
DiagOutWin Diag(AppName);
Font       StdFont("Courier New",10,FALSE,FALSE);

//-----------------------------
// external function prototypes
//-----------------------------

int PASCAL WinMain
    (
    HANDLE instance,
    HANDLE prevInstance,
    LPSTR  commandLine,
    int    commandShow
    );

static BOOL InitApplication
    (
    HANDLE instance
    );

static BOOL InitInstance
    (
    HANDLE instance,
    int    commandShow
    );

LRESULT CALLBACK MainWindow
    (
    HWND  thisWindow,
    UINT  message,
    WPARAM wordParam,
    LPARAM longParam
    );

void RunTest
    (
    WPARAM testid
    );

void TestRoulette
    (
    strstream & buffer
    );

void TestBool
    (
    strstream & buffer
    );

void TestRandDev
    (
    strstream & buffer
    );

void TestMutableFloat
    (
    strstream & buffer
    );

void TestMutableDouble
    (
    strstream & buffer
    );

void TestMath
    (
    strstream & buffer
    );

void TestSorting
    (
    strstream & buffer
    );

void TestFSM
    (
    strstream & buffer
    );

void TestBlackbox
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    );

void TestBBAnalysis
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    );

void TestPeakSearch
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    );

void TestDilemma
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    );

void TestArtist
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    );

void TestEvolvingMachines
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    );

void TestRoboticAnts
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    );

void CopyBuffer
    (
    bool allFlag
    );

void ErrorMessage
    (
    const char * msg,
    ErrorType etype = ET_WARNING
    );

BOOL CALLBACK DlgAbout
    (
    HWND  dlg,
    WORD  message,
    WORD  wParam,
    DWORD lParam
    );

//-------------------------------------------
//  WinMain -- program start and message loop
//-------------------------------------------

#ifdef __BORLANDC__
    #pragma argsused
#endif

int PASCAL WinMain
    (
    HANDLE instance,
    HANDLE prevInstance,
    LPSTR  commandLine,
    int    commandShow
    )
    {
    MSG msg;

    if (prevInstance)
        {
        ErrorMessage("Forge is already running!", ET_WARNING);
        return 0;
        }

    if (!InitApplication(instance))
        return 0;

    if (!InitInstance(instance, commandShow))
        return 0;

    try {
        while (GetMessage(&msg, NULL, NULL, NULL))
            {
            if (TranslateAccelerator(MainWdw,MainAccel,&msg) == 0)
                {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                }
            }
        }
    catch (ExceptionBase & ex)
        {
        ex.Explain(Diag);
        }

    return msg.wParam;
    }

//--------------------------
// initialize an application
//--------------------------

static BOOL InitApplication
    (
    HANDLE instance
    )
    {
    WNDCLASS wc;

    wc.style         = 0;
    wc.lpfnWndProc   = MainWindow;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = instance;
    wc.hIcon         = LoadIcon(instance,IconForge);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName  = MenuForge;
    wc.lpszClassName = ForgeClassName;

    return RegisterClass(&wc);
    }

//-----------------------
// initialize an instance
//-----------------------

static BOOL InitInstance
    (
    HANDLE instance,
    int commandShow
    )
    {
    HWND hWnd;

    // save the instance handle
    ThisInstance = instance;

    // create a window
    hWnd = CreateWindow(ForgeClassName, AppName,
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        NULL, NULL, instance, 0);

    if (!hWnd)
        {
        ErrorMessage("Cannot create main window", ET_WARNING);
        return FALSE;
        }

    ShowWindow(hWnd, commandShow);
    UpdateWindow(hWnd);

    MainWdw = hWnd;

    // get size of client area
    RECT area;

    GetClientRect(hWnd,&area);

    // create edit control
    Display = CreateWindow("EDIT","",DisplayStyle,
                           0, 0, area.right, area.bottom,
                           hWnd, (HMENU)DisplayID,instance,0);

    if (!Display)
        {
        ErrorMessage("Cannot create output window", ET_WARNING);
        return FALSE;
        }

    ShowWindow(Display, SW_SHOW);
    UpdateWindow(Display);

    // set edit window font
    SendMessage(Display,WM_SETFONT,(WPARAM)StdFont.GetFont(),0L);

    // load accelarator table
    MainAccel = LoadAccelerators(instance,MenuForge);

    if (MainAccel == NULL)
        return FALSE;

    return TRUE;
    }

//---------------------
// Main window function
//---------------------

LRESULT CALLBACK MainWindow
    (
    HWND  thisWindow,
    UINT  message,
    WPARAM wordParam,
    LPARAM longParam
    )
    {
    #ifndef _WIN32
    DLGPROC thunk;
    #endif

    switch (message)
        {
        case WM_COMMAND:
            switch (wordParam)
                {
                case IDM_TEST_ROULETTE:
                case IDM_TEST_BOOL:
                case IDM_TEST_RANDDEV:
                case IDM_TEST_MFLOAT:
                case IDM_TEST_MDOUBLE:
                case IDM_TEST_MATH:
                case IDM_TEST_SORTS:
                case IDM_TEST_FSM:
                case IDM_OPT_BLACKBOX:
                case IDM_OPT_BBOX_ANALYSIS:
                case IDM_OPT_PEAK_SEARCH:
                case IDM_OPT_PDILEMMA:
                case IDM_OPT_TRAVART:
                case IDM_EVOL_MACHINES:
                case IDM_ROBOTIC_ANTS:
                    RunTest(wordParam);
                    break;

                case IDM_COPY:
                    // copy edit window selection to clipboard
                    CopyBuffer(false);
                    break;

                case IDM_COPYALL:
                    // copy edit window to clipboard
                    CopyBuffer(true);
                    break;

                case IDM_ABOUT:
                    #ifdef _WIN32
                    DialogBox(ThisInstance,NameDlgAbout,
                              thisWindow,FARPROC(DlgAbout));
                    #else
                    thunk = MakeProcInstance(FARPROC(DlgAbout),
                                             ThisInstance);

                    DialogBox(ThisInstance,NameDlgAbout,
                              thisWindow,thunk);

                    FreeProcInstance(thunk);
                    #endif
                    break;

                case IDM_EXIT:
                    // exit program
                    DestroyWindow(thisWindow);
                    break;

                default:
                    return DefWindowProc(thisWindow, message,
                                         wordParam, longParam);
                }

            break;

        case WM_SETFOCUS:
            // set focus to edit control
            SetFocus(Display);
            break;

        case WM_SIZE:
            // resize edit control
            MoveWindow(Display,0,0,
                       LOWORD(longParam),
                       HIWORD(longParam),
                       TRUE);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(thisWindow, message,
                                 wordParam, longParam);
        }

    return 0L;
    }

//--------------------------------------
// framework for running test procedures
//--------------------------------------

void RunTest
    (
    WPARAM testid
    )
    {
    #if defined(__WIN32__) || defined(_WIN32)
    const size_t strbufsz = 100000;
    #else
    const size_t strbufsz = 32000;
    #endif

    char * strbuf = new char [strbufsz];

    if (strbuf == NULL)
        return;

    strstream buffer(strbuf,strbufsz,ios::out);

    HCURSOR cursor = SetCursor(LoadCursor(NULL,IDC_WAIT));

    try
        {
        switch (testid)
            {
            case IDM_TEST_ROULETTE:
                TestRoulette(buffer);
                break;

            case IDM_TEST_BOOL:
                TestBool(buffer);
                break;

            case IDM_TEST_RANDDEV:
                TestRandDev(buffer);
                break;

            case IDM_TEST_MFLOAT:
                TestMutableFloat(buffer);
                break;

            case IDM_TEST_MDOUBLE:
                TestMutableDouble(buffer);
                break;

            case IDM_TEST_MATH:
                TestMath(buffer);
                break;

            case IDM_TEST_SORTS:
                TestSorting(buffer);
                break;

            case IDM_TEST_FSM:
                TestFSM(buffer);
                break;

            case IDM_OPT_BLACKBOX:
                TestBlackbox(ThisInstance,MainWdw,buffer);
                break;

            case IDM_OPT_BBOX_ANALYSIS:
                TestBBAnalysis(ThisInstance,MainWdw,buffer);
                break;

            case IDM_OPT_PEAK_SEARCH:
                TestPeakSearch(ThisInstance,MainWdw,buffer);
                break;

            case IDM_OPT_PDILEMMA:
                TestDilemma(ThisInstance,MainWdw,buffer);
                break;

            case IDM_OPT_TRAVART:
                TestArtist(ThisInstance,MainWdw,buffer);
                break;

            case IDM_EVOL_MACHINES:
                TestEvolvingMachines(ThisInstance,MainWdw,buffer);
                break;

            case IDM_ROBOTIC_ANTS:
                TestRoboticAnts(ThisInstance,MainWdw,buffer);
                break;

            default:
                buffer << "unknown test selection!";
            }
        }
    catch (ExceptionBase & ex)
        {
        ex.Explain(Diag);
        buffer << "\r\nPROGRAM ERROR! BUFFER MAY BE INCOMPLETE!";
        }

    // terminate buffer
    buffer << ends;

    // display buffer in edit control
    #if defined(__WIN32__) || defined(_WIN32)
    SendMessage(Display,WM_SETTEXT,0,(LPARAM)(LPSTR)buffer.str());
    #else
    SendMessage(Display,EM_SETSEL,0,MAKELONG(0,-1));
    SendMessage(Display,EM_REPLACESEL,0,(LPARAM)(LPSTR)buffer.str());
    #endif
    SendMessage(Display,EM_SETSEL,0,0);

    // restore cursor and set focus to edit window
    SetCursor(cursor);
    SetFocus(Display);

    // delete stream buffer
    delete [] strbuf;
    }

//-----------------------------
// Copy buffer to the clipboard
//-----------------------------

void CopyBuffer
    (
    bool allFlag
    )
    {
    #ifdef __WIN32__
        if (allFlag)
            SendMessage(Display,EM_SETSEL,0,-1L);
    #else
        if (allFlag)
            SendMessage(Display,EM_SETSEL,0,MAKELONG(0,-1));
    #endif

    SendMessage(Display,WM_COPY,0,0L);
    SendMessage(Display,EM_SETSEL,0,0L);
    }

//-------------------------------------------
// Display an error message of given severity
//-------------------------------------------

void ErrorMessage
    (
    const char * msg,
    ErrorType etype
    )
    {
    UINT style;
    UINT sound;
    int response;
    char text[128];

    // copy message to output buffer
    strcpy(text,msg);

    // set values specific to error severity
    switch (etype)
        {
        case ET_CRASH:
            sound = MB_ICONSTOP;
            style = MB_OK;
            strcat(text," PROGRAM WILL TERMINATE!");
            break;
        case ET_ERROR:
            sound = MB_ICONEXCLAMATION;
            style = MB_YESNO;
            strcat(text,"Click 'Yes' to terminate program, "
                        "or 'No' to continue");
            break;
        case ET_WARNING:
            sound = MB_ICONASTERISK;
            style = MB_OK;
        }

    // match icon to sound
    style |= sound;

    // sound off and display message
    MessageBeep(sound);
    response = MessageBox(NULL,text,AppName,style);

    // check for termination
    switch (etype)
        {
        case ET_ERROR:
            if (response == IDNO)
                break;
        case ET_CRASH:
            PostQuitMessage(1); // boom-boom
            // program should never reach this point!
        }
    }

//--------------------
// About... dialog box
//--------------------

#ifdef __BORLANDC__
#pragma argsused
#endif

BOOL CALLBACK DlgAbout
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
            SetDlgItemText(dlg,IDD_ABOUT_VERSION,AppVersion);
            break;

        case WM_COMMAND:
            EndDialog(dlg, TRUE);
            return TRUE;
        }

    return FALSE;
    }


