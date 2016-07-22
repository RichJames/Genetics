//==============================================================
// Diagnostic & Exception classes   diagnose.h       v1.10
//      Base classes for diagnostics and exceptions
//      Copyright 1994, 1995 Scott Robert Ladd
//==============================================================

#ifndef DIAGNOSE_H
#define DIAGNOSE_H

#include "simple.h"

enum DiagLevel
	{
	DIAG_MSG,
	DIAG_WARNING,
	DIAG_ERROR,
	DIAG_FATAL
	};

class DiagOutput
	{
	public:
		virtual void DisplayMsg
            (
            const char * msg, DiagLevel level = DIAG_MSG
            ) = 0;
	};

class DiagnosticBase
	{
	public:
		virtual void Dump
            (
            DiagOutput & out
            ) = 0;

		virtual void ShowInternals
            (
            DiagOutput & out
            ) = 0;

		virtual Boolean CheckIntegrity() = 0;
	};

class ExceptionBase
	{
	public:
		virtual ~ExceptionBase();

		virtual void Explain
            (
            DiagOutput & out
            ) = 0;
	};

#endif
