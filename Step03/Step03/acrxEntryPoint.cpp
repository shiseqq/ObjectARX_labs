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
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep03App : public AcRxArxApp {

public:
	CStep03App () : AcRxArxApp () {}

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

	static void AsdkStep03_create(void) {
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

	static void AsdkStep03_setLayer(void) {
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
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep03App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, AsdkStep03, _create, create, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, AsdkStep03, _setLayer, setLayer, ACRX_CMD_TRANSPARENT, NULL)

