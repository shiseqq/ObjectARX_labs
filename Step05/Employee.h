#pragma once
#include "StdAfx.h"

class Employee {
public:
	Employee()
	{
		m_pFace->setColorIndex(2);
		m_pLeftEye->setColorIndex(5);
		m_pRightEye->setColorIndex(5);
		m_pMouth->setColorIndex(1);
	}

	Acad::ErrorStatus addEmpToBtr(AcDbBlockTableRecord* pBlockTableRecord) {
		Acad::ErrorStatus es;
		if ((es = pBlockTableRecord->appendAcDbEntity(m_pFace)) != Acad::eOk) {
			delete m_pFace;
			delete m_pLeftEye;
			delete m_pRightEye;
			delete m_pMouth;
			pBlockTableRecord->erase();
			pBlockTableRecord->close();
			return (es);
		}
		m_pFace->close();

		if ((es = pBlockTableRecord->appendAcDbEntity(m_pLeftEye)) != Acad::eOk) {
			delete m_pLeftEye;
			delete m_pRightEye;
			delete m_pMouth;
			pBlockTableRecord->erase();
			pBlockTableRecord->close();
			return (es);
		}
		m_pLeftEye->close();

		if ((es = pBlockTableRecord->appendAcDbEntity(m_pRightEye)) != Acad::eOk) {
			delete m_pRightEye;
			delete m_pMouth;
			pBlockTableRecord->erase();
			pBlockTableRecord->close();
			return (es);
		}
		m_pRightEye->close();

		if ((es = pBlockTableRecord->appendAcDbEntity(m_pMouth)) != Acad::eOk) {
			delete m_pMouth;
			pBlockTableRecord->erase();
			pBlockTableRecord->close();
			return (es);
		}
		m_pMouth->close();
		return (es);
	}

private:
	AcDbCircle* m_pFace = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, 1.0);
	AcDbCircle* m_pLeftEye = new AcDbCircle(AcGePoint3d(0.33, 0.25, 0.0), AcGeVector3d::kZAxis, 0.1);
	AcDbCircle* m_pRightEye = new AcDbCircle(AcGePoint3d(-0.33, 0.25, 0.0), AcGeVector3d::kZAxis, 0.1);
	AcDbArc* m_pMouth = new AcDbArc(AcGePoint3d(0, -1.5, 0), 1.0, 3.141592 - (3.141592 * 0.7), 3.141592 - (3.141592 * 0.3));
};