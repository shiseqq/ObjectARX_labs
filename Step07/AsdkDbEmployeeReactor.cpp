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
//----- AsdkDbEmployeeReactor.cpp : Implementation of AsdkDbEmployeeReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "AsdkDbEmployeeReactor.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(AsdkDbEmployeeReactor, AcDbDatabaseReactor, 1)

//-----------------------------------------------------------------------------
AsdkDbEmployeeReactor::AsdkDbEmployeeReactor (AcDbDatabase *pDb) : AcDbDatabaseReactor(), mpDatabase(pDb) {
	if ( pDb )
		pDb->addReactor (this) ;
}

//-----------------------------------------------------------------------------
AsdkDbEmployeeReactor::~AsdkDbEmployeeReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void AsdkDbEmployeeReactor::Attach (AcDbDatabase *pDb) {
	Detach () ;
	if ( mpDatabase == NULL ) {
		if ( (mpDatabase =pDb) != NULL )
			pDb->addReactor (this) ;
	}
}

void AsdkDbEmployeeReactor::Detach () {
	if ( mpDatabase ) {
		mpDatabase->removeReactor (this) ;
		mpDatabase =NULL ;
	}
}


void AsdkDbEmployeeReactor::objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	// dbObj should be of type AcDbBlockReference
	// whose name is "EMPLOYEE". This check is not absolutely necessary,
	// since our application is adding the reactor to employee objects
	// only, we know this notificatin can only come from such an object.

	AcDbBlockReference* pInsert = AcDbBlockReference::cast(dbObj);
	if (!pInsert)
		return;

	// Get the ObjectId of the BlockTableRecord where the reference is defined
	AcDbObjectId blockId = pInsert->blockTableRecord();
	AcDbBlockTableRecord* pBlockTableRecord;
	if (acdbOpenAcDbObject((AcDbObject*&)pBlockTableRecord, blockId, AcDb::kForRead) != Acad::eOk) {
		acutPrintf(_T("\nCannot open block table record!"));
		return;
	}

	const TCHAR* blockName;
	pBlockTableRecord->getName(blockName);
	pBlockTableRecord->close();

	if (_tcscmp(blockName, _T("EMPLOYEE")))
		return; // Not an employee

	// Ok, this is an employee. Attach the object reactor
	dbObj->addReactor(pEmployeeReactor);
}