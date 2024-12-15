// MeshDoc.h : interface of the CMeshDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHDOC_H__50ED6A30_C0C1_11D2_B4C7_006067306B48__INCLUDED_)
#define AFX_MESHDOC_H__50ED6A30_C0C1_11D2_B4C7_006067306B48__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "Lib3d/Base3d.h"

class CMeshDoc : public CDocument
{
protected: // create from serialization only
	CMeshDoc();
	DECLARE_DYNCREATE(CMeshDoc)

// Attributes
public:

	// The scene
	CSceneGraph3d m_SceneGraph;

	// Options
	BOOL m_AddWireframe;
	BOOL m_Smooth;
	float m_PolygonOffset;
	BOOL m_Light;
	GLenum m_Mode;

	void RenderScene();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMeshDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMeshDoc)
	afx_msg void OnGlAddWireframe();
	afx_msg void OnUpdateGlAddWireframe(CCmdUI* pCmdUI);
	afx_msg void OnGlSmooth();
	afx_msg void OnUpdateGlSmooth(CCmdUI* pCmdUI);
	afx_msg void OnMeshLoop();
	afx_msg void OnUpdateMeshLoop(CCmdUI* pCmdUI);
	afx_msg void OnGlLight();
	afx_msg void OnGlLine();
	afx_msg void OnUpdateGlLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGlLight(CCmdUI* pCmdUI);
	afx_msg void OnGlVertex();
	afx_msg void OnUpdateGlVertex(CCmdUI* pCmdUI);
	afx_msg void OnGlFace();
	afx_msg void OnUpdateGlFace(CCmdUI* pCmdUI);
	afx_msg void OnMeshColorHeight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHDOC_H__50ED6A30_C0C1_11D2_B4C7_006067306B48__INCLUDED_)
