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
//----- AsdkEmployeeDetails.cpp : Implementation of AsdkEmployeeDetails
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "AsdkEmployeeDetails.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 AsdkEmployeeDetails::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	AsdkEmployeeDetails, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, ASDKEMPLOYEEDETAILS,
ASDKEMPLOYEEDETAILSAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
//-----constructor
AsdkEmployeeDetails::AsdkEmployeeDetails() :AcDbObject()
{
	//-----do your initialization.
	m_firstName = NULL;
	m_lastName = NULL;

}


AsdkEmployeeDetails::~AsdkEmployeeDetails () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus AsdkEmployeeDetails::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (AsdkEmployeeDetails::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus AsdkEmployeeDetails::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > AsdkEmployeeDetails::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < AsdkEmployeeDetails::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	return (pFiler->filerStatus ()) ;
}

//- Dxf Filing protocol
Acad::ErrorStatus AsdkEmployeeDetails::dxfOutFields (AcDbDxfFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbObject::dxfOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	es =pFiler->writeItem (AcDb::kDxfSubclass, _RXST("AsdkEmployeeDetails")) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (kDxfInt32, AsdkEmployeeDetails::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus AsdkEmployeeDetails::dxfInFields (AcDbDxfFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbObject::dxfInFields (pFiler) ;
	if ( es != Acad::eOk || !pFiler->atSubclassData (_RXST("AsdkEmployeeDetails")) )
		return (pFiler->filerStatus ()) ;
	//----- Object version number needs to be read first
	struct resbuf rb ;
	pFiler->readItem (&rb) ;
	if ( rb.restype != AcDb::kDxfInt32 ) {
		pFiler->pushBackItem () ;
		pFiler->setError (Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32) ;
		return (pFiler->filerStatus ()) ;
	}
	Adesk::UInt32 version =(Adesk::UInt32)rb.resval.rlong ;
	if ( version > AsdkEmployeeDetails::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < AsdkEmployeeDetails::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while ( es == Acad::eOk && (es =pFiler->readResBuf (&rb)) == Acad::eOk ) {
		switch ( rb.restype ) {
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
				pFiler->pushBackItem () ;
				es =Acad::eEndOfFile ;
				break ;
		}
	}
	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if ( es != Acad::eEndOfFile )
		return (Acad::eInvalidResBuf) ;

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus AsdkEmployeeDetails::firstName(TCHAR*& firstName)
{
	assertReadEnabled();
	firstName = _tcsdup(m_firstName);
	return Acad::eOk;

}

Acad::ErrorStatus AsdkEmployeeDetails::setFirstName(const TCHAR* firstName)
{

	assertWriteEnabled();
	if (m_firstName)
		free(m_firstName);
	m_firstName = _tcsdup(firstName);
	return Acad::eOk;

}

Acad::ErrorStatus AsdkEmployeeDetails::lastName(TCHAR*& lastName)
{
	assertReadEnabled();
	lastName = _tcsdup(m_lastName);
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployeeDetails::setLastName(const TCHAR* lastName)
{
	assertWriteEnabled();
	if (m_lastName)
		free(m_lastName);
	m_lastName = _tcsdup(lastName);
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployeeDetails::cube(Adesk::Int32& cube)
{
	assertReadEnabled();
	cube = m_cube;
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployeeDetails::setCube(const Adesk::Int32 cube)
{
	assertWriteEnabled();
	m_cube = cube;
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployeeDetails::iD(Adesk::Int32& ID)
{
	assertReadEnabled();
	ID = m_ID;
	return Acad::eOk;
}

Acad::ErrorStatus AsdkEmployeeDetails::setID(const Adesk::Int32 ID)
{
	assertWriteEnabled();
	m_ID = ID;
	return Acad::eOk;
}
