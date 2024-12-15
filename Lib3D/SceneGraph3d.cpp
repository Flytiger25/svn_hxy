//********************************************
// SceneGraph.cpp
//********************************************
// class CSceneGraph
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 15/01/98
// Modified : 15/01/98
//********************************************

#include "stdafx.h"
#include "Base3d.h"
#include "SceneGraph3d.h"
#include "math.h"
#include "../FormView3.h"
#include "SDIViewSwitchDoc.h"
#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a>b?a:b)
//////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////

//********************************************
// Constructor
//********************************************
CSceneGraph3d::CSceneGraph3d()
{
	m_pIndexTextureBinding = NULL;
	m_ListDone = 0;
	m_Modified = 0;
}

//********************************************
// Destructor
//********************************************
CSceneGraph3d::~CSceneGraph3d()
{
	Free();
}

//********************************************
// Destructor
//********************************************
void CSceneGraph3d::Free(void)
{
	// Textures
	if (m_ArrayTexture.GetSize())
	{
		::glDeleteTextures(m_ArrayTexture.GetSize(), m_pIndexTextureBinding);
		if (m_pIndexTextureBinding != NULL)
		{
			delete[] m_pIndexTextureBinding;
			m_pIndexTextureBinding = NULL;
		}
	}
	m_ArrayTexture.Free();
	// Objects
	m_ArrayObject3d.Free();
}

//////////////////////////////////////////////
// DATAS
//////////////////////////////////////////////

//********************************************
// Add
//********************************************
void CSceneGraph3d::Add(CObject3d* pObject3d)
{
	m_ArrayObject3d.Add(pObject3d);
}
//
extern CSDIViewSwitchDoc* pDoc;
void CSceneGraph3d::ZoomAllToFitCurrentMesh()
{
	CObject3d* pObject;
	Point3D lower, upper;
	CVector3d lower_, upper_;
	lower_.Set(1e10, 1e10, 1e10);
	upper_.Set(-1e10, -1e10, -1e10);
	//for (int i = 0; i < m_ArrayObject3d.GetSize(); i++)
	{
		pObject = m_ArrayObject3d.GetAt(pDoc->m_CurrentObjectIndex);
		pObject->ComputeBoundingBox(lower, upper);
		lower_.Set(MIN(lower[0], lower_[0]), MIN(lower[1], lower_[1]), MIN(lower[2], lower_[2]));
		upper_.Set(MAX(upper[0], upper_[0]), MAX(upper[1], upper_[1]), MAX(upper[2], upper_[2]));
	}
	CTransform transform;
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation = lower_ + upper_;
	m_translation /= -2;
	float length = upper_.x() - lower_.x();
	float length1 = upper_.y() - lower_.y();
	if (length < upper_.y() - lower_.y())
		length = upper_.y() - lower_.y();
	if (length < upper_.z() - lower_.z())
		length = upper_.z() - lower_.z();

	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1 / length, 1 / length, 1 / length);
	transform.SetScale(&m_Scale);
	for (int i = 0; i < m_ArrayObject3d.GetSize(); i++)
	{
		pObject = m_ArrayObject3d.GetAt(i);
		pObject->m_Transform.Copy(transform);
	}
}
void CSceneGraph3d::ZoomToFitCurrentMesh()
{
	CObject3d* pObject;
	Point3D lower, upper;
	CVector3d lower_, upper_;
	lower_.Set(1e10, 1e10, 1e10);
	upper_.Set(-1e10, -1e10, -1e10);
	//for (int i = 0; i < m_ArrayObject3d.GetSize(); i++)
	{
		pObject = m_ArrayObject3d.GetAt(pDoc->m_CurrentObjectIndex);
		pObject->ComputeBoundingBox(lower, upper);
		lower_.Set(MIN(lower[0], lower_[0]), MIN(lower[1], lower_[1]), MIN(lower[2], lower_[2]));
		upper_.Set(MAX(upper[0], upper_[0]), MAX(upper[1], upper_[1]), MAX(upper[2], upper_[2]));
	}
	CTransform transform;
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation = lower_ + upper_;
	m_translation /= -2;
	float length = upper_.x() - lower_.x();
	float length1 = upper_.y() - lower_.y();
	if (length < upper_.y() - lower_.y())
		length = upper_.y() - lower_.y();
	if (length < upper_.z() - lower_.z())
		length = upper_.z() - lower_.z();

	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1 / length, 1 / length, 1 / length);
	transform.SetScale(&m_Scale);
	//for (int i = 0; i < m_ArrayObject3d.GetSize(); i++)
	{
		pObject->m_Transform.Copy(transform);
	}
}
//
void CSceneGraph3d::ZoomAll()
{
	CObject3d* pObject;
	Point3D lower, upper;
	CVector3d lower_, upper_;
	lower_.Set(1e10, 1e10, 1e10);
	upper_.Set(-1e10, -1e10, -1e10);
	for (int i = 0; i < m_ArrayObject3d.GetSize(); i++)
	{
		pObject = m_ArrayObject3d.GetAt(i);
		pObject->ComputeBoundingBox(lower, upper);
		lower_.Set(MIN(lower[0], lower_[0]), MIN(lower[1], lower_[1]), MIN(lower[2], lower_[2]));
		upper_.Set(MAX(upper[0], upper_[0]), MAX(upper[1], upper_[1]), MAX(upper[2], upper_[2]));
	}
	CTransform transform;
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation = lower_ + upper_;
	m_translation /= -2;
	float length = upper_.x() - lower_.x();
	float length1 = upper_.y() - lower_.y();
	if (length < upper_.y() - lower_.y())
		length = upper_.y() - lower_.y();
	if (length < upper_.z() - lower_.z())
		length = upper_.z() - lower_.z();

	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1 / length, 1 / length, 1 / length);
	transform.SetScale(&m_Scale);
	for (int i = 0; i < m_ArrayObject3d.GetSize(); i++)
	{
		pObject = m_ArrayObject3d.GetAt(i);
		pObject->m_Transform.Copy(transform);
	}
}

void CSceneGraph3d::InsertAt(int index, CObject3d* pObject3d)
{
	m_ArrayObject3d.InsertAt(index, pObject3d);
}
void CSceneGraph3d::SetAt(int index, CObject3d* pObject3d)
{
	m_ArrayObject3d.SetAt(index, pObject3d);
}

//********************************************
// Add
//********************************************
void CSceneGraph3d::RemoveAt(int index)
{
	m_ArrayObject3d.RemoveAt(index);
}

//********************************************
// GetAt
//********************************************
CObject3d* CSceneGraph3d::GetAt(int index)
{
	ASSERT(index < m_ArrayObject3d.GetSize());
	return m_ArrayObject3d[index];
}

//********************************************
// Operator []
//********************************************
CObject3d* CSceneGraph3d::operator[](int index)
{
	ASSERT(index < m_ArrayObject3d.GetSize());
	return m_ArrayObject3d[index];
}




//////////////////////////////////////////////
// OPENGL
//////////////////////////////////////////////

void CSceneGraph3d::SetTransform(float m_xTranslation, float m_yTranslation, float m_zTranslation,
	float m_xRotation, float m_yRotation, float m_zRotation,
	float m_xScaling, float m_yScaling, float m_zScaling)
{
	unsigned int size = m_ArrayObject3d.GetSize();
	for (unsigned int i = 0; i < size; i++)
	{
		CObject3d* pObject3d = m_ArrayObject3d[i];
		pObject3d->SetTransform(m_xTranslation, m_yTranslation, m_zTranslation,
			m_xRotation, m_yRotation, m_zRotation,
			m_xScaling, m_yScaling, m_zScaling);
	}
}
//********************************************
// glDraw
//********************************************
void CSceneGraph3d::glDraw()
{
	unsigned int size = m_ArrayObject3d.GetSize();
	for (unsigned int i = 0; i < size; i++)
	{
		CObject3d* pObject3d = m_ArrayObject3d[i];
		pObject3d->glDraw();
	}
}

void CSceneGraph3d::glBuildList()
{
	unsigned int size = m_ArrayObject3d.GetSize();
	for (unsigned int i = 0; i < size; i++)
	{
		CObject3d* pObject3d = m_ArrayObject3d[i];
		pObject3d->glDraw();
	}

	//set flag
	m_ListDone = 1;
	m_Modified = 0;
}
//********************************************
// glDraw_Accelerate
//********************************************
void CSceneGraph3d::glDraw_Accelerate()
{
	// Build list at first
	if (!m_ListDone || m_Modified)
		glBuildList();

	// Search for a new list
	if (::glIsList(m_ListOpenGL) == GL_TRUE)
	{
		::glCallList(m_ListOpenGL);
	}
}
//////////////////////////////////////////////
// MISC
//////////////////////////////////////////////

//********************************************
// BuildAdjacency
// For each mesh
//********************************************
int CSceneGraph3d::BuildAdjacency()
{
	int size = m_ArrayObject3d.GetSize();
	if (size == 0)
		return 0;

	TRACE("Scene %x : Start BuildAdjacency...\n", this);
	TRACE("  NbObject : %d\n", size);
	for (int i = 0; i < size; i++)
	{
		CObject3d* pObject3d = m_ArrayObject3d[i];
		if (pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d* pMesh = (CMesh3d*)m_ArrayObject3d[i];
		pMesh->BuildAdjacency();
	}
	TRACE("Scene %x : End BuildAdjacency...\n", this);

	return 1;
}

//********************************************
// CalculateNormalPerVertex
// For each mesh
//********************************************
int CSceneGraph3d::CalculateNormalPerVertex()
{
	int size = m_ArrayObject3d.GetSize();
	if (size == 0)
		return 0;

	for (int i = 0; i < size; i++)
	{
		CObject3d* pObject3d = m_ArrayObject3d[i];
		if (pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d* pMesh = (CMesh3d*)m_ArrayObject3d[i];
		pMesh->CalculateNormalPerVertex();
	}

	return 1;
}

//********************************************
// CalculateNormalPerFace
// For each mesh
//********************************************
int CSceneGraph3d::CalculateNormalPerFace()
{
	int size = m_ArrayObject3d.GetSize();
	if (size == 0)
		return 0;

	for (int i = 0; i < size; i++)
	{
		CObject3d* pObject3d = m_ArrayObject3d[i];
		if (pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d* pMesh = (CMesh3d*)m_ArrayObject3d[i];
		pMesh->CalculateNormalPerFace();
	}

	return 1;
}


//********************************************
// SetNormalBinding
//********************************************
void CSceneGraph3d::SetNormalBinding(int type)
{
	int size = m_ArrayObject3d.GetSize();
	for (int i = 0; i < size; i++)
	{
		CObject3d* pObject3d = m_ArrayObject3d[i];
		if (pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d* pMesh = (CMesh3d*)m_ArrayObject3d[i];
		pMesh->SetNormalBinding(type);
	}
}

//********************************************
// SetColorBinding
//********************************************
void CSceneGraph3d::SetColorBinding(int type)
{
	int size = m_ArrayObject3d.GetSize();
	for (int i = 0; i < size; i++)
	{
		CObject3d* pObject3d = m_ArrayObject3d[i];
		if (pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d* pMesh = (CMesh3d*)m_ArrayObject3d[i];
		pMesh->SetColorBinding(type);
	}
}


//////////////////////////////////////////////
// TEXTURES
//////////////////////////////////////////////

//********************************************
// HasTexture
//********************************************
int CSceneGraph3d::HasTexture(char* name,
	int* index)
{
	for (int i = 0; i < m_ArrayTexture.GetSize(); i++)
		if (m_ArrayTexture[i]->GetFileName() == name)
		{
			*index = i;
			return 1;
		}
	return 0;
}


//////////////////////////////////////////////
// I/O
//////////////////////////////////////////////

//********************************************
// SaveFile
//********************************************
int CSceneGraph3d::SaveFile(char* name)
{
	// Check
	if (NbObject() == 0)
	{
		AfxMessageBox("This scene does not contain meshes");
		return 0;
	}

	// Check for valid file
	CStdioFile file;
	CFileException ex;

	// Write header
	if (!WriteHeader(file, name))
	{
		AfxMessageBox("Error during writing header");
		return 0;
	}

	// Meshes
	for (int i = 0; i < NbObject(); i++)
	{
		CObject3d* pObject = m_ArrayObject3d[i];
		if (pObject->GetType() == TYPE_MESH3D)
			((CMesh3d*)pObject)->WriteFile(file);
	}

	// Close file
	file.Close();

	return 1;
}

//********************************************
// SaveFileRaw
//********************************************
int CSceneGraph3d::SaveFileRaw(char* name)
{
	// Check
	if (NbObject() == 0)
	{
		AfxMessageBox("This scene does not contain meshes");
		return 0;
	}

	// Check for valid file
	CFile file;
	CFileException ex;

	// Try to open file (text mode)
	if (!file.Open(name, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, &ex))
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << ex.m_cause << "\n";
#endif
		AfxMessageBox("Unable to open file for writing");
		return 0;
	}
	// Meshes
	unsigned int NbMesh = NbObject();
	file.Write(&NbMesh, sizeof(unsigned int));
	for (unsigned int i = 0; i < NbMesh; i++)
	{
		CObject3d* pObject = m_ArrayObject3d[i];
		if (pObject->GetType() == TYPE_MESH3D)
		{
			((CMesh3d*)pObject)->WriteFileRaw(file);

		}
		TRACE("total mesh is %d,this is %d\n", NbMesh, i);
	}

	// Close file
	file.Close();
	AfxMessageBox("finish writing!");
	return 1;
}


//**********************************************
// WriteHeader
// Do not close file
//**********************************************
int CSceneGraph3d::WriteHeader(CStdioFile& file,
	char* name)
{
	CFileException ex;

	// Try to open file (text mode)
	if (!file.Open(name, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &ex))
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << ex.m_cause << "\n";
#endif
		AfxMessageBox("Unable to open file for writing");
		return 0;
	}

	// ** Header *******************************
	TRACE("\nSave VRML 2.0 File...\n");
	TRACE("  name : %s\n", name);
	TRY
	{
		file.WriteString("#VRML V2.0 utf8\n\n");
		file.WriteString("# Produced by 3d Toolbox 1.0 (Pierre Alliez, CNET / DIH / HDM)\n\n");
	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "Error during writing " << e->m_cause << "\n";
#endif
		AfxMessageBox("Error during writing file header");
		file.Close();
		return 0;
	}
	END_CATCH

		// do not close file
		return 1;
}

int CSceneGraph3d::ReadFile(char* name)
{	// Check for valid file
	CFile file;
	CFileException ex;
	TRACE("name is %s\n", name);
	if (!file.Open(name, CFile::modeRead | CFile::typeBinary, &ex))
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << ex.m_cause << "\n";
#endif
		//AfxMessageBox("Unable to open file for reading");
		ex.ReportError();
		//AfxMessageBox("打开文件失败:%d",ex.ReportError());
		return 0;
	}
	unsigned int  rnbmesh;
	file.Read(&rnbmesh, sizeof(unsigned int));
	for (unsigned int i = 0; i < rnbmesh; i++)
	{
		CMesh3d* pObject = new CMesh3d();
		pObject->ReadFile(file);
		m_ArrayObject3d.Add(pObject);
		TRACE("total mesh is %d,this is %d\n", rnbmesh, i);
	}
	// Close file
	file.Close();
	AfxMessageBox("finish reading!");
	return 1;
}


// ** EOF **

