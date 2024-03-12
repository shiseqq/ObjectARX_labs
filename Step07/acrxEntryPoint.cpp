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
#include "utilities.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("Asdk")

//globals
AsdkEdEmployeeReactor* pEdEmployeeReactor = NULL;
AsdkEmployeeReactor* pEmployeeReactor = NULL;

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep07App : public AcRxArxApp {

public:
	CStep07App() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);
		// TODO: Add your initialization code here
		pEdEmployeeReactor = new AsdkEdEmployeeReactor(true);
		pEmployeeReactor = new AsdkEmployeeReactor();

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);
		// TODO: Add your code here

		// TODO: clean up your application
		detachAllEmployeeReactors();
		delete pEdEmployeeReactor;
		delete pEmployeeReactor;

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}


	// ----- AsdkStep07._CREATE command (do not rename)
	static void AsdkStep07_CREATE(void)
	{
		// Add your code for command AsdkStep07._CREATE here
				// Create a new layer named "USER"
		// createLayer returns the object ID of the newly created layer
		AcDbObjectId layerId;
		if (createLayer(_T("USER"), layerId) != Acad::eOk) {
			acutPrintf(_T("\nERROR: Couldn't create layer record."));
			return;
		}
		// This is not always needed, but a call to 'applyCurDwgLayerTableChanges()'
		// will synchronize the newly created layer table change with the 
		// rest of the current DWG database.
		applyCurDwgLayerTableChanges();

		acutPrintf(_T("\nLayer USER successfully created."));

		// Create a new block definition named "EMPLOYEE"
		if (createBlockRecord(_T("EMPLOYEE")) != Acad::eOk)
			acutPrintf(_T("\nERROR: Couldn't create block record."));
		else
			acutPrintf(_T("\nBlock EMPLOYEE successfully created."));
	}
	// -----------------------------------------------------------------------------
	virtual AcRx::AppRetCode On_kLoadDwgMsg(void* pkt);
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep07App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep07App, AsdkStep07, _CREATE, CREATE, ACRX_CMD_TRANSPARENT, NULL)
// -----------------------------------------------------------------------------

AcRx::AppRetCode CStep07App::On_kLoadDwgMsg(void* pkt)
{
	AcRx::AppRetCode retCode = AcRxArxApp::On_kLoadDwgMsg(pkt);

	DocVars.docData().m_pAsdkDbEmployeeReactor = new AsdkDbEmployeeReactor(acdbHostApplicationServices()->workingDatabase());
	return (retCode);
}

