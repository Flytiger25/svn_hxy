// SDIViewSwitchDoc.h : interface of the CSDIViewSwitchDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIVIEWSWITCHDOC_H__CB97F4C7_1752_11D2_B134_00C04FB9CA2B__INCLUDED_)
#define AFX_SDIVIEWSWITCHDOC_H__CB97F4C7_1752_11D2_B134_00C04FB9CA2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "Point3D.h"
#include "Lib3D/SceneGraph3d.h"
#include <map>
typedef struct tagDOC_RECORD
{
	char name[32];
	char address[40];
	char city[24];
	char state[4];
	char zipcode[8];
	char quote[400];
}DOC_RECORD;

enum Mesh3DSelectMode
{
	Mesh3DVertex,
	Mesh3DEdge,
	Mesh3DLandMark
};

///////////////////////////////////////////////////

class CSDIViewSwitchDoc : public CDocument
{

protected: // create from serialization only
	CSDIViewSwitchDoc();
	DECLARE_DYNCREATE(CSDIViewSwitchDoc)

public:
	CFile m_file;   // This file is kept open to do read/writes on a transaction basis.

	DOC_RECORD m_DocRecord;
	static double SWITCHSPLIT_FILE_SIGNATURE;
	UINT m_nHeaderSize;
	UINT m_nNumberOfRecords;
	UINT m_nRecordLength;
	//double m_Arraypointx[100];
	//double m_Arraypointy[100];
	//int    m_Arraynum;
	int    p, n;//阶数
	//double U[100];
	//double N[100];
	int		i;
	int		m_bAddpoint;
	int		m_bSelect, m_SelectItem;
	Point3D point[20], point1, point2;
	COLORREF color;
	double  weight[4][4];
	BOOL    m_Curve1Show, m_Curve2Show;
	int     m_DiscNum;
	int     m_Reparemeterization; //0 null, 1 mobius, 2 bilinear, 3 quadratic
	int		m_Disc1, m_Disc2;
	double  omega;//weight factor
	int		ShowTexture;
	int     ShowCoordinateAxis;
	int     ShowSurface;
	int     ShowExactCurve;
	int     ShowExactCurvePolygon;
	int     ShowOurResult;

	CString Texturename;

	// The scene
	CSceneGraph3d m_SceneGraph;

	//selected object array
	std::vector<int> m_ArraySelectObjects;

	int m_CurrentObjectIndex; //current index

	std::map<HTREEITEM, CObject3d*> m_ArrayTreeItemObjectMap;

	int				  m_BezierNum;
	int	m_Hyperbolanum;


	// Options
	BOOL m_AddWireframe;
	BOOL m_IsoParameterCurve;
	BOOL m_Smooth;
	float m_PolygonOffset;
	BOOL m_Light;
	GLenum m_Mode;

	//Render the surface using opengl or self-triangulation
	BOOL m_UsingOpenglNurbsRender;
	int  m_Tessellation;

	// Operations
public:
	//
	void ZoomAllToFitCurrentMesh();

	//
	void ZoomToFitCurrentMesh();

	//
	void ZoomAll();

	//read nurbs set
	void ReadNurbsSet(LPCTSTR lpszPathName, CSceneGraph3d* pSceneGraph);

	void Read_Mesh_OpenMesh(LPCTSTR lpszPathName, CSceneGraph3d& m_SceneGraph);

	BOOL OnOpenDocument_OpenMesh(LPCTSTR lpszPathName);
	void OnGenerateNewSurface();
	void OnGenerateNewSurface1();
	void OnGenerateBezier21Surface1();

	//Render the scene using CDC
	void RenderScene();
	void SetTransform(float m_xTranslation, float m_yTranslation, float m_zTranslation,
		float m_xRotation, float m_yRotation, float m_zRotation,
		float m_xScaling, float m_yScaling, float m_zScaling);
	void RenderSceneCDC();
	void UpdateReadTree();
	void UpdateShowView();
	void UpdateTreeControl();
	void WriteHeader();
	BOOL ReadHeader();
	void FileSeekRecord(UINT nRecord);
	bool GetRecord(UINT nRecordIndex, void* pRecord);
	void UpdateRecord(UINT nRecordIndex, void* pRecord);

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSDIViewSwitchDoc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
protected:
	virtual BOOL SaveModified();

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSDIViewSwitchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CSDIViewSwitchDoc)
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
	afx_msg void OnGlAddWireframe();
	afx_msg void OnUpdateGlAddWireframe(CCmdUI* pCmdUI);
	afx_msg void OnFileClose();
	afx_msg void OnBnClickedNavigatestartposition();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDIVIEWSWITCHDOC_H__CB97F4C7_1752_11D2_B134_00C04FB9CA2B__INCLUDED_)
