// MyTreeView.cpp : implementation file

#include "stdafx.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "resource.h"
#include "SDIViewSwitch.h"
#include "Lib3D\texture.h"
#include "lib3d\Mesh3d.h"
#include "MyTreeView.h"
#include "MySplitterWnd.h"
#include "SplitterView.h" 
#include "MainFrm.h"
#include "FormView3.h"
#include <fstream>
#include "math.h"
#include "Nurbs3D.h"
#include "lib3d/Base3d.h"
#include "SDIViewSwitch.h"
#include "lib3d/Mesh3d.h"
#include "lib3d/ColorRamp.h"
#include "FormView3.h"
#include "MyTreeView.h"
#include <fstream>
#include <atlimage.h>
#include "FaceInputArea.h"
#include "CFramebuffer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//#define	FOVY	25.0	
#define ZNEAR	0.1
#define	ZFAR	2000.0
#define EYELEN  0.1
#define MSCALE  200
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMyTreeView, CTreeView)
CMyTreeView* pTreeView;
extern char Post_Message[1000];
CMyTreeView::CMyTreeView()
{
	m_pTreeCtrl = NULL;

	double type = 1;
	// OpenGL
	m_hGLContext = NULL;
	m_GLPixelIndex = 0;

	// Mouse
	m_LeftButtonDown = FALSE;
	m_RightButtonDown = FALSE;

	// Colors
	m_ClearColorRed = 1.0f;
	m_ClearColorGreen = 1.0f;
	m_ClearColorBlue = 1.0f;

	// Animation
	m_StepRotationX = 0.0f;
	m_StepRotationY = 5.0f;
	m_StepRotationZ = 0.0f;

	m_Initialize = FALSE;
	m_ShowSkeleton = FALSE;
	m_ShowColon = TRUE;
	m_ShowFrame = FALSE;
	m_InNavigationProcess = FALSE;
	m_FistTimeRun = TRUE;
	pTreeView = this;

	FOVY = 25.0;
	InitGeometry();
}
/////////////////////////////////////////////////////////////////////////////
void CMyTreeView::InitGeometry(void)
{
	m_xRotation = 0.0f;
	m_yRotation = 0.0f;
	m_zRotation = 0.0f;

	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = -0.0f;

	m_xScaling = 1.0f;
	m_yScaling = 1.0f;
	m_zScaling = 1.0f;

	m_SpeedRotation = 1.0f / 10.0f;
	m_SpeedTranslation = 1.0f / 100.0f;

	m_xyRotation = 1;
}
CMyTreeView::~CMyTreeView()
{
	TRACE("destructing CMyTreeView\n");
	m_ArrayCentralpoint.clear();
	m_ArrayScale.clear();
	m_ArrayEyes.clear();
	m_ArrayLookPoints.clear();
}
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMyTreeView, CView)
	//{{AFX_MSG_MAP(CMyTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_TIMER()
	ON_COMMAND(ID_ZOOM_ZOOMALL, &CMyTreeView::OnZoomall)
	ON_BN_CLICKED(ID_SURFACE_SHOWSURFACEINFORMATION, &CFormView3::OnSurfaceShowsurfaceinformation)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWSURFACEINFORMATION, &CFormView3::OnUpdateSurfaceShowsurfaceinformation)
	ON_BN_CLICKED(ID_SURFACE_SHOWISOPARAMETERCURVES, &CFormView3::OnSurfaceShowIsoParameterCurves)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWISOPARAMETERCURVES, &CFormView3::OnUpdateSurfaceShowisoparametercurves)
	ON_BN_CLICKED(ID_SURFACE_SHOWSURFACE, &CFormView3::OnSurfaceShowSurface)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWSURFACE, &CFormView3::OnUpdateSurfaceShowSurface)
	ON_BN_CLICKED(ID_SURFACE_SHOWCONTROLPOINTNETWORK, &CFormView3::OnSurfaceShowcontrolpointnetwork)
	ON_UPDATE_COMMAND_UI(ID_SURFACE_SHOWCONTROLPOINTNETWORK, &CFormView3::OnUpdateSurfaceShowcontrolpointnetwork)

	ON_UPDATE_COMMAND_UI(ID_SURFACE_DELETETHISFACE, &CFormView3::OnUpdateSurfaceDeletethisface)
	ON_COMMAND(ID_SURFACE_DELETETHISFACE, &CFormView3::OnSurfaceDeletethisface)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
//********************************************
// CreateViewGLContext
// Create an OpenGL rendering context
//********************************************
BOOL CMyTreeView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}
void CMyTreeView::OnEditCopy()
{
	return;
	// Clean clipboard of contents, and copy the DIB.
	if (OpenClipboard())
	{

		BeginWaitCursor();

		// Snap
		CSize size;
		unsigned char* pixel = SnapClient(&size);

		// Image
		CTexture image;

		// Link image - buffer
		int success = 0;
		VERIFY(image.ReadBuffer(pixel, size.cx, size.cy, 24));

		// Cleanup memory
		delete[] pixel;

		EmptyClipboard();

		SetClipboardData(CF_DIB, image.ExportHandle());
		CloseClipboard();
		EndWaitCursor();
	}
}
BOOL CMyTreeView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);

	if (m_hGLContext == NULL)
		return FALSE;

	if (wglMakeCurrent(hDC, m_hGLContext) == FALSE)
		return FALSE;

	return TRUE;
}
void CMyTreeView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here



}
/////////////////////////////////////////////////////////////////////////////
// CMyTreeView diagnostics
#ifdef _DEBUG
void CMyTreeView::AssertValid() const
{
	CView::AssertValid();
}

void CMyTreeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG
BOOL CMyTreeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyTreeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CMyTreeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
BOOL CMyTreeView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CView::PreCreateWindow(cs);
}
void CMyTreeView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC, m_hGLContext);
	::ReleaseDC(hWnd, hDC);
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
////////////////////////////////////////////////////////////////////////////////////
void CMyTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hSelectedItem = m_pTreeCtrl->GetSelectedItem();
	if (hSelectedItem == m_hPreviousSelectedItem) //if same as last view, then don't need to ReplaceView
		return;

	m_hPreviousSelectedItem = hSelectedItem;

	CMySplitterWnd* pParentSplitter = (CMySplitterWnd*)GetParent();
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////
extern void drawString(const char* str);
int CMyTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if (SetWindowPixelFormat(hDC) == FALSE)
		return 0;

	if (CreateViewGLContext(hDC) == FALSE)
		return 0;

	//::ReleaseDC(hWnd,hDC);

	// Default mode
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glEnable(GL_BLEND);
	//glEnable(GL_POINT_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	// Lights properties
	//float	ambientProperties[]  = {0.9f, 0.9f, 0.9f, 1.0f};
	//float	diffuseProperties[]  = {0.9f, 0.9f, 0.9f, 1.0f};
	//float	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};

	// 0.91f, 0.42f,0.34f,1.0f
	//
	float	ambientProperties[] = { 0.81f, 0.81f, 0.81f, 1.0f };
	float	diffuseProperties[] = { 0.81f, 0.81f, 0.81f, 1.0f };
	float	specularProperties[] = { 0.81f, 0.81f, 0.81f, 1.0f };
	float	Position[] = { 0.0f, 0.0f, -150.0f, 1.0f };
	float	Position1[] = { -541.599f, 298.343f, -244.357f };
	float	Position2[] = { 0.0f, 0.0f, 15.0f, 1.0f };
	float	Position3[] = { 0.0f, 15.0f, 0.0f, 1.0f };
	float	Position4[] = { 15.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightfv(GL_LIGHT0, GL_POSITION, Position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularProperties);
	glLightfv(GL_LIGHT1, GL_POSITION, Position1);

	/*glLightfv(GL_LIGHT2, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularProperties);
	glLightfv(GL_LIGHT2, GL_POSITION, Position2);

	glLightfv(GL_LIGHT3, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specularProperties);
	glLightfv(GL_LIGHT3, GL_POSITION, Position3);

	glLightfv(GL_LIGHT4, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specularProperties);
	glLightfv(GL_LIGHT4, GL_POSITION, Position4);*/

	glEnable(GL_COLOR_MATERIAL);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_AMBIENT_AND_DIFFUSE);
	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

	glClearColor(m_ClearColorRed, m_ClearColorGreen, m_ClearColorBlue, 1.0f);
	glClearDepth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	CRect rect;
	GetClientRect(&rect);
	double aspect = (rect.Height() == 0) ? rect.Width() : (double)rect.Width() / (double)rect.Height();
	gluPerspective(FOVY, aspect, ZNEAR, ZFAR);

	glMatrixMode(GL_MODELVIEW);				// 设置当前矩阵为模型视图矩阵
	glLoadIdentity();						// 重置当前指定的矩阵为单位矩阵
	//eye & target position.
	g_eye[0] = 0;
	g_eye[1] = 0;
	g_eye[2] = 5;

	g_look[0] = 0;
	g_look[1] = 0;
	g_look[2] = 0;

	g_dir[0] = 0;
	g_dir[1] = 1;
	g_dir[2] = 0;

	// Default : lighting
	glEnable(GL_LIGHT0);
	//glDisable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);
	glEnable(GL_AUTO_NORMAL);

	// Default : blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(Gl_LESS);

	// Modulate : texture lighting
	glEnable(GL_TEXTURE_2D);

	//#if defined(GPU_ACCELERATE)
	//initCgFX();
	//#endif
	return 1;
}
/////////////////////////////////////////////////////////////////////////////

void CMyTreeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_RightButtonDown = FALSE;
	m_LeftButtonDown = FALSE;
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}
void CMyTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;
	if (nFlags & MK_CONTROL)
	{
		m_bPicking = TRUE;

		HWND hWnd = GetSafeHwnd();
		HDC hDC = ::GetDC(hWnd);

		wglMakeCurrent(hDC, m_hRC);

		StartPicking((float)point.x, (float)point.y, 1);
		//StopPicking();

		wglMakeCurrent(NULL, NULL);
	}
	else if (nFlags & MK_LBUTTON)
	{
		m_bDragging = TRUE;
		//	m_dDragStart[0] = +0.5*3.14*(2*point.x-m_iWidth)/(double)m_iWidth;
		//	m_dDragStart[1] = -0.5*3.14*(2*point.y-m_iHeight)/(double)m_iHeight;
		//	SetCapture();
	}
	SetCapture();

	CView::OnLButtonDown(nFlags, point);
}
//********************************************
// SetWindowPixelFormat
//********************************************
BOOL CMyTreeView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;

	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);
	if (m_GLPixelIndex == 0) // Choose default
	{
		m_GLPixelIndex = 1;
		if (DescribePixelFormat(hDC, m_GLPixelIndex,
			sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
			return FALSE;
	}

	if (!SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc))
		return FALSE;


	// CListBox

	return TRUE;
}
void CMyTreeView::OnMouseMove(UINT nFlags, CPoint point)
{
	// Both : rotation
	//if(m_LeftButtonDown && m_RightButtonDown)
	if (m_LeftButtonDown && !m_RightButtonDown)
	{
		if (m_xyRotation)
		{
			m_yRotation -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
			m_xRotation -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
		}
		else
		{
			m_zRotation -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
			m_xRotation -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
		}
		m_LeftDownPos = point;
		m_RightDownPos = point;
		InvalidateRect(NULL, FALSE);
	}

	else

		// Left : x / y translation
		//if(m_LeftButtonDown)
		if (m_LeftButtonDown && m_RightButtonDown)
		{
			m_xTranslation -= (float)(m_LeftDownPos.x - point.x) * m_SpeedTranslation;
			m_yTranslation += (float)(m_LeftDownPos.y - point.y) * m_SpeedTranslation;
			m_LeftDownPos = point;
			InvalidateRect(NULL, FALSE);
		}

		else

			// Right : z translation
			if (m_RightButtonDown)
			{
				m_zTranslation += (float)(m_RightDownPos.y - point.y) * m_SpeedTranslation;
				m_RightDownPos = point;
				InvalidateRect(NULL, FALSE);
			}

	/*
	TRACE("\nPosition\n");
	TRACE("Translation : %g %g %g\n",m_xTranslation,m_yTranslation,m_zTranslation);
	TRACE("Rotation    : %g %g %g\n",m_xRotation,m_yRotation,m_zRotation);
	*/
	CView::OnMouseMove(nFlags, point);
}
void CMyTreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_RightButtonDown = TRUE;
	m_RightDownPos = point;
	SetCapture();
	CView::OnRButtonDown(nFlags, point);
}

void CMyTreeView::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_RightButtonDown = FALSE;
	m_LeftButtonDown = FALSE;
	//m_MouseMove==FALSE;
	//opengl画图区域鼠标右键菜单
	//if (m_MouseMove==FALSE)
	//{
	/*if (!m_MouseMove)
	{

		::SetForegroundWindow(m_hWnd);
		CPoint screenPoint, clientpoint;
		UINT uFlag;
		GetCursorPos(&screenPoint);
		clientpoint = screenPoint;
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU2));
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
		popup->Detach();
	}
	//}*/
	if (nFlags & MK_CONTROL)
		StartPicking((float)point.x, (float)point.y, 2);
	m_MouseMove = FALSE;
	ReleaseCapture();
	CView::OnRButtonUp(nFlags, point);
}
void CMyTreeView::ChangeFov()
{
	CRect rect;
	GetClientRect(&rect);
	int cx = rect.Width();
	int cy = rect.Height();
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	//TRACE("Activate view, set active OpenGL rendering context...\n");
	wglMakeCurrent(hDC, m_hGLContext);

	// Set OpenGL perspective, viewport and mode
	double aspect = (cy == 0) ? cx : (double)cx / (double)cy;

	glViewport(0, 0, cx, cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(FOVY, aspect, ZNEAR, ZFAR);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glDrawBuffer(GL_BACK);

	// Release
	::ReleaseDC(hWnd, hDC);
}
void CMyTreeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	//TRACE("Activate view, set active OpenGL rendering context...\n");
	wglMakeCurrent(hDC, m_hGLContext);

	// Set OpenGL perspective, viewport and mode
	double aspect = (cy == 0) ? cx : (double)cx / (double)cy;

	glViewport(0, 0, cx, cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(FOVY, aspect, ZNEAR, ZFAR);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glDrawBuffer(GL_BACK);

	// Release
	::ReleaseDC(hWnd, hDC);

}
void CMyTreeView::OnDestroy()
{

	if (wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL, NULL);

	if (m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}

#if defined(GPU_ACCELERATE)
	DestroyCgFX();
#endif

	CView::OnDestroy();

}

void CMyTreeView::ZoomAll()
{
	g_eye[0] = 0;
	g_eye[1] = 0;
	g_eye[2] = 5;

	g_look[0] = 0;
	g_look[1] = 0;
	g_look[2] = 0;

	g_dir[0] = 0;
	g_dir[1] = 1;
	g_dir[2] = 0;

	m_xScaling = m_yScaling = m_zScaling = 1;
	m_xTranslation = m_yTranslation = m_zTranslation = 0;
	m_xRotation = m_yRotation = m_zRotation = 0;

	Invalidate(TRUE);
}

void CMyTreeView::OnPaint()
{
	// Device context for painting
	CPaintDC dc(this);

	// Model is stored in Document
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();

	pDoc->SetTransform(m_xTranslation, m_yTranslation, m_zTranslation,
		m_xRotation, m_yRotation, m_zRotation,
		m_xScaling, m_yScaling, m_zScaling);
	//if(pDoc->m_SceneGraph.NbObject() == 0)
	//	return;

	//pDoc->m_Mode = GL_LINE;
	//ASSERT_VALID(pDoc);

	// Useful in multidoc templates
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC, m_hGLContext);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//gluLookAt(g_eye[0],g_eye[1],g_eye[2],g_look[0],g_look[1],g_look[2],g_dir[0],g_dir[1],g_dir[2]);
	gluLookAt(g_eye[0], g_eye[1], g_eye[2], g_look[0], g_look[1], g_look[2], g_dir[0], g_dir[1], g_dir[2]);

	//glLightfv( GL_LIGHT0, GL_POSITION, Position );
	/*if(!m_SetCameraMode)
	{
		glTranslated(m_xTranslation,m_yTranslation,m_zTranslation);
		glRotatef(m_xRotation, 1.0, 0.0, 0.0);
		glRotatef(m_yRotation, 0.0, 1.0, 0.0);
		glRotatef(m_zRotation, 0.0, 0.0, 1.0);
		glScalef(m_xScaling,m_yScaling,m_zScaling);
	}*/

	//DrawCoordinateAxis();
	//test
	//glClearColor(1,0,0,1);
	pDoc->RenderScene();
	/*CFramebuffer* buffer = new CFramebuffer();
	buffer->begin();
	pDoc->RenderScene();
	buffer->saveFrameBuff("D:\\1.jpg");
	buffer->end();
	delete buffer;*/


	glPopMatrix();
	// Double buffer
	SwapBuffers(hDC);

	//SwapBuffers();
	glFlush();

	//Render using CDC
	pDoc->RenderSceneCDC();
	// Release
	//::ReleaseDC(hWnd,hDC);
}
// Hand-made client snapping
unsigned char* CMyTreeView::SnapClient(CSize* pSize)
{
	BeginWaitCursor();

	// Client zone
	CRect rect;
	GetClientRect(&rect);
	//CSize size(CTexture::LowerPowerOfTwo(rect.Width()),rect.Height());
	CSize size(rect.Width(), rect.Height());
	*pSize = size;

	ASSERT(size.cx > 0);
	ASSERT(size.cy > 0);

	// Alloc
	unsigned char* pixel = new unsigned char[3 * size.cx * size.cy];
	ASSERT(pixel != NULL);

	// Capture frame buffer
	TRACE("Start reading client...\n");
	TRACE("Client : (%d,%d)\n", size.cx, size.cy);

	CRect ClientRect, MainRect;
	this->GetWindowRect(&ClientRect);
	CWnd* pMain = AfxGetApp()->m_pMainWnd;
	CWindowDC dc(pMain);
	pMain->GetWindowRect(&MainRect);
	int xOffset = ClientRect.left - MainRect.left;
	int yOffset = ClientRect.top - MainRect.top;
	for (int j = 0; j < size.cy; j++)
		for (int i = 0; i < size.cx; i++)
		{
			COLORREF color = dc.GetPixel(i + xOffset, j + yOffset);
			pixel[3 * (size.cx * (size.cy - 1 - j) + i)] = (BYTE)GetBValue(color);
			pixel[3 * (size.cx * (size.cy - 1 - j) + i) + 1] = (BYTE)GetGValue(color);
			pixel[3 * (size.cx * (size.cy - 1 - j) + i) + 2] = (BYTE)GetRValue(color);
		}
	EndWaitCursor();
	return pixel;
}
void CMyTreeView::Mesh3dPickPoints()
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);
	double xWin = pt1.x;
	double yWin = pt1.y;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);
	GLdouble out[3], in[3];
	int selectindex = -1;
	double mindis = 1e10;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		Point3D proPoint;
		in[0] = pVertex->x();
		in[1] = pVertex->y();
		in[2] = pVertex->z();
		gluProject(in[0], in[1], in[2], pMesh->modelview, pMesh->projection, pMesh->m_viewPort, &(out[0]), &(out[1]), &(out[2]));

		proPoint.x = out[0];
		proPoint.y = pMesh->m_viewPort[3] - out[1];
		proPoint.z = out[2];

		if (SQR(xWin - proPoint[0]) + SQR(yWin - proPoint[1]) < mindis)
		{
			mindis = SQR(xWin - proPoint[0]) + SQR(yWin - proPoint[1]);
			selectindex = i;
		}
	}
	if (selectindex >= 0 && selectindex < pMesh->NbVertex())
	{
		CVertex3d* pVertex = pMesh->GetVertex(selectindex);
		pMesh->m_ArraySharpVertexIndex.push_back(selectindex);
		sprintf(Post_Message, "(%d,%d): %f %f %f uv: (%f %f)total %d points", selectindex, pVertex->m_OriginalIndex, pVertex->x(), pVertex->y(), pVertex->z(), pVertex->u, pVertex->v, pMesh->m_ArraySharpVertexIndex.size());
		//CClientDC dc(this);
		//TRACE("%d %d %s"xWin,yWin,message);
		//AfxGetMainWnd()->SetWindowText(message);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		//dc.TextOut(xWin,yWin,message);
	}
	pMesh->SetModified(TRUE);
}
/*void CMyTreeView::ACISNURBSSelect()
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);
	double xWin = pt1.x;
	double yWin = pt1.y;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex);
	GLdouble out[3], in[3];
	int selectindex = -1;
	double mindis = 1e10;
	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		Point3D proPoint;
		in[0] = pVertex->x();
		in[1] = pVertex->y();
		in[2] = pVertex->z();
		gluProject(in[0], in[1], in[2], pMesh->modelview, pMesh->projection, pMesh->m_viewPort, &(out[0]), &(out[1]), &(out[2]));

		proPoint.x = out[0];
		proPoint.y = pMesh->m_viewPort[3] - out[1];
		proPoint.z = out[2];

		if (SQR(xWin - proPoint[0]) + SQR(yWin - proPoint[1]) < mindis)
		{
			mindis = SQR(xWin - proPoint[0]) + SQR(yWin - proPoint[1]);
			selectindex = i;
		}
	}
	if (selectindex >= 0 && selectindex < pMesh->NbVertex())
	{
		CVertex3d* pVertex = pMesh->GetVertex(selectindex);
		pMesh->m_ArraySharpVertexIndex.push_back(selectindex);
		sprintf(Post_Message, "(%d,%d): %f %f %f uv: (%f %f)total %d points", selectindex, pVertex->m_OriginalIndex, pVertex->x(), pVertex->y(), pVertex->z(), pVertex->u, pVertex->v, pMesh->m_ArraySharpVertexIndex.size());
		//CClientDC dc(this);
		//TRACE("%d %d %s"xWin,yWin,message);
		//AfxGetMainWnd()->SetWindowText(message);
		::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
		//dc.TextOut(xWin,yWin,message);
	}
	pMesh->SetModified(TRUE);
}*/
void CMyTreeView::NURBSPickControlPoints()
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);
	double xWin = pt1.x;
	double yWin = pt1.y;
	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);
	GLdouble out[3], in[3];
	int selectiindex = -1;
	int selectjindex = -1;
	double mindis = 1e10;
	for (int i = 0; i < pSurface->GetCtrlNumberU(); i++)
		for (int j = 0; j < pSurface->GetCtrlNumberV(); j++)
		{
			Point3D point = pSurface->GetControlPoint(i, j);
			Point3D proPoint;
			in[0] = point.x;
			in[1] = point.y;
			in[2] = point.z;
			gluProject(in[0], in[1], in[2], pSurface->modelview, pSurface->projection, pSurface->m_viewPort, &(out[0]), &(out[1]), &(out[2]));

			proPoint.x = out[0];
			proPoint.y = pSurface->m_viewPort[3] - out[1];
			proPoint.z = out[2];

			if (SQR(xWin - proPoint[0]) + SQR(yWin - proPoint[1]) < mindis)
			{
				mindis = SQR(xWin - proPoint[0]) + SQR(yWin - proPoint[1]);
				selectiindex = i;
				selectjindex = j;
			}
		}
	if (selectiindex >= 0 && selectiindex < pSurface->GetCtrlNumberU()
		&& selectjindex >= 0 && selectjindex < pSurface->GetCtrlNumberV())
	{
		Point3D point = pSurface->GetControlPoint(selectiindex, selectjindex);
		char message[80];
		sprintf(message, "(%d, %d): %f %f %f", selectiindex, selectjindex, point.x, point.y, point.z);
		//CClientDC dc(this);
		//TRACE("%d %d %s"xWin,yWin,message);
		AfxGetMainWnd()->SetWindowText(message);
		//dc.TextOut(xWin,yWin,message);
	}
}
void CMyTreeView::Pick(float xWin, float yWin, int flag)
{
	POINT pt1;
	GetCursorPos(&pt1);
	//pt1.x = xWin;
	//pt1.y = yWin;
	RECT Rect;
	//ScreenToClient(&pt1);
	GetClientRect(&Rect);
	yWin = Rect.bottom - yWin;
	//xWin = pt1.x;
	//yWin = pt1.y;
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	CObject3d* Object = (CObject3d*)pDoc->m_SceneGraph.GetAt(0);
	GLdouble out1[3], out2[3];
	gluUnProject(xWin, yWin, 0, Object->modelview, Object->projection, Object->m_viewPort, &(out1[0]), &(out1[1]), &(out1[2]));
	gluUnProject(xWin, yWin, 1, Object->modelview, Object->projection, Object->m_viewPort, &(out2[0]), &(out2[1]), &(out2[2]));

	PickStruct str;

	str.point.setValue(out1[0], out1[1], out1[2]);
	str.dir.SetValue(out2[0], out2[1], out2[2]);

	for (int i = 0; i < pDoc->m_SceneGraph.NbObject(); i++)
	{
		pDoc->m_SceneGraph.GetAt(i)->PickTest(str);
	}

	if (str.m_SelectIndex == -1)
		return;
	//Ctrl+ Lbuttondown
	//add to the selection
	if (flag == 1)
	{
		if (str.m_SelectIndex > -1)
		{
			pDoc->m_ArraySelectObjects.push_back(str.m_SelectIndex);
			pDoc->m_SceneGraph.GetAt(str.m_SelectIndex)->m_IsSelected = TRUE;
			pDoc->m_CurrentObjectIndex = str.m_SelectIndex;
		}
	}
	else	if (flag == 2)
	{
		//rbutton down, pop up menu
		pDoc->m_CurrentObjectIndex = str.m_SelectIndex;
		if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_NURBSSURFACE)
		{
			CMenu menu;
			VERIFY(menu.LoadMenu(IDR_MENU1));
			CMenu* popup = menu.GetSubMenu(0);
			ASSERT(popup != NULL);
			popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt1.x, pt1.y, AfxGetMainWnd());
			popup->Detach();
		}
		else
			if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_MESH3D)
			{
				CMenu menu;
				VERIFY(menu.LoadMenu(IDR_MENU3));
				CMenu* popup = menu.GetSubMenu(0);
				ASSERT(popup != NULL);
				popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt1.x, pt1.y, AfxGetMainWnd());
				popup->Detach();
			}
			else
				if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_NURBSCURVE3D) {
					CMenu menu;
					VERIFY(menu.LoadMenu(IDR_MENU_NURBSCURVE));
					CMenu* popup = menu.GetSubMenu(0);
					ASSERT(popup != NULL);
					popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt1.x, pt1.y, AfxGetMainWnd());
					popup->Detach();
				}
				else
					if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_POLYGON)
					{
						CMenu menu;
						VERIFY(menu.LoadMenu(IDR_MENU6));
						CMenu* popup = menu.GetSubMenu(0);
						ASSERT(popup != NULL);
						popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt1.x, pt1.y, AfxGetMainWnd());
						popup->Detach();
					}
					else
						if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_ACIS_FACE)
						{
							CMenu menu;
							VERIFY(menu.LoadMenu(IDR_MENU7));
							CMenu* popup = menu.GetSubMenu(0);
							ASSERT(popup != NULL);
							popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt1.x, pt1.y, AfxGetMainWnd());
							popup->Detach();
						}
						else
							if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_OCC)
							{
								CMenu menu;
								VERIFY(menu.LoadMenu(IDR_MENU7));
								CMenu* popup = menu.GetSubMenu(0);
								ASSERT(popup != NULL);
								popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt1.x, pt1.y, AfxGetMainWnd());
								popup->Detach();
							}
							else
								if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_OCC_FACE)
								{
									CMenu menu;
									VERIFY(menu.LoadMenu(IDR_MENU7));
									CMenu* popup = menu.GetSubMenu(0);
									ASSERT(popup != NULL);
									popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt1.x, pt1.y, AfxGetMainWnd());
									popup->Detach();
								}
								else
									if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_OCC_EDGE)
									{
										CMenu menu;
										VERIFY(menu.LoadMenu(IDR_MENU7));
										CMenu* popup = menu.GetSubMenu(0);
										ASSERT(popup != NULL);
										popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt1.x, pt1.y, AfxGetMainWnd());
										popup->Detach();
									}
	}
}

void CMyTreeView::Mesh3DPick(float xWin, float yWin, int flag)
{
	//POINT pt1;
	//GetCursorPos(&pt1);
	//pt1.x = xWin;
	//pt1.y = yWin;
	RECT Rect;
	//ScreenToClient(&pt1);
	GetClientRect(&Rect);
	yWin = Rect.bottom - yWin;
	//xWin = pt1.x;
	//yWin = pt1.y;
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	CObject3d* Object = (CObject3d*)pDoc->m_SceneGraph.GetAt(0);
	GLdouble out1[3], out2[3];
	gluUnProject(xWin, yWin, 0, Object->modelview, Object->projection, Object->m_viewPort, &(out1[0]), &(out1[1]), &(out1[2]));
	gluUnProject(xWin, yWin, 1, Object->modelview, Object->projection, Object->m_viewPort, &(out2[0]), &(out2[1]), &(out2[2]));

	PickStruct str;

	str.point.setValue(out1[0], out1[1], out1[2]);
	str.dir.SetValue(out2[0], out2[1], out2[2]);

	Object->eyepoint.setValue(out1[0], out1[1], out1[2]);
	Object->seepointwithdepth1.setValue(out2[0], out2[1], out2[2]);
	//Mesh3dPickPoints();
	Mesh3dPickObjects(str);
}
void CMyTreeView::StartPicking(float xWin, float yWin, int flag)
{
	CClientDC dc(this);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	/*if (pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_NURBSSURFACE && flag == 1)
	{
		NURBSPickControlPoints();
	}
	else if (pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_MESH3D && flag == 1)
	{
		Mesh3DPick(xWin, yWin, flag);
	}
	else if (pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_BREP_ACIS)
	{

	}*/
	Pick(xWin, yWin, flag);

	Invalidate(TRUE);
}

void CMyTreeView::Mesh3dPickObjects(PickStruct& str)
{

	sprintf(Post_Message, "");
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);
	int selectindex = -1;
	double mindis = 1e10;
	double toLINEDIS = 0.1;
	double currentt, mint = MAX_DOUBLE;

	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh->GetVertex(i);
		pVertex->m_OriginalIndex = i;
		Point3D proPoint;
		proPoint[0] = pVertex->x();
		proPoint[1] = pVertex->y();
		proPoint[2] = pVertex->z();

		Vector3D unitdir = str.dir;
		unitdir.SetUnit();

		double prolen = (proPoint - str.point) * unitdir;
		double len = Length(proPoint - str.point);

		double lengtoline = sqrt(len * len - prolen * prolen);


		if (lengtoline < toLINEDIS)
		{
			//计算t
			currentt = (proPoint - str.point) * str.dir / (str.dir * str.dir);
			if (currentt < mint)
			{
				mint = currentt;
				selectindex = i;
			}
		}
	}


	if (selectindex >= 0 && selectindex < pMesh->NbVertex())
	{
		CVertex3d* pVertex = pMesh->GetVertex(selectindex);
		pMesh->m_ArraySharpVertexIndex.push_back(selectindex);
		sprintf(Post_Message, "(%d,%d): %f %f %f total %d points", selectindex, pVertex->m_OriginalIndex, pVertex->x(), pVertex->y(), pVertex->z(), pMesh->m_ArraySharpVertexIndex.size());
		//CClientDC dc(this);
		//TRACE("%d %d %s"xWin,yWin,message);
		//AfxGetMainWnd()->SetWindowText(message);

		//dc.TextOut(xWin,yWin,message);
	}
	//添加选择三角形代码

	mint = MAX_DOUBLE;
	CFace3d* pointToFace = NULL;
	int m_selectTriangleIndex;
	for (int i = 0; i < pMesh->NbFace(); i++)
	{
		CFace3d* currentFace = pMesh->GetFace(i);
		currentFace->m_originalIndex = i;

		if (currentFace->NbVertex() != 3)
			assert(false);

		// 取三角平面的法相以及中心点
		CVertex3d* faceCenter = currentFace->GetCenter();
		CVector3d* faceNormal = currentFace->GetNormal();
		faceNormal->NormalizeL2();
		Point3D p1;
		/*p1.x = faceCenter->m_Coord[0];
		p1.y = faceCenter->m_Coord[1];
		p1.z = faceCenter->m_Coord[2];*/
		p1.x = currentFace->v1()->x();
		p1.y = currentFace->v1()->y();
		p1.z = currentFace->v1()->z();

		Vector3D n1;
		n1.x = faceNormal->m_x;
		n1.y = faceNormal->m_y;
		n1.z = faceNormal->m_z;

		// 获取射线的方向以及起点
		double t = (p1 - str.point) * n1 / (str.dir * n1);

		// 判断点是否在三角形内
		Point3D testPoint = str.point + str.dir * t;

		int testPOnFace = (testPoint - p1) * n1;
		if (fabs(testPOnFace) > TOLER)
		{
			continue;
		}

		int res = pMesh->CheckPointInTriangle3D(testPoint, currentFace);


		if (res && fabs(t) < fabs(mint))
		{
			mint = t;
			pointToFace = currentFace;
			m_selectTriangleIndex = i;
		}

	}
	//CVertex3d** p = pointToFace->m_pVertex;

	//for (int i = 0; i < 6; i++)
	//{
	//	double x = p[i]->m_Coord[0];
	//	double y = p[i]->m_Coord[1];
	//	double z = p[i]->m_Coord[2];
	//}


	if (pointToFace == NULL)
		assert(false);
	else
	{
		pointToFace->SetColor(CColor(255, 0, 0));
		pMesh->m_ArraySharpVertexIndex.push_back(pointToFace->m_pVertex[0]->m_OriginalIndex);
		pMesh->m_ArraySharpVertexIndex.push_back(pointToFace->m_pVertex[1]->m_OriginalIndex);
		pMesh->m_ArraySharpVertexIndex.push_back(pointToFace->m_pVertex[2]->m_OriginalIndex);
		char message[1000];
		sprintf(message, "nearest face ID %d, face vertex (%d,%d,%d):(%.2f %.2f %f) (%.2f %.2f %f) (%.2f %.2f %.2f)", pointToFace->m_originalIndex, pointToFace->m_pVertex[0]->m_OriginalIndex, pointToFace->m_pVertex[1]->m_OriginalIndex, pointToFace->m_pVertex[2]->m_OriginalIndex,
			pointToFace->m_pVertex[0]->m_Coord[0], pointToFace->m_pVertex[0]->m_Coord[1], pointToFace->m_pVertex[0]->m_Coord[2], pointToFace->m_pVertex[1]->m_Coord[0], pointToFace->m_pVertex[1]->m_Coord[1], pointToFace->m_pVertex[1]->m_Coord[2], pointToFace->m_pVertex[2]->m_Coord[0], pointToFace->m_pVertex[2]->m_Coord[1], pointToFace->m_pVertex[2]->m_Coord[2]);
		strcat(Post_Message, message);
	}

	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	pMesh->SetModified(TRUE);
}
void CMyTreeView::StopPicking()
{

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	m_nHits = glRenderMode(GL_RENDER);
	if (m_nHits != 0) {
		ProcessHits();
		Invalidate(FALSE);
	}

}
void CMyTreeView::DrawScene(CDC* pdc, GLenum mode)
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	if (num != 1)
		return;
	CObject3d* pSurface = pDoc->m_SceneGraph.GetAt(0);
	((CNurbsSuface*)pSurface)->glDraw();
}
void CMyTreeView::ProcessHits()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CNurbsSuface* pSurface = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(0);
	GLint i, j, numberOfNames;
	GLuint names, * ptr, minZ, * ptrNames;

	ptr = (GLuint*)m_selectBuf;
	minZ = 0xffffffff;
	for (i = 0; i < m_nHits; i++) {
		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr + 2;
		}

		ptr += names + 2;
	}
	if (numberOfNames > 0) {
		ptr = ptrNames;
		for (j = 0; j < numberOfNames; j++, ptr++)
		{
			int k, tetIndex;
			k = *ptr;
			//pSurface->m_ArrayFace[*ptr]->SetFlag(1);

		}
	}

}

//视图回到初始位置
void CMyTreeView::OnZoomall()
{

	m_xRotation = 0.0f;
	m_yRotation = 0.0f;
	m_zRotation = 0.0f;

	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = -0.0f;

	m_xScaling = 1.0f;
	m_yScaling = 1.0f;
	m_zScaling = 1.0f;

	m_SpeedRotation = 1.0f / 100.0f;
	m_SpeedTranslation = 1.0f / 100.0f;

	m_xyRotation = 1;

	//Zoom all.
	pDoc->ZoomAll();

	InvalidateRect(NULL, FALSE);
}

void CMyTreeView::OnZoomToFitCurrentMesh()
{

	m_xRotation = 0.0f;
	m_yRotation = 0.0f;
	m_zRotation = 0.0f;

	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = -0.0f;

	m_xScaling = 1.0f;
	m_yScaling = 1.0f;
	m_zScaling = 1.0f;

	m_SpeedRotation = 1.0f / 100.0f;
	m_SpeedTranslation = 1.0f / 100.0f;

	m_xyRotation = 1;

	//Zoom all.
	pDoc->ZoomToFitCurrentMesh();

	InvalidateRect(NULL, FALSE);
}

void CMyTreeView::OnZoomAllToFitCurrentMesh()
{

	m_xRotation = 0.0f;
	m_yRotation = 0.0f;
	m_zRotation = 0.0f;

	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = -0.0f;

	m_xScaling = 1.0f;
	m_yScaling = 1.0f;
	m_zScaling = 1.0f;

	m_SpeedRotation = 1.0f / 100.0f;
	m_SpeedTranslation = 1.0f / 100.0f;

	m_xyRotation = 1;

	//Zoom all.
	pDoc->ZoomAllToFitCurrentMesh();

	InvalidateRect(NULL, FALSE);
}
#define VK_D 68 
#define VK_d 100
#define VK_L 76 
#define VK_l 108
#define VK_F 70
#define VK_f 102
#define VK_W 87
#define VK_w 119 
#define VK_T 84
#define VK_t 116
#define VK_A 65
#define VK_a 97
#define VK_I 49
#define VK_i 105
#define VK_S 83
#define VK_s 115
BOOL CMyTreeView::PreTranslateMessage(MSG* pMsg)
{
	CClientDC dc(this);
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetDocument();
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return TRUE;

	double xWin, yWin;
	//whether the keyboard message
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		/*if(pDoc->m_SceneGraph.GetAt(p_FormView3->currentindex)->GetType() == TYPE_NURBSSURFACE)
		{
			NURBSPickControlPoints(xWin,yWin);
		}else if(pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_MESH3D)
		{
			NURBSPickControlPoints(xWin,yWin);
		}else if(pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_MESH3D_OPENMESH)
		{
			MeshOpenmeshPickPoints(xWin,yWin);
		}*/
		//select Landmark 
		if (pMsg->wParam == VK_F || pMsg->wParam == VK_f)
		{
			Mesh3dSelectFace();
			return TRUE;
		}
		else if (pMsg->wParam == VK_L || pMsg->wParam == VK_l)
		{
			Mesh3dSelectLandmark();
			return TRUE;
		}
		else if (pMsg->wParam == VK_D || pMsg->wParam == VK_d)
		{
			Mesh3dUnSelectLandmark();
			return TRUE;
		}
		else if (pMsg->wParam == VK_W || pMsg->wParam == VK_w)
		{
			Mesh3dFlipEdge();
			return TRUE;
		}
		else if (pMsg->wParam == VK_T || pMsg->wParam == VK_t)
		{
			Mesh3dSelectEdge();
			return TRUE;
		}
		else if (pMsg->wParam == VK_A || pMsg->wParam == VK_a)
		{
			Mesh3dInputArea();
			return TRUE;
		}
		else if (pMsg->wParam == VK_S || pMsg->wParam == VK_s)
		{
			Mesh3dSelectSharpEdge();
			return TRUE;
		}
	}
	return CView::PreTranslateMessage(pMsg);
}
double CMyTreeView::TriangleArea(double x1, double y1, double x2, double y2, double x3, double y3)
{
	Vector3D v1(x2 - x1, y2 - y1, 0);
	Vector3D v2(x3 - x1, y3 - y1, 0);
	Vector3D vv = v1 ^ v2;
	Vector3D normal1(0, 0, 1);
	double len = vv * normal1;
	//double len = Length(vv) / 2;
	return len;
}
BOOL CMyTreeView::JudgeWhetherPointLiesInsideTriangle(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3, double TOL)
{
	Vector3D v1(x2 - x1, y2 - y1, 0);
	Vector3D v2(x - x1, y - y1, 0);
	Vector3D vv1 = v1 ^ v2;

	Vector3D v3(x3 - x2, y3 - y2, 0);
	Vector3D v4(x - x2, y - y2, 0);
	Vector3D vv2 = v3 ^ v4;

	Vector3D v5(x1 - x3, y1 - y3, 0);
	Vector3D v6(x - x3, y - y3, 0);
	Vector3D vv3 = v5 ^ v6;

	if (vv1[2] > 0 && vv2[2] > 0 && vv3[2] > 0)
		return TRUE;
	else
		return FALSE;
}
double CMyTreeView::JudgeWhetherPointLiesInsideTriangle_double(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3)
{
	Vector3D v1(x2 - x1, y2 - y1, 0);
	Vector3D v2(x - x1, y - y1, 0);
	Vector3D vv1 = v1 ^ v2;

	Vector3D v3(x3 - x2, y3 - y2, 0);
	Vector3D v4(x - x2, y - y2, 0);
	Vector3D vv2 = v3 ^ v4;

	Vector3D v5(x1 - x3, y1 - y3, 0);
	Vector3D v6(x - x3, y - y3, 0);
	Vector3D vv3 = v5 ^ v6;

	double minval = 1e10;
	if (vv1[2] < minval)
		minval = vv1[2];
	if (vv2[2] < minval)
		minval = vv2[2];
	if (vv3[2] < minval)
		minval = vv3[2];

	return minval;
}
BOOL CMyTreeView::JudgeWhetherPointLiesOnLineSegment(double x, double y, double x1, double y1, double x2, double y2, double TOL)
{
	//(x1,y1) As the original point and (x1,y1) -> (x2,y2)	As the xaxis
	Vector3D origin(x1, y1, 0);
	Vector3D xaxis(x2 - x1, y2 - y1, 0);
	double length = Length(xaxis);
	if (length < 1e-6)
	{
		if (fabs(x - x1) < 1e-6 && fabs(y - y1) < 1e-6)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		xaxis.SetUnit();
		Vector3D zaxis(0, 0, 1);
		Vector3D yaxis = zaxis ^ xaxis;
		Vector3D point(x, y, 0);
		double xcoorinate = (point - origin) * xaxis;
		double ycoorinate = (point - origin) * yaxis;
		if (xcoorinate >= 0 && xcoorinate <= length && fabs(ycoorinate) <= TOL)
			return TRUE;
		else
			return FALSE;
	}

}
double CMyTreeView::GetDistancebetweenPointandLineSegment(double x, double y, double x1, double y1, double x2, double y2)
{
	//(x1,y1) As the original point and (x1,y1) -> (x2,y2)	As the xaxis
	Vector3D origin(x1, y1, 0);
	Vector3D xaxis(x2 - x1, y2 - y1, 0);
	double length = Length(xaxis);
	if (length < 1e-6)
	{
		if (fabs(x - x1) < 1e-6 && fabs(y - y1) < 1e-6)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		xaxis.SetUnit();
		Vector3D zaxis(0, 0, 1);
		Vector3D yaxis = zaxis ^ xaxis;
		Vector3D point(x, y, 0);
		double xcoorinate = (point - origin) * xaxis;
		double ycoorinate = (point - origin) * yaxis;
		if (xcoorinate >= 0 && xcoorinate <= length)
			return fabs(ycoorinate);
		else
			if (xcoorinate <= 1e-8)
			{
				return sqrt(SQR(xcoorinate) + SQR(ycoorinate));
			}
			else
			{
				return sqrt(SQR(xcoorinate - length) + SQR(ycoorinate));
			}
	}
}
BOOL CMyTreeView::JudgeWhetherPointLiesOnFace(CFace3d* pFace3d)
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);

	CVertex3d* pVertex1 = pFace3d->v1();
	CVertex3d* pVertex2 = pFace3d->v2();
	CVertex3d* pVertex3 = pFace3d->v3();

	double x1 = pVertex1->x();
	double y1 = pVertex1->y();
	double z1 = pVertex1->z();
	double x2 = pVertex2->x();
	double y2 = pVertex2->y();
	double z2 = pVertex2->z();
	double x3 = pVertex3->x();
	double y3 = pVertex3->y();
	double z3 = pVertex3->z();

	GLdouble out1[3], out2[3], out3[3];
	Point3D outpoint1, outpoint2, outpoint3;
	gluProject(x1, y1, z1, pMesh->modelview, pMesh->projection, pMesh->m_viewPort, &(out1[0]), &(out1[1]), &(out1[2]));

	outpoint1.x = out1[0];
	outpoint1.y = pMesh->m_viewPort[3] - out1[1];
	outpoint1.z = out1[2];

	gluProject(x2, y2, z2, pMesh->modelview, pMesh->projection, pMesh->m_viewPort, &(out2[0]), &(out2[1]), &(out2[2]));
	outpoint2.x = out2[0];
	outpoint2.y = pMesh->m_viewPort[3] - out2[1];
	outpoint2.z = out2[2];

	gluProject(x3, y3, z3, pMesh->modelview, pMesh->projection, pMesh->m_viewPort, &(out3[0]), &(out3[1]), &(out3[2]));
	outpoint3.x = out3[0];
	outpoint3.y = pMesh->m_viewPort[3] - out3[1];
	outpoint3.z = out3[2];

	//Judge whether current cursor lies inside the triangle
	//BOOL b = JudgeWhetherPointLiesInsideTriangle(0.25, 0.25, 0, 0, 1, 0, 0, 1);
	BOOL b = JudgeWhetherPointLiesInsideTriangle(pt1.x, pt1.y, outpoint1.x, outpoint1.y, outpoint2.x, outpoint2.y, outpoint3.x, outpoint3.y);
	return b;

	//BOOL b = JudgeWhetherPointLiesOnLineSegment(pt1.x, pt1.y, outpoint1.x, outpoint1.y, outpoint2.x, outpoint2.y);
	//return b;
}
BOOL CMyTreeView::JudgeWhetherPointLiesOnEdge(CVertex3d* pVertex1, CVertex3d* pVertex2)
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);

	double x1 = pVertex1->x();
	double y1 = pVertex1->y();
	double z1 = pVertex1->z();
	double x2 = pVertex2->x();
	double y2 = pVertex2->y();
	double z2 = pVertex2->z();

	GLdouble out1[3], out2[3];
	Point3D outpoint1, outpoint2;
	gluProject(x1, y1, z1, pMesh->modelview, pMesh->projection, pMesh->m_viewPort, &(out1[0]), &(out1[1]), &(out1[2]));

	outpoint1.x = out1[0];
	outpoint1.y = pMesh->m_viewPort[3] - out1[1];
	outpoint1.z = out1[2];
	gluProject(x2, y2, z2, pMesh->modelview, pMesh->projection, pMesh->m_viewPort, &(out2[0]), &(out2[1]), &(out2[2]));

	outpoint2.x = out2[0];
	outpoint2.y = pMesh->m_viewPort[3] - out2[1];
	outpoint2.z = out2[2];

	//Judge whether current cursor lies on the edge
	BOOL b = JudgeWhetherPointLiesOnLineSegment(pt1.x, pt1.y, outpoint1.x, outpoint1.y, outpoint2.x, outpoint2.y);
	return b;
}
void CMyTreeView::Mesh3dFlipEdge()
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);
	pMesh->SetFlagOnVertices_ArrayIndex();
	for (int i = 0; i < pMesh->m_ArrayFace.GetSize(); i++)
	{
		CFace3d* pFace3d = pMesh->GetFace(i);

		int index = 0;
		for (int index = 0; index < 3; index++)
		{
			BOOL b = JudgeWhetherPointLiesOnEdge(pFace3d->v(index), pFace3d->v((index + 1) % 3));
			if (b)
			{
				if (pFace3d->f(index))
				{
					CFace3d* pFace3d1 = pFace3d->f(index);
					//switch
					CVertex3d* pVertex4 = pFace3d1->v1();
					int ii = 1;
					while (pVertex4 == pFace3d->v(index) || pVertex4 == pFace3d->v((index + 1) % 3))
					{
						pVertex4 = pFace3d1->v(ii);
						ii++;
					}

					//Insert Two Faces
					CFace3d* pNewFace1 = new CFace3d(pFace3d->v((index + 2) % 3), pFace3d->v(index), pVertex4);
					CFace3d* pNewFace2 = new CFace3d(pVertex4, pFace3d->v((index + 1) % 3), pFace3d->v((index + 2) % 3));
					pMesh->AddFace(pNewFace1);
					pMesh->AddFace(pNewFace2);
					pMesh->DeleteFace(pFace3d);
					pMesh->DeleteFace(pFace3d1);
					pMesh->BuildAdjacency();
					pMesh->SetModified(TRUE);
					sprintf(Post_Message, "Flip Faces Successfully!");
					::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
					Invalidate(TRUE);
					return;
				}
			}
		}
	}
}
void CMyTreeView::Mesh3dInputArea()
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);
	if (pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1)->GetType() != TYPE_MESH3D)
		return;
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);
	int size = pMesh->m_ArrayFacePointsArray.size();
	int flag = 0;
	int intersectionnum = 0;
	for (int i = 0; i < size; i++)
	{
		int size1 = pMesh->m_ArrayFacePointsArray[i].size();
		flag = 0;
		intersectionnum = 0;
		for (int j = 0; j < size1; j++)
		{
			int index1 = pMesh->m_ArrayFacePointsArray[i][j];
			int index2 = pMesh->m_ArrayFacePointsArray[i][(j + 1) % size1];

			CVertex3d* pVertex1 = pMesh->GetVertex(index1);
			CVertex3d* pVertex2 = pMesh->GetVertex(index2);

			double x1 = pVertex1->x();
			double y1 = pVertex1->y();
			double z1 = pVertex1->z();
			double x2 = pVertex2->x();
			double y2 = pVertex2->y();
			double z2 = pVertex2->z();

			if (fabs(x1 - x2) < 1e-8)
			{
				GLdouble out1[3], out2[3];
				Point3D outpoint1, outpoint2;
				gluProject(x1, y1, z1, pMesh->modelview, pMesh->projection, pMesh->m_viewPort, &(out1[0]), &(out1[1]), &(out1[2]));
				gluProject(x2, y2, z2, pMesh->modelview, pMesh->projection, pMesh->m_viewPort, &(out2[0]), &(out2[1]), &(out2[2]));

				outpoint1.x = out1[0];
				outpoint1.y = pMesh->m_viewPort[3] - out1[1];
				//outpoint1.y = out1[1];
				outpoint1.z = out1[2];

				outpoint2.x = out2[0];
				outpoint2.y = pMesh->m_viewPort[3] - out2[1];
				//outpoint2.y = out2[1];
				outpoint2.z = out2[2];
				if (outpoint1.x > pt1.x)
					if (pt1.y >= outpoint1.y && pt1.y <= outpoint2.y || pt1.y >= outpoint2.y && pt1.y <= outpoint1.y)
						intersectionnum++;
			}
		}
		if (intersectionnum % 2 == 1)
		{
			pMesh->m_currentfaceindex = i;
			::PostMessage(g_hwndMain, WM_SETAREA, 0, 0);
			//FaceInputArea dlg(::AfxGetApp()->GetMainWnd());
			//if(dlg.DoModal() == IDOK)
			//{
			//	pMesh->m_ArrayTargetFaceArea[pMesh->m_currentfaceindex++] = dlg.area;
			//}
			//pPolyOmino->OnBnClickedPolyominoSetArea();
			//pDoc->UpdateAllViews(NULL);
			return;
		}
	}

}
void CMyTreeView::Mesh3dSelectFace()
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);
	pMesh->SetVertexFlagAsItsIndex();
	for (int i = 0; i < pMesh->m_ArrayFace.GetSize(); i++)
	{
		CFace3d* pFace3d = pMesh->GetFace(i);

		BOOL b = JudgeWhetherPointLiesOnFace(pFace3d);

		if (b)
		{
			pFace3d->m_selectedflag = 1;
			sprintf(Post_Message, "Select Face %d", i);
			::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
			Invalidate(TRUE);
			return;
		}

	}
}
void CMyTreeView::Mesh3dSelectSharpEdge()
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);
	pMesh->SetVertexFlagAsItsIndex();
	for (int i = 0; i < pMesh->m_ArraySharpEdges.size(); i++)
	{
		std::pair<int, int> pair = pMesh->m_ArraySharpEdges[i];
		CVertex3d* pVertex1 = pMesh->GetVertex(pair.first);
		CVertex3d* pVertex2 = pMesh->GetVertex(pair.second);

		BOOL b = JudgeWhetherPointLiesOnEdge(pVertex1, pVertex2);
		if (b)
		{
			pMesh->m_ArraySelectedSharpEdgeIndex.push_back(i);
			sprintf(Post_Message, "Select Sharp Edge %d ", i);
			::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
			Invalidate(TRUE);
			return;
		}
	}
}
void CMyTreeView::Mesh3dSelectEdge()
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);
	pMesh->SetVertexFlagAsItsIndex();
	for (int i = 0; i < pMesh->m_ArrayFace.GetSize(); i++)
	{
		CFace3d* pFace3d = pMesh->GetFace(i);
		CVertex3d* pVertex1 = pFace3d->v1();
		CVertex3d* pVertex2 = pFace3d->v2();
		CVertex3d* pVertex3 = pFace3d->v3();

		int index = 0;
		for (int index = 0; index < 3; index++)
		{
			BOOL b = JudgeWhetherPointLiesOnEdge(pFace3d->v(index), pFace3d->v((index + 1) % 3));
			if (b)
			{
				int start = pFace3d->v(index)->m_Flag;
				int end = pFace3d->v((index + 1) % 3)->m_Flag;

				std::pair<int, int> pair;
				pair.first = start;
				pair.second = end;

				pMesh->m_ArraySharpEdges.push_back(pair);
				sprintf(Post_Message, "Select Edge(%d %d), Total Num:%d", start, end, pMesh->m_ArraySharpEdges.size());
				::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
				Invalidate(TRUE);
				return;
			}
		}
	}
}
void CMyTreeView::Mesh3dUnSelectLandmark()
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		//std::vector<int> landmark = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int index1 = pLandMark->landmark_points[j];
			int index2 = pLandMark->landmark_points[j + 1];
			CVertex3d* pVertex1 = pMesh->GetVertex(index1);
			CVertex3d* pVertex2 = pMesh->GetVertex(index2);

			BOOL b = JudgeWhetherPointLiesOnEdge(pVertex1, pVertex2);
			if (b)
			{
				std::vector<int>::iterator it;
				it = std::find(pMesh->m_SelectedLandmarkIndexArray.begin(), pMesh->m_SelectedLandmarkIndexArray.end(), i);
				if (it != pMesh->m_SelectedLandmarkIndexArray.end())
				{
					pMesh->m_SelectedLandmarkIndexArray.erase(it);
				}

				char landmarkindex[100];
				CString landmarkindexstr = "SelectedLandmark: ";
				for (int i = 0; i < pMesh->m_SelectedLandmarkIndexArray.size(); i++)
				{
					sprintf(landmarkindex, " %d ", pMesh->m_SelectedLandmarkIndexArray[i]);
					landmarkindexstr = landmarkindexstr + landmarkindex;
				}
				sprintf(Post_Message, "%s", landmarkindexstr);
				::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
				Invalidate(TRUE);
				return;
			}
		}
	}
}
void CMyTreeView::Mesh3dSelectLandmark()
{
	POINT pt1;
	GetCursorPos(&pt1);
	ScreenToClient(&pt1);
	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);
	for (int i = 0; i < pMesh->m_LandMarkArray.size(); i++)
	{
		GeoMapLandMark* pLandMark = pMesh->m_LandMarkArray[i];
		//std::vector<int> landmark = pMesh->m_LandMarkArray[i];
		for (int j = 0; j < pLandMark->landmark_points.size() - 1; j++)
		{
			int index1 = pLandMark->landmark_points[j];
			int index2 = pLandMark->landmark_points[j + 1];
			CVertex3d* pVertex1 = pMesh->GetVertex(index1);
			CVertex3d* pVertex2 = pMesh->GetVertex(index2);

			BOOL b = JudgeWhetherPointLiesOnEdge(pVertex1, pVertex2);
			if (b)
			{
				std::vector<int>::iterator it;
				it = std::find(pMesh->m_SelectedLandmarkIndexArray.begin(), pMesh->m_SelectedLandmarkIndexArray.end(), i);
				if (it == pMesh->m_SelectedLandmarkIndexArray.end())
				{
					pMesh->m_SelectedLandmarkIndexArray.push_back(i);
				}

				char landmarkindex[100];
				CString landmarkindexstr = "SelectedLandmark: ";
				for (int i = 0; i < pMesh->m_SelectedLandmarkIndexArray.size(); i++)
				{
					sprintf(landmarkindex, " %d ", pMesh->m_SelectedLandmarkIndexArray[i]);
					landmarkindexstr = landmarkindexstr + landmarkindex;
				}
				sprintf(Post_Message, "%s", landmarkindexstr);
				::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
				Invalidate(TRUE);
				return;
			}
		}
	}
}


