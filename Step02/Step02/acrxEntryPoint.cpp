// (C) Copyright 2002-2022 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep02App : public AcRxArxApp {

public:
	CStep02App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	static void AsdkStep02_myInput(void) {
		int stat, val;
		double rValue;
		TCHAR kWord[133];

		acedInitGet(RSG_NOZERO | RSG_NONEG, NULL);

		if (acedGetInt(_T("\nEnter an integer value: "), &val) != RTNORM)
			return;

		rValue = 12.0;
		acedInitGet(RSG_NOZERO, _T("PI A B C"));
		stat = acedGetReal(_T("\nEnter a real value [PI/A/B/C] <C>: "), &rValue);
		switch (stat) {
		case RTCAN:
			return;
		case RTKWORD:
			if (acedGetInput(kWord) != RTNORM)
				return;
			if (!_tcscmp(_T("PI"), kWord))
				rValue = 3.14;
			else if (!_tcscmp(_T("A"), kWord))
				rValue = 10;
			else if (!_tcscmp(_T("B"), kWord))
				rValue = 11;
			else if (!_tcscmp(_T("C"), kWord))
				rValue = 12;
			break;
		case RTNONE:
			acutPrintf(_T("\nTake default rValue %lf"), rValue);
			break;
		case RTNORM:
			break;
		default:
			return;
		}

		acutPrintf(_T("\nInteger : %d"), val);
		acutPrintf(_T("\nReal    : %lf"), rValue);
	}

	static void AsdkStep02_mySelect(void) {
		ads_name en;
		ads_point pt;

		if (acedEntSel(_T("\nSelect entity"), en, pt) != RTNORM)
			return;
		acutPrintf(_T("\nYou selected entity ID: %ld, %ld"), en[0]);
	}	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep02App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep02App, AsdkStep02, _mySelect, mySelect, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep02App, AsdkStep02, _myInput, myInput, ACRX_CMD_TRANSPARENT, NULL);
