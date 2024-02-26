#include "StdAfx.h"
#include "utilities.h"

Acad::ErrorStatus createLayer(const TCHAR* layerName, AcDbObjectId& layerId) {

	Acad::ErrorStatus es;
	AcDbLayerTable* pTable;

	layerId = AcDbObjectId::kNull;
	if ((es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pTable, AcDb::kForRead)) != Acad::eOk) {
		return es;
	}
	if ((es = pTable->getAt(layerName, layerId, Adesk::kFalse)) == Acad::eOk) {
		pTable->close();
		return es;
	}
	AcDbLayerTableRecord* pLTRec = new AcDbLayerTableRecord;
	pLTRec->setName(layerName);
	if ((es = pTable->upgradeOpen()) == Acad::eOk) {
		es = pTable->add(layerId, pLTRec);
		pLTRec->close();
	}
	else {
		delete pLTRec;
	}
	pTable->close();
	return es;
}

Acad::ErrorStatus createBlockRecord(const TCHAR* name) {
	AcDbBlockTable* pBlockTable;
	Acad::ErrorStatus es;
	if ((es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead)) != Acad::eOk)
		return es;

	if (pBlockTable->has(name) == Adesk::kTrue) {
		pBlockTable->close();
		return Acad::eDuplicateKey;
	}
	AcDbBlockTableRecord* pBlockTableRecord = new AcDbBlockTableRecord;
	pBlockTableRecord->setName(name);
	pBlockTableRecord->setOrigin(AcGePoint3d::kOrigin);
	if ((es = pBlockTable->upgradeOpen()) != Acad::eOk) {
		delete pBlockTableRecord;
		pBlockTable->close();
		return (es);
	}
	if ((es = pBlockTable->add(pBlockTableRecord)) != Acad::eOk) {
		pBlockTable->close();
		delete pBlockTableRecord;
		return (es);
	}
	pBlockTable->close();
	AcDbCircle* pFace = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, 1.0);
	AcDbCircle* pLeftEye = new AcDbCircle(AcGePoint3d(0.33, 0.25, 0.0), AcGeVector3d::kZAxis, 0.1);
	AcDbCircle* pRightEye = new AcDbCircle(AcGePoint3d(-0.33, 0.25, 0.0), AcGeVector3d::kZAxis, 0.1);
	AcDbArc* pMouth = new AcDbArc(AcGePoint3d(0, -1.5, 0), 1.0, 3.141592 - (3.141592 * 0.7), 3.141592 - (3.141592 * 0.3));
	pFace->setColorIndex(2);
	pLeftEye->setColorIndex(5);
	pRightEye->setColorIndex(5);
	pMouth->setColorIndex(1);
	if ((es = pBlockTableRecord->appendAcDbEntity(pFace)) != Acad::eOk) {
		delete pFace;
		delete pLeftEye;
		delete pRightEye;
		delete pMouth;
		pBlockTableRecord->erase();
		pBlockTableRecord->close();
		return (es);
	}
	pFace->close();

	if ((es = pBlockTableRecord->appendAcDbEntity(pLeftEye)) != Acad::eOk) {
		delete pLeftEye;
		delete pRightEye;
		delete pMouth;
		pBlockTableRecord->erase();
		pBlockTableRecord->close();
		return (es);
	}
	pLeftEye->close();

	if ((es = pBlockTableRecord->appendAcDbEntity(pRightEye)) != Acad::eOk) {
		delete pRightEye;
		delete pMouth;
		pBlockTableRecord->erase();
		pBlockTableRecord->close();
		return (es);
	}
	pRightEye->close();

	if ((es = pBlockTableRecord->appendAcDbEntity(pMouth)) != Acad::eOk) {
		delete pMouth;
		pBlockTableRecord->erase();
		pBlockTableRecord->close();
		return (es);
	}
	pMouth->close();

	pBlockTableRecord->close();

	return (Acad::eOk);
}
