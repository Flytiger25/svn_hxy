// ParseSmf.h: interface for the CParseSmf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSESMF_H__996A829C_B052_48FB_8956_4FBA0FF5CC22__INCLUDED_)
#define AFX_PARSESMF_H__996A829C_B052_48FB_8956_4FBA0FF5CC22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CParseOBJ
{
public:
	void Free(void);
	int Run(char *filename,CSceneGraph3d *pSceneGraph);
	int Run2(char *filename,CSceneGraph3d *pSceneGraph);
	int AlignWithHumanBody(CSceneGraph3d *pSceneGraph);
	int TransformMeshVertices(CMesh3d* pMesh);
	CParseOBJ();
	virtual ~CParseOBJ();
	CString m_FileName;
};

#endif // !defined(AFX_PARSESMF_H__996A829C_B052_48FB_8956_4FBA0FF5CC22__INCLUDED_)
