//********************************************
// Transform.cpp
//********************************************
// class CTransform
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 23/02/98
// Modified : 23/02/98
//********************************************

#include "stdafx.h"
#include "Transform.h"


//********************************************
// Constructor
//********************************************
CTransform::CTransform()
{
	SetScale(CVector3d(1.0f,1.0f,1.0f));
	SetTranslation(CVector3d(0.0f,0.0f,0.0f));
	SetValueRotation(0.0f);
}

//********************************************
// Constructor
//********************************************
void CTransform::Clear(void)
{
	SetScale(CVector3d(1.0f,1.0f,1.0f));
	SetTranslation(CVector3d(0.0f,0.0f,0.0f));
	SetValueRotation(0.0f);
}

//********************************************
// Copy
//********************************************
void CTransform::Copy(CTransform &transform)
{
	SetScale(transform.GetScale());
	SetTranslation(transform.GetTranslation());
	SetRotation(transform.GetRotation());
	SetValueRotation(transform.GetValueRotation());
}

//********************************************
// Copy
//********************************************
void CTransform::Copy(CTransform *pTransform)
{
	SetScale(pTransform->GetScale());
	SetTranslation(pTransform->GetTranslation());
	SetRotation(pTransform->GetRotation());
	SetValueRotation(pTransform->GetValueRotation());
}

void CTransform::SetTransformFromBoundingBox(Vector3D* m_lower, Vector3D* m_upper)
{
	Vector3D m_center;
	m_center = *m_lower + *m_upper;
	m_center /= -2;

	float length = m_upper->x - m_lower->x;
	if (length < m_upper->y - m_lower->y)
		length = m_upper->y - m_lower->y;
	if (length < m_upper->z - m_lower->z)
		length = m_upper->z - m_lower->z;

	length = length / 2;

	// CTransform transform;

	 //translation
	CVector3d translation(m_center[0], m_center[1], m_center[2]);
	SetTranslation(&translation);

	//scale
	CVector3d scale(1 / length, 1 / length, 1 / length);
	SetScale(&scale);
}




//////////////////////////////////////////////
// DATAS
//////////////////////////////////////////////
void CTransform::Read(std::ifstream &file)
{
	file >> m_Scale.m_x >> m_Scale.m_y >> m_Scale.m_z;
	file >> m_Rotation.m_x >> m_Rotation.m_y >> m_Rotation.m_z;
	file >> m_Translation.m_x >> m_Translation.m_y >> m_Translation.m_z;
	file >> m_ValueRotation;
}
void CTransform::Save(std::ofstream &file)
{
	file << m_Scale.m_x << " " << m_Scale.m_y << " "<< m_Scale.m_z << "\n";
	file << m_Rotation.m_x << " " << m_Rotation.m_y << " "<< m_Rotation.m_z << "\n";
	file << m_Translation.m_x << " " << m_Translation.m_y << " "<< m_Translation.m_z << "\n";
	file << m_ValueRotation << "\n";
}


// ** EOF **
