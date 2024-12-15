#if !defined(AFX_MYTREEVIEW_H__50_68_69_6C_69_70_20_4A_2E_20_53_70_6F_72_79_2E__INCLUDED_)
#define AFX_MYTREEVIEW_H__50_68_69_6C_69_70_20_4A_2E_20_53_70_6F_72_79_2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTreeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
#include <afxcview.h>
#include <vector>
class CMesh3d;
class CVertex3d;
class CFace3d;
class CMyTreeView : public CView
{
protected:
	CMyTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyTreeView)

	// Attributes
public:
	std::vector<Vector3D> m_ArrayCentralpoint;
	std::vector<double>   m_ArrayScale;
	std::vector<Vector3D> m_ArrayEyes;
	std::vector<Vector3D> m_ArrayLookPoints;
	BOOL   m_Initialize;
	BOOL   m_ShowSkeleton;
	BOOL   m_ShowColon;
	BOOL   m_ShowFrame;
	BOOL   m_InNavigationProcess; //whether in process
	BOOL   m_FistTimeRun;
	int	   MarchingSteps;
	double FOVY;
	BOOL m_MouseMove;
	//GPU PHONE SHADING
	//CGcontext cgContextsil1;
	//CGprofile vertProfile1, fragmentProfile1;

	//CGprogram finalvertProgram1, finalfragProgram1;
	//CGparameter modelViewProj1, modelViewIT1,frag_mat_diff1, frag_mat_spec1, frag_mat_shiny1,frag_mat_ambient1;

	//void	initCgFX();

	//void DestroyCgFX();
	//END

	double originalscale;

	//for view point animation
	Vector3D startviewpoint;
	Vector3D endviewpoint;
	int      m_viewpointindex;

	double type;
	CTreeCtrl* m_pTreeCtrl;
	HTREEITEM  m_hRightPaneFormView1;
	HTREEITEM  m_hRightPaneFormView2;
	HTREEITEM  m_hRightPaneEditView;
	HTREEITEM  m_hRightPaneListView;
	HTREEITEM  m_hRightPaneHorizSplitterView;
	HTREEITEM  m_hRightPaneVertSplitterView;
	HTREEITEM  m_hPreviousSelectedItem;

	// Operations
public:

	void ChangeFov();

	// OpenGL specific
	double  g_eye[3];   //eye position
	double  g_look[3];  //target position
	double  g_dir[3];   //eye up direction.
	int     m_eyeindex;

	BOOL m_bDragging;
	GLuint m_nHits;
	BOOL m_bPicking;
	GLdouble m_dDragStart[2];
	GLint m_viewPort[4];
	GLuint m_selectBuf[512];
	double m_dMatrix[16];
	void StartPicking(float xWin, float yWin, int flag);
	void StopPicking();
	void DrawScene(CDC* pdc, GLenum mode);
	void ProcessHits();
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);
	HGLRC m_hGLContext;
	HDC hDC;
	int m_GLPixelIndex;
	void SetClearColor(void) { glClearColor(m_ClearColorRed, m_ClearColorGreen, m_ClearColorBlue, 1.0f); }
	unsigned char* SnapClient(CSize* pSize);

	// Mouse 
	BOOL m_LeftButtonDown;
	BOOL m_RightButtonDown;
	CPoint m_LeftDownPos;
	CPoint m_RightDownPos;
	HCURSOR m_CursorRotation;
	HGLRC m_hRC;
	// Position, rotation ,scaling
	void InitGeometry(void);

	float m_xRotation;
	float m_yRotation;
	float m_zRotation;
	BOOL m_xyRotation;

	float m_xTranslation;
	float m_yTranslation;
	float m_zTranslation;

	float m_xScaling;
	float m_yScaling;
	float m_zScaling;

	float m_SpeedTranslation;
	float m_SpeedRotation;

	// Colors
	float m_ClearColorRed;
	float m_ClearColorGreen;
	float m_ClearColorBlue;

	// Animation
	float m_StepRotationX;
	float m_StepRotationY;
	float m_StepRotationZ;


	// Operations
public:
	//
	void Mesh3dSelectSharpEdge();

	//
	void Mesh3dUnSelectLandmark();

	//
	double JudgeWhetherPointLiesInsideTriangle_double(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3);

	//
	double TriangleArea(double x1, double y1, double x2, double y2, double x3, double y3);

	//
	void MoveSelectedPoint();

	//
	void Mesh3dInputArea();

	BOOL JudgeWhetherPointLiesInsideTriangle(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3, double TOL = 1);
	//
	BOOL JudgeWhetherPointLiesOnLineSegment(double x, double y, double x1, double y1, double x2, double y2, double TOL = 1);

	//
	double GetDistancebetweenPointandLineSegment(double x, double y, double x1, double y1, double x2, double y2);

	//
	BOOL JudgeWhetherPointLiesOnFace(CFace3d* pFace3d);

	//
	BOOL JudgeWhetherPointLiesOnEdge(CVertex3d* pVertex1, CVertex3d* pVertex2);

	//
	void ZoomAll();  //Zoom the view to fit

	//NURBS pick Control Point
	void NURBSPickControlPoints();

	//
	void Mesh3DPick(float xWin, float yWin, int flag);

	//
	void Pick(float xWin, float yWin, int flag);

	//Mesh3d pick point
	void Mesh3dPickPoints();

	//
	void Mesh3dPickObjects(PickStruct& str);

	//Openmesh pick point
	void MeshOpenmeshPickPoints();

	//
	void Mesh3dSelectLandmark();

	//
	void Mesh3dFlipEdge();

	//
	void Mesh3dSelectEdge();

	//
	void Mesh3dSelectFace();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMeshView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTreeView)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTreeView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditCopy();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnZoomall();
	afx_msg void OnZoomToFitCurrentMesh();
	afx_msg void OnZoomAllToFitCurrentMesh();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTREEVIEW_H__50_68_69_6C_69_70_20_4A_2E_20_53_70_6F_72_79_2E__INCLUDED_)
