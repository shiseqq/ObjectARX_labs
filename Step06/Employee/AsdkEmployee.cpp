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
//----- AsdkEmployee.cpp : Implementation of AsdkEmployee
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "AsdkEmployee.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 AsdkEmployee::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	AsdkEmployee, AcDbEllipse,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kAllAllowedBits, ASDKEMPLOYEE,
	_T("ASDKEMPLOYEEAPP")
	_T("|Product Desc:     A description for your object")
	_T("|Company:          Your company name")
	_T("|WEB Address:      Your company WEB site address")
)

//-----------------------------------------------------------------------------
AsdkEmployee::AsdkEmployee() : AcDbEllipse(AcGePoint3d(),
	AcGeVector3d(0, 0, 1),
	AcGeVector3d(1, 0, 0), 1)
{
	// TODO: do your initialization. 
	m_firstName = m_lastName = NULL;

}

//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus AsdkEmployee::dwgOutFields(AcDbDwgFiler* pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(AsdkEmployee::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	// Write the data members.
	pFiler->writeItem(m_lastName);
	pFiler->writeItem(m_firstName);
	pFiler->writeItem(m_cube);
	pFiler->writeItem(m_ID);

	return (pFiler->filerStatus());
}

Acad::ErrorStatus AsdkEmployee::dwgInFields(AcDbDwgFiler* pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbObject::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > AsdkEmployee::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < AsdkEmployee::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	//------------------------------------------------------------
	// Read the data members.
	switch (version)
	{
	case (1):
		pFiler->readItem(&m_lastName);
		pFiler->readItem(&m_firstName);
		pFiler->readItem(&m_cube);
		pFiler->readItem(&m_ID);
		break;
	}
	//------------------------------------------------------------

	return (pFiler->filerStatus());
}

//- Dxf Filing protocol
Acad::ErrorStatus AsdkEmployee::dxfOutFields(AcDbDxfFiler* pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbObject::dxfOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	es = pFiler->writeItem(AcDb::kDxfSubclass, _RXST("AsdkEmployee"));
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(kDxfInt32, AsdkEmployee::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	//------------------------------------------------------------
	//Write out the data members
	pFiler->writeItem(AcDb::kDxfXTextString, m_lastName);
	pFiler->writeItem(AcDb::kDxfXTextString + 1, m_firstName);
	pFiler->writeItem(AcDb::kDxfInt32, m_cube);
	pFiler->writeItem(AcDb::kDxfInt32 + 1, m_ID);
	//------------------------------------------------------------

	return (pFiler->filerStatus());
}

Acad::ErrorStatus AsdkEmployee::dxfInFields(AcDbDxfFiler* pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbObject::dxfInFields(pFiler);
	if (es != Acad::eOk || !pFiler->atSubclassData(_RXST("AsdkEmployee")))
		return (pFiler->filerStatus());
	//----- Object version number needs to be read first
	struct resbuf rb;
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt32) {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32);
		return (pFiler->filerStatus());
	}
	Adesk::UInt32 version = (Adesk::UInt32)rb.resval.rlong;
	if (version > AsdkEmployee::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < AsdkEmployee::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;

	//----- Read params in non order dependant manner
	//- read all the member variables
	while (es == Acad::eOk && (es = pFiler->readResBuf(&rb)) == Acad::eOk) {
		switch (rb.restype) {
			//----- Read params by looking at their DXF code
		case AcDb::kDxfXTextString:
			if (m_lastName != NULL)
				free(m_lastName);
			m_lastName = _tcsdup(rb.resval.rstring);
			break;
		case  AcDb::kDxfXTextString + 1:
			if (m_firstName != NULL)
				free(m_firstName);
			m_firstName = _tcsdup(rb.resval.rstring);
			break;
		case  AcDb::kDxfInt32:
			m_cube = rb.resval.rlong;
			break;
		case AcDb::kDxfInt32 + 1:
			m_ID = rb.resval.rlong;
			break;
		default:
			//----- An unrecognized group. Push it back so that the subclass can read it again.
			pFiler->pushBackItem();
			es = Acad::eEndOfFile;
			break;
		}
	}
	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if (es != Acad::eEndOfFile)
		return (Acad::eInvalidResBuf);

	return (pFiler->filerStatus());
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean AsdkEmployee::subWorldDraw(AcGiWorldDraw* mode) {
	assertReadEnabled();

	//----- Draw the AcDbEllipse entity
	AcDbEllipse::subWorldDraw(mode);

	//----- Write the Employee ID and Name
	TCHAR buffer[255];
	_stprintf(buffer, _T("%d (cube#: %d)"), m_ID, m_cube);
	_stprintf(buffer, _T("%d (cube#: %d)"), m_ID, m_cube);
	mode->geometry().text(center(), normal(), majorAxis(), minorAxis().length() / 2, 1.0, 0.0, buffer);
	_stprintf(buffer, _T("%s %s"), m_firstName, m_lastName);
	mode->geometry().text(center() - minorAxis() / 2, normal(), majorAxis(), minorAxis().length() / 2, 1.0, 0.0, buffer);

	//----- Returns Adesk::kTrue to not call viewportDraw()
	return (Adesk::kTrue);
}


Acad::ErrorStatus AsdkEmployee::lastName(TCHAR*& lastName)
{
	assertReadEnabled();
	lastName = _tcsdup(m_lastName);
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployee::setLastName(const TCHAR* lastName)
{
	assertWriteEnabled();
	if (m_lastName)
		free(m_lastName);
	m_lastName = _tcsdup(lastName);
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployee::firstName(TCHAR*& firstName)
{
	assertReadEnabled();
	firstName = _tcsdup(m_firstName);
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployee::setFirstName(const TCHAR* firstName)
{
	assertWriteEnabled();
	if (m_firstName)
		free(m_firstName);
	m_firstName = _tcsdup(firstName);
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployee::cube(Adesk::Int32& cube)
{
	assertReadEnabled();
	cube = m_cube;
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployee::setCube(const Adesk::Int32 cube)
{
	assertWriteEnabled();
	m_cube = cube;
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployee::iD(Adesk::Int32& ID)
{
	assertReadEnabled();
	ID = m_ID;
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployee::setID(const Adesk::Int32 ID)
{
	assertWriteEnabled();
	m_ID = ID;
	return Acad::eOk;
}



