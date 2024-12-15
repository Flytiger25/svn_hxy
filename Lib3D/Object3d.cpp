//********************************************
// Object3d.cpp
//********************************************
// class CObject3d, the base 3d object
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 17/12/97
// Modified : 15/01/98
//********************************************

#include "stdafx.h"

#include "Base3d.h"
#include "Object3d.h"


//********************************************
// GetType
//********************************************
int CObject3d::GetType()
{
	return type;
}

//********************************************
// glBuildList
//********************************************
int CObject3d::glBuildList()
{
	//AfxMessageBox("CObject3d::BuildList : virtual function");
	return 1;
}

//********************************************
// glDraw
//********************************************
int CObject3d::glDraw()
{
	AfxMessageBox("CObject3d::BuildList : virtual function");
	return 1;
}
int CObject3d::SetTransform(float m_xTranslation, float m_yTranslation, float m_zTranslation,
	float m_xRotation, float m_yRotation, float m_zRotation,
	float m_xScaling, float m_yScaling, float m_zScaling)
{
	if (m_IsMove)
	{
		this->m_xTranslation = m_xTranslation;
		this->m_yTranslation = m_yTranslation;
		this->m_zTranslation = m_zTranslation;
		this->m_xRotation = m_xRotation;
		this->m_yRotation = m_yRotation;
		this->m_zRotation = m_zRotation;
		this->m_xScaling = m_xScaling;
		this->m_yScaling = m_yScaling;
		this->m_zScaling = m_zScaling;
	}

	return 1;
}
void CObject3d::UpdateTreeControl(CTreeCtrl* pTreeControl)
{
	char name[200];
	CString path = filename;
	path = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	sprintf(name, "%s", path);
	if (pParentObject)
		pTreeItem = pTreeControl->InsertItem(name, pParentObject->pTreeItem);
	else
		pTreeItem = pTreeControl->InsertItem(name, NULL, NULL);
}
// ** EOF **



