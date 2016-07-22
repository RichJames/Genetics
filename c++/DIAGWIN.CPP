//==============================================================
// Diagnostic & Exception classes   diagdos.cpp     v1.00
//      Classes for diagnostics and exceptions under Windows
//      Copyright 1995 Scott Robert Ladd
//==============================================================

#include "diagwin.h"

void DiagOutWin::DisplayMsg
    (
    const char * msg,
    DiagLevel level
    )
	{
    UINT style;
    UINT sound;
    int response;
    char text[256];

    // copy message to output buffer
    strncpy(text,msg,128);

    // set values specific to error severity
    switch (level)
        {
        case DIAG_FATAL:
            sound = MB_ICONSTOP;
            style = MB_OK;
            strcat(text,"\nPROGRAM WILL TERMINATE!");
            break;
        case DIAG_ERROR:
            sound = MB_ICONEXCLAMATION;
            style = MB_YESNO;
            strcat(text,"\nClick 'Yes' to terminate program, "
                        "or 'No' to continue");
            break;
        case DIAG_WARNING:
        case DIAG_MSG:
            sound = MB_ICONASTERISK;
            style = MB_OK;
        }

    // match icon to sound
    style |= sound;

    // sound off and display message
    MessageBeep(sound);
    response = MessageBox(NULL,text,Title,style);

    // check for termination
    switch (level)
        {
        case DIAG_ERROR:
            if (response == IDNO)
                break;
        case DIAG_FATAL:
            PostQuitMessage(1); // boom-boom
            // program should never reach this point!
        }
	}

