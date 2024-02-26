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
class CStep04App : public AcRxArxApp {

public:
	CStep04App () : AcRxArxApp () {}

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
	
	static void AsdkStep04_ADDENTRY() {
		TCHAR empName[132] = {0};

		if (acedGetString(NULL, _T("\nEnter employee name: "), empName) != RTNORM)
			return;

		AcDbDictionary* namedDict;
		if (acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(namedDict, AcDb::kForRead) != Acad::eOk) {
			return;
		}
		AcDbObjectId objId;
		AcDbDictionary* empDict;
		if (namedDict->getAt(_T("ASDK_EMPLOYEE_DICTIONARY"), objId) == Acad::eKeyNotFound) {
			if (namedDict->upgradeOpen() != Acad::eOk) {
				namedDict->close();
				return;
			}
			empDict = new AcDbDictionary;
			if (namedDict->setAt((_T("ASDK_EMPLOYEE_DICTIONARY")), empDict, objId) != Acad::eOk) {
				delete empDict;
				namedDict->close();
				return;
			}
		}
		else {
			AcDbObject* pO;
			if (acdbOpenAcDbObject(pO, objId, AcDb::kForWrite) != Acad::eOk) {
				acutPrintf(_T("\nCannot open the object for write."));
				namedDict->close();
				return;
			}
			if ((empDict = AcDbDictionary::cast(pO)) == NULL) {
				acutPrintf(_T("\nEntry found in the NOD, but it is not a dictionary."));
				pO->close();
				namedDict->close();
				return;
			}
		}
		namedDict->close();
		if (empDict->getAt(empName, objId) == Acad::eOk) {
			acutPrintf(_T("\nThis employee is already registered."));
			empDict->close();
			return;
		}
		AcDbXrecord* empEntry = new AcDbXrecord;
		if (empDict->setAt(empName, empEntry, objId) != Acad::eOk) {
			acutPrintf(_T("\nFailed to add the new employee in the dictionary."));
			delete empEntry;
			empDict->close();
			return;
		}
		empEntry->close();
		empDict->close();
	}

	static void AsdkStep04_LISTENTRIES(void)
	{
		AcDbDictionary* pNOD;
		if (acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNOD, AcDb::kForRead) != Acad::eOk) {
			acutPrintf(_T("\nUnable to open the NOD! Aborting..."));
			return;
		}
		AcDbObjectId idO;
		AcDbObject* pO;
		if (pNOD->getAt(_T("ASDK_EMPLOYEE_DICTIONARY"), idO) != Acad::eOk) {
			acutPrintf(_T("\nNo dictionary, no entry to remove..."));
			pNOD->close();
			return;
		}
		if (acdbOpenAcDbObject(pO, idO, AcDb::kForRead) != Acad::eOk) {
			acutPrintf(_T("\nCannot open the object for write."));
			pNOD->close();
			return;
		}
		AcDbDictionary* pEmployeeDict;
		if ((pEmployeeDict = AcDbDictionary::cast(pO)) == NULL) {
			acutPrintf(_T("\nEntry found in the NOD, but it is not a dictionary."));
			pO->close();
			pNOD->close();
			return;
		}
		pNOD->close();
		AcDbDictionaryIterator* pIter;
		if ((pIter = pEmployeeDict->newIterator()) != NULL) {
			for (; !pIter->done(); pIter->next()) {
				acutPrintf(_T("*Employee: %s\n"), pIter->name());
			}
			delete pIter;
		}

		pEmployeeDict->close();
	}

	static void AsdkStep04_REMOVEENTRY(void)
	{
		TCHAR strID[133];
		if (acedGetString(0, _T("Enter employee name: "), strID) != RTNORM)
			return;
		AcDbDictionary* pNOD;
		if (acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNOD, AcDb::kForRead) != Acad::eOk) {
			acutPrintf(_T("\nUnable to open the NOD! Aborting..."));
			return;
		}
		AcDbObjectId idO;
		AcDbObject* pO;
		if (pNOD->getAt(_T("ASDK_EMPLOYEE_DICTIONARY"), idO) != Acad::eOk) {
			acutPrintf(_T("\nNo dictionary, no entry to remove..."));
			pNOD->close();
			return;
		}
		if (acdbOpenAcDbObject(pO, idO, AcDb::kForRead) != Acad::eOk) {
			acutPrintf(_T("\nCannot open the object for write."));
			pNOD->close();
			return;
		}
		AcDbDictionary* pEmployeeDict;
		if ((pEmployeeDict = AcDbDictionary::cast(pO)) == NULL) {
			acutPrintf(_T("\nEntry found in the NOD, but it is not a dictionary."));
			pO->close();
			pNOD->close();
			return;
		}
		pNOD->close();
		if (pEmployeeDict->getAt(strID, idO) != Acad::eOk) {
			acutPrintf(_T("\nEntry not found."));
			pEmployeeDict->close();
			return;
		}
		pEmployeeDict->close();
		if (acdbOpenAcDbObject(pO, idO, AcDb::kForWrite) != Acad::eOk) {
			acutPrintf(_T("\nEntry cannot be opened for write."));
			return;
		}
		pO->erase();
		pO->close();
	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep04App)


ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, AsdkStep04, _LISTENTRIES, LISTENTRIES, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, AsdkStep04, _ADDENTRY, ADDENTRY, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, AsdkStep04, _REMOVEENTRY, REMOVEENTRY, ACRX_CMD_TRANSPARENT, NULL)