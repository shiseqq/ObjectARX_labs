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
//----- AsdkEmployeeReactor.h : Declaration of the AsdkEmployeeReactor
//-----------------------------------------------------------------------------
#pragma once

#ifdef STEP07_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"

//-----------------------------------------------------------------------------
//----- Note: Uncomment the DLLIMPEXP symbol below if you wish exporting
//----- your class to other ARX/DBX modules
class /*DLLIMPEXP*/ AsdkEmployeeReactor : public AcDbObjectReactor {

public:
	ACRX_DECLARE_MEMBERS(AsdkEmployeeReactor) ;

public:
	//-----------------------------------------------------------------------------
	AsdkEmployeeReactor::AsdkEmployeeReactor() : AcDbObjectReactor() {
	}

	//-----------------------------------------------------------------------------
	virtual AsdkEmployeeReactor::~AsdkEmployeeReactor() {
	}

	virtual void openedForModify(const AcDbObject* dbObj)
	{
		if (DocVars.docData().m_doRepositioning)
			return;

		// If none of our monitored commands is active return
		if (!DocVars.docData().m_editCommand)
			return;

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

		// Store the objectID and the position
		DocVars.docData().m_changedObjects.append(pInsert->objectId());
		DocVars.docData().m_employeePositions.append(pInsert->position());
	}
} ;

ACDB_REGISTER_OBJECT_ENTRY_AUTO(AsdkEmployeeReactor)
