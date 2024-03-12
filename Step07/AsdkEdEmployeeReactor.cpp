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
//----- AsdkEdEmployeeReactor.cpp : Implementation of AsdkEdEmployeeReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "AsdkEdEmployeeReactor.h"

ACRX_CONS_DEFINE_MEMBERS(AsdkEmployeeReactor, AcDbObjectReactor, 1)

//-----------------------------------------------------------------------------
void AsdkEdEmployeeReactor::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void AsdkEdEmployeeReactor::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcEditor *AsdkEdEmployeeReactor::Subject () const {
	return (acedEditor) ;
}

bool AsdkEdEmployeeReactor::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}

void AsdkEdEmployeeReactor::commandWillStart(const ACHAR* cmdStr)
{
	// Check whether one of our monitored commands starts
	if (_tcscmp(cmdStr, _T("MOVE")) && _tcscmp(cmdStr, _T("ROTATE"))
		&& _tcscmp(cmdStr, _T("SCALE")) && _tcscmp(cmdStr, _T("STRETCH"))
		&& _tcscmp(cmdStr, _T("GRIP_STRETCH"))
		) {
		return;
	}

	// Set the global variable...
	DocVars.docData().m_editCommand = true;
	DocVars.docData().m_doRepositioning = false;

	// ...and delete all stored information
	DocVars.docData().m_changedObjects.setLogicalLength(0);
	DocVars.docData().m_employeePositions.setLogicalLength(0);
}

// -----------------------------------------------------------------------------
void AsdkEdEmployeeReactor::commandEnded(const ACHAR* cmdStr)
{
	// Was one of our monitored commands active
	if (DocVars.docData().m_editCommand == false)
		return;

	DocVars.docData().m_editCommand = false;

	// disable the object reactor
	// (so the object reactor knows that we are changing the object)
	DocVars.docData().m_doRepositioning = true;

	// Reset to previous position
	AcDbBlockReference* pInsert;
	for (int i = 0; i < DocVars.docData().m_changedObjects.length(); ++i) {
		if (acdbOpenObject(pInsert, DocVars.docData().m_changedObjects.at(i), AcDb::kForWrite) == Acad::eOk) {
			AcGePoint3d newPos = pInsert->position();
			AcGePoint3d oldPos = DocVars.docData().m_employeePositions.at(i);
			// Resetting the position to the original one
			if (newPos != oldPos) {
				pInsert->setPosition(oldPos);
				acutPrintf(_T("\nEMPLOYEE has been reset to its original location."));
			}
			pInsert->close();
		}
	}
}