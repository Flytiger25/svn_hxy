//********************************************
// SceneGraph.h
// class CSceneGraph
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 15/01/98
// Modified : 26/05/98
//********************************************

#ifndef _SCENEGRAPH_3D_
#define _SCENEGRAPH_3D_

#include "Object3d.h"
#include "Texture.h"
#include "Material.h"
#include "Array3d.h"
#include <gl\gl.h>
#include <gl\glu.h>
//#include <GL\glaux.h>
class CSceneGraph3d
{

private :

	// Datas
	CArray3d<CObject3d> m_ArrayObject3d;
	CArray3d<CTexture>  m_ArrayTexture;
	CArray3d<CMaterial> m_ArrayMaterial;
	GLuint *m_pIndexTextureBinding;
	unsigned int m_ListDone;
	unsigned int m_Modified;
	unsigned int m_ListOpenGL;

public :

	// Constructor
	CSceneGraph3d();
	~CSceneGraph3d();
	//
	void ZoomAllToFitCurrentMesh();

	//
	void ZoomToFitCurrentMesh();

	//
	void ZoomAll();

	// Datas
	void SetAt(int index, CObject3d *pObject3d);
	void Add(CObject3d *pObject3d);
	void InsertAt(int index,CObject3d *pObject3d);
	void RemoveAt(int index);
	CObject3d *GetAt(int index);
	CObject3d *operator[](int index);
	int NbObject(void) { return m_ArrayObject3d.GetSize(); }
	void Free(void);

	// I/O
	int ReadFile(char *name);
	int SaveFile(char *name);
	int SaveFileRaw(char *name);
	int WriteHeader(CStdioFile &file,char *name);

	// Misc
	int BuildAdjacency();
	int CalculateNormalPerVertex();
	int CalculateNormalPerFace();

	// Binding
	void SetNormalBinding(int type);
	void SetColorBinding(int type);

	//Draw
	void glDraw();

	//
	void SetTransform(float m_xTranslation, float m_yTranslation, float m_zTranslation,
		float m_xRotation, float m_yRotation, float m_zRotation,
		float m_xScaling, float m_yScaling, float m_zScaling);

	//Draw Accelerate
	void glDraw_Accelerate();

	// Textures
	CArray3d<CTexture> *GetArrayTexture() { return &m_ArrayTexture; }
	int HasTexture(char *name,int *index);
	int NbTexture() { return m_ArrayTexture.GetSize(); }
	int AddTexture(CTexture *pTexture) { return m_ArrayTexture.Add(pTexture); }
	void RemoveAllTexture() { m_ArrayTexture.RemoveAll(); }
	void RemoveTexture(int index) { m_ArrayTexture.RemoveAt(index); }
	CTexture *GetTexture(int index) { return m_ArrayTexture[index]; }

	//Build List
	void glBuildList();
	void SetModifyFlag(int flag){m_Modified = flag;}
};

#endif // _SCENEGRAPH_3D_
