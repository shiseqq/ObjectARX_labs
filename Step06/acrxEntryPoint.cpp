// (C) Copyright 1990-2006 by Autodesk, Inc. 
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
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("Asdk")
#pragma comment (lib ,"AdskEmployee.lib")
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep06App : public AcRxArxApp {

public:
	CStep06App() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);
		// TODO: Add your initialization code here
		AcRxObject* pSvc;
		if (!(pSvc = acrxServiceDictionary->at(ASDKEMPLOYEE_DBXSERVICE)))
		{
			// Try to load the module, if it is not yet present 
			if (!acrxDynamicLinker->loadModule(_T("AsdkEmployee.dbx"), 0))
			{
				acutPrintf(_T("Unable to load AsdkEmployeeDetails.dbx. Unloading this application...\n"));
				return (AcRx::kRetError);
			}
		}

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);
		// TODO: Add your code here

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}


	// ----- AsdkStep06._CREATEEMPLOYEE command (do not rename)
	static void AsdkStep06_CREATEEMPLOYEE(void)
	{
		// Add your code for command AsdkStep06._CREATEEMPLOYEE here
		// Get user input for employee
		int id, cubeNumber;
		TCHAR strFirstName[133];
		TCHAR strLastName[133];
		AcGePoint3d pt;
		if (acedGetInt(_T("Enter employee ID: "), &id) != RTNORM
			|| acedGetInt(_T("Enter cube number: "), &cubeNumber) != RTNORM
			|| acedGetString(0, _T("Enter employee first name: "), strFirstName) != RTNORM
			|| acedGetString(0, _T("Enter employee last name: "), strLastName) != RTNORM
			|| acedGetPoint(NULL, _T("Employee position: "), asDblArray(pt)) != RTNORM
			) {
			return;
		}

		// Get a pointer to the current drawing
		// and get the drawing's block table. Open it for read.
		AcDbBlockTable* pBlockTable;
		if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead) == Acad::eOk) {
			// Get the Model Space record and open it for write. This will be the owner of the new employee entity.
			AcDbBlockTableRecord* pSpaceRecord;
			if (pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite) == Acad::eOk) {
				AsdkEmployee* pEnt = new AsdkEmployee;
				pEnt->setID(id);
				pEnt->setCube(cubeNumber);
				pEnt->setFirstName(strFirstName);
				pEnt->setLastName(strLastName);
				pEnt->setCenter(pt);
				// Append pEnt to Model Space, then close it and the Model Space record.
				AcDbObjectId idObj;
				if (pSpaceRecord->appendAcDbEntity(idObj, pEnt) == Acad::eOk)
					pEnt->close();
				else
					delete pEnt;
				pSpaceRecord->close();
			}
			pBlockTable->close();
		}
	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep06App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, AsdkStep06, _CREATEEMPLOYEE, CREATEEMPLOYEE, ACRX_CMD_TRANSPARENT, NULL)
