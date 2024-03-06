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
#include "EmployeeDetails\AsdkEmployeeDetails.h" 
#pragma comment (lib ,"AsdkEmployeeDetails.lib")

//-----------------------------------------------------------------------------
#define szRDS _RXST("Asdk")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep05App : public AcRxArxApp {

public:
	CStep05App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {

		AcRxObject* pSvc;
		if (!(pSvc = acrxServiceDictionary->at(ASDKEMPLOYEEDETAILS_DBXSERVICE)))
		{
			// Try to load the module, if it is not yet present 
			if (!acrxDynamicLinker->loadModule(_T("ASDKEmployeeDetails.dbx"), 0))
			{

				acutPrintf(_T("Unable to load AsdkEmployeeDetails.dbx. Unloading this application...\n"));
				return (AcRx::kRetError);

			}

		}
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);
		// TODO: Add your initialization code here

		return (retCode);
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
	
	static void AsdkStep05_CREATE(void)
	{
		AcDbObjectId layerId;
		if (createLayer(_T("USER"), layerId) != Acad::eOk) {
			acutPrintf(_T("\nERROR: Couldn't create layer record."));
			return;
		}
		applyCurDwgLayerTableChanges();

		acutPrintf(_T("\nLayer USER successfully created."));

		if (createBlockRecord(_T("EMPLOYEE")) != Acad::eOk)
			acutPrintf(_T("\nERROR: Couldn't create block record."));
		else
			acutPrintf(_T("\nBlock EMPLOYEE successfully created."));
	}


	// ----- AsdkStep05._SETLAYER command (do not rename)
	static void AsdkStep05_SETLAYER(void)
	{
		Acad::ErrorStatus es;
		AcDbBlockTable* pBlockTbl;
		AcDbBlockTableRecord* pMS;

		if ((es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTbl, AcDb::kForRead)) != Acad::eOk) {
			acutPrintf(_T("\nCouldn't open the block table!"));
			return;
		}
		if ((es = pBlockTbl->getAt(ACDB_MODEL_SPACE, pMS, AcDb::kForWrite)) != Acad::eOk) {
			acutPrintf(_T("\nCouldn't get Model Space! Drawing corrupt.\n"));
			pBlockTbl->close();
			return;
		}
		pBlockTbl->close();

		AcDbBlockTableRecordIterator* pBtrIter;
		if ((es = pMS->newIterator(pBtrIter)) != Acad::eOk) {
			acutPrintf(_T("\nCouldn't create Model Space iterator."));
			pMS->close();
			return;
		}

		TCHAR* blockName;
		AcDbEntity* pEnt;
		AcDbBlockTableRecord* pCurEntBlock;
		AcDbObjectId blockId;

		for (pBtrIter->start(); !pBtrIter->done(); pBtrIter->step()) {
			if ((es = pBtrIter->getEntity(pEnt, AcDb::kForRead)) != Acad::eOk) {
				acutPrintf(_T("\nCouldn't open entity."));
				continue;
			}
			if (pEnt->isA() != AcDbBlockReference::desc()) {
				pEnt->close();
				continue;
			}
			blockId = (AcDbBlockReference::cast(pEnt))->blockTableRecord();
			if (acdbOpenObject((AcDbObject*&)pCurEntBlock, blockId, AcDb::kForRead) == Acad::eOk) {
				pCurEntBlock->getName(blockName);
				if (_tcscmp(blockName, _T("EMPLOYEE")) == 0) {
					if (pEnt->upgradeOpen() == Acad::eOk)
						pEnt->setLayer(_T("USER"));
				}
				pCurEntBlock->close();
				acdbFree(blockName);
			}
			pEnt->close();
		}
		delete pBtrIter;
		pMS->close();
	}

	static void AsdkStep05_LISTDETAILS(void)
	{
		ads_name ename;
		ads_point pt;
		if (acedEntSel(_T("Select employee: "), ename, pt) != RTNORM)
			return;
		AcDbObjectId idO;
		if (acdbGetObjectId(idO, ename) != Acad::eOk)
			return;
		AcDbObject* pO;
		if (acdbOpenAcDbObject(pO, idO, AcDb::kForRead) != Acad::eOk)
			return;
		if (!pO->isKindOf(AcDbBlockReference::desc())) {
			acutPrintf(_T("\nThis is not a block reference."));
			pO->close();
			return;
		}
		if ((idO = pO->extensionDictionary()) == AcDbObjectId::kNull) {
			pO->close();
			return;
		}
		pO->close();
		AcDbDictionary* pExtDict;
		if (acdbOpenAcDbObject((AcDbObject*&)pExtDict, idO, AcDb::kForRead, Adesk::kFalse) != Acad::eOk) {
			acutPrintf(_T("\nFailed to open ext. dictionary."));
			return;
		}
		AcDbDictionary* pEmployeeDict;
		if (pExtDict->getAt(_T("ASDK_EMPLOYEE_DICTIONARY"), idO) == Acad::eKeyNotFound) {
			pExtDict->close();
			return;
		}
		else {
			if (acdbOpenAcDbObject(pO, idO, AcDb::kForRead) != Acad::eOk) {
				pExtDict->close();
				acutPrintf(_T("\nFailed to open the 'Employee' dictionary."));
				return;
			}
			if ((pEmployeeDict = AcDbDictionary::cast(pO)) == NULL) {
				pO->close();
				pExtDict->close();
				acutPrintf(_T("\nThe entry is not a dictionary"));
				return;
			}
		}
		if (pEmployeeDict->getAt(_T("DETAILS"), idO) != Acad::eOk) {
			pEmployeeDict->close();
			pExtDict->close();
			return;
		}
		if (acdbOpenAcDbObject(pO, idO, AcDb::kForRead) != Acad::eOk) {
			pEmployeeDict->close();
			pExtDict->close();
			acutPrintf(_T("\nFailed to open the object detail."));
			return;
		}
		AsdkEmployeeDetails* pEmployeeDetails = AsdkEmployeeDetails::cast(pO);
		if (pEmployeeDetails == NULL) {
			acutPrintf(_T("\nNo details found!."));
			pO->close();
			pEmployeeDict->close();
			pExtDict->close();
			return;
		}
		Adesk::Int32 i;
		pEmployeeDetails->iD(i);
		acutPrintf(_T("*Employee's ID: %d\n"), i);
		pEmployeeDetails->cube(i);
		acutPrintf(_T("*Employee's cube number: %d\n"), i);
		TCHAR* st = NULL;
		pEmployeeDetails->firstName(st);
		acutPrintf(_T("*Employee's first name: %s\n"), st);
		delete[] st;
		pEmployeeDetails->lastName(st);
		acutPrintf(_T("*Employee's last name: %s\n"), st);
		delete[] st;

		pO->close();
		pEmployeeDict->close();
		pExtDict->close();
	}


	static void AsdkStep05_REMOVEDETAIL(void)
	{
		ads_name ename;
		ads_point pt;
		if (acedEntSel(_T("Select employee: "), ename, pt) != RTNORM)
			return;
		AcDbObjectId idO;
		if (acdbGetObjectId(idO, ename) != Acad::eOk)
			return;
		AcDbObject* pO;
		if (acdbOpenAcDbObject(pO, idO, AcDb::kForRead) != Acad::eOk)
			return;
		if (!pO->isKindOf(AcDbBlockReference::desc())) {
			acutPrintf(_T("\nThis is not a block reference."));
			pO->close();
			return;
		}
		if ((idO = pO->extensionDictionary()) == AcDbObjectId::kNull) {
			pO->close();
			return;
		}
		pO->close();
		AcDbDictionary* pExtDict;
		if (acdbOpenAcDbObject((AcDbObject*&)pExtDict, idO, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk) {
			acutPrintf(_T("\nFailed to open ext. dictionary."));
			return;
		}
		AcDbDictionary* pEmployeeDict;
		if (pExtDict->getAt(_T("ASDK_EMPLOYEE_DICTIONARY"), idO) == Acad::eKeyNotFound) {
			pExtDict->close();
			return;
		}
		else {
			if (acdbOpenAcDbObject(pO, idO, AcDb::kForWrite) != Acad::eOk) {
				pExtDict->close();
				acutPrintf(_T("\nFailed to open the 'Employee' dictionary."));
				return;
			}
			if ((pEmployeeDict = AcDbDictionary::cast(pO)) == NULL) {
				pO->close();
				pExtDict->close();
				acutPrintf(_T("\nThe entry is not a dictionary"));
				return;
			}
		}
		if (pEmployeeDict->getAt(_T("DETAILS"), idO) != Acad::eOk) {
			pEmployeeDict->close();
			pExtDict->close();
			acutPrintf(_T("\nNo details assigned to that 'Employee' object."));
			return;
		}
		if (acdbOpenAcDbObject(pO, idO, AcDb::kForWrite) != Acad::eOk) {
			pEmployeeDict->close();
			pExtDict->close();
			acutPrintf(_T("\nFailed to open the object detail."));
			return;
		}
		pO->erase();
		pO->close();
		if (pEmployeeDict->numEntries() == 0)
			pEmployeeDict->erase();
		pEmployeeDict->close();
		if (pExtDict->numEntries() == 0)
			pExtDict->erase();
		pExtDict->close();
	}

	static void AsdkStep05_ADDDETAIL(void)
	{
		ads_name ename;
		ads_point pt;
		if (acedEntSel(_T("Select employee: "), ename, pt) != RTNORM)
			return;
		AcDbObjectId idO;
		if (acdbGetObjectId(idO, ename) != Acad::eOk)
			return;
		AcDbObject* pO;
		if (acdbOpenAcDbObject(pO, idO, AcDb::kForWrite) != Acad::eOk)
			return;
		if (!pO->isKindOf(AcDbBlockReference::desc())) {
			acutPrintf(_T("\nThis is not a block reference."));
			pO->close();
			return;
		}
		int id, cubeNumber;
		TCHAR strFirstName[133];
		TCHAR strLastName[133];
		if (acedGetInt(_T("Enter employee ID: "), &id) != RTNORM
			|| acedGetInt(_T("Enter cube number: "), &cubeNumber) != RTNORM
			|| acedGetString(0, _T("Enter employee first name: "), strFirstName) != RTNORM
			|| acedGetString(0, _T("Enter employee last name: "), strLastName) != RTNORM
			) {
			pO->close();
			return;
		}
		if ((idO = pO->extensionDictionary()) == AcDbObjectId::kNull) {
			if (pO->createExtensionDictionary() != Acad::eOk) {
				pO->close();
				acutPrintf(_T("\nFailed to create ext. dictionary."));
				return;
			}
			idO = pO->extensionDictionary();
		}
		pO->close();
		AcDbDictionary* pExtDict;
		if (acdbOpenAcDbObject((AcDbObject*&)pExtDict, idO, AcDb::kForWrite, Adesk::kTrue) != Acad::eOk) {
			acutPrintf(_T("\nFailed to open ext. dictionary."));
			return;
		}
		if (pExtDict->isErased())
			pExtDict->erase(Adesk::kFalse);
		AcDbDictionary* pEmployeeDict;
		if (pExtDict->getAt(_T("ASDK_EMPLOYEE_DICTIONARY"), idO) == Acad::eKeyNotFound) {
			pEmployeeDict = new AcDbDictionary;
			Acad::ErrorStatus es;
			if ((es = pExtDict->setAt(_T("ASDK_EMPLOYEE_DICTIONARY"), pEmployeeDict, idO)) != Acad::eOk) {
				pExtDict->close();
				acutPrintf(_T("\nFailed to create the 'Employee' dictionary."));
				return;
			}
		}
		else {
			if (acdbOpenAcDbObject(pO, idO, AcDb::kForWrite) != Acad::eOk) {
				pExtDict->close();
				acutPrintf(_T("\nFailed to open the 'Employee' dictionary."));
				return;
			}
			if ((pEmployeeDict = AcDbDictionary::cast(pO)) == NULL) {
				pO->close();
				pExtDict->close();
				acutPrintf(_T("\nThe entry is not a dictionary"));
				return;
			}
		}
		pExtDict->close();
		if (pEmployeeDict->getAt(_T("DETAILS"), idO) == Acad::eOk) {
			pEmployeeDict->close();
			acutPrintf(_T("\nDetails already assign to that 'Employee' object."));
			return;
		}
		AsdkEmployeeDetails* pEmployeeDetails = new AsdkEmployeeDetails;
		pEmployeeDetails->setID(id);
		pEmployeeDetails->setCube(cubeNumber);
		pEmployeeDetails->setFirstName(strFirstName);
		pEmployeeDetails->setLastName(strLastName);
		if (pEmployeeDict->setAt(_T("DETAILS"), pEmployeeDetails, idO) != Acad::eOk) {
			delete pEmployeeDetails;
			acutPrintf(_T("\nFailed to add details to that object."));
			pEmployeeDict->close();
			return;

		}
		acutPrintf(_T("\nDetails successfully added!"));
		pEmployeeDict->close();
		pEmployeeDetails->close();
	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep05App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, AsdkStep05, _CREATE, CREATE, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, AsdkStep05, _SETLAYER, SETLAYER, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, AsdkStep05, _LISTDETAILS, LISTDETAILS, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, AsdkStep05, _REMOVEDETAIL, REMOVEDETAIL, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, AsdkStep05, _ADDDETAIL, ADDDETAIL, ACRX_CMD_TRANSPARENT, NULL)