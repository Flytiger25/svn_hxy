// SDIViewSwitchDoc.cpp : implementation of the CSDIViewSwitchDoc class

#include "stdafx.h"
#include "lib3d\Base3d.h"
#include "SDIViewSwitch.h"
#include "SDIViewSwitchDoc.h"
#include "MainFrm.h"
#include "math.h"
#include "C_ParseNurbs.h"
#include <fstream>
#include "FormView3.h"
#include "Nurbs3d.h"
#include "ParseM.h"
#include "ParseNurbsCurve.h"
#include "MyTreeView.h"
#include "ParsePOLY.h"
#include "CParseSTEP.h"
#include "GeometricEngineSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
double CSDIViewSwitchDoc::SWITCHSPLIT_FILE_SIGNATURE = 0x1234567890123; //static member variable

IMPLEMENT_DYNCREATE(CSDIViewSwitchDoc, CDocument)

BEGIN_MESSAGE_MAP(CSDIViewSwitchDoc, CDocument)
	//{{AFX_MSG_MAP(CSDIViewSwitchDoc)
	ON_COMMAND(ID_GL_ADD_WIREFRAME, OnGlAddWireframe)
	ON_UPDATE_COMMAND_UI(ID_GL_ADD_WIREFRAME, OnUpdateGlAddWireframe)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_GL_SMOOTH, OnGlSmooth)
	ON_UPDATE_COMMAND_UI(ID_GL_SMOOTH, OnUpdateGlSmooth)
	ON_COMMAND(ID_MESH_LOOP, OnMeshLoop)
	ON_UPDATE_COMMAND_UI(ID_MESH_LOOP, OnUpdateMeshLoop)
	ON_COMMAND(ID_GL_LIGHT, OnGlLight)
	ON_COMMAND(ID_GL_LINE, OnGlLine)
	ON_UPDATE_COMMAND_UI(ID_GL_LINE, OnUpdateGlLine)
	ON_UPDATE_COMMAND_UI(ID_GL_LIGHT, OnUpdateGlLight)
	ON_COMMAND(ID_GL_VERTEX, OnGlVertex)
	ON_UPDATE_COMMAND_UI(ID_GL_VERTEX, OnUpdateGlVertex)
	ON_COMMAND(ID_GL_FACE, OnGlFace)
	ON_UPDATE_COMMAND_UI(ID_GL_FACE, OnUpdateGlFace)
	ON_COMMAND(ID_MESH_COLOR_HEIGHT, OnMeshColorHeight)
	ON_BN_CLICKED(IDC_NavigateStartPosition, OnBnClickedNavigatestartposition)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CSDIViewSwitchDoc* pDoc;
extern int m_ShowCoordinateAxis;
extern CString m_FileTextureName;
CSDIViewSwitchDoc::CSDIViewSwitchDoc()
{
	TRACE("\n\n-------\n");
	int number = 25;
	while (number > 0)
	{
		TRACE("A%d\n", --number % 4);
		number /= 5;
	}
	TRACE("\n\n-------\n");
	//TRACE("%f,%f,%f,%f,%f,%f",a,a +l,a+2*l,a+3*l,a+4*l,b);

	pDoc = this;
	m_bAddpoint = 0;
	m_nNumberOfRecords = 0;
	m_nRecordLength = sizeof(tagDOC_RECORD);
	m_nHeaderSize = sizeof(m_nNumberOfRecords) + sizeof(SWITCHSPLIT_FILE_SIGNATURE);
	m_DiscNum = 10;
	m_Reparemeterization = 0;
	m_AddWireframe = 1;
	m_Smooth = 1;
	m_PolygonOffset = -1.0f;
	m_Light = TRUE;
	m_Mode = GL_FILL;
	m_Disc1 = 8;
	m_Disc2 = 30;
	omega = 1;
	ShowTexture = 0;
	m_IsoParameterCurve = FALSE;
	m_ShowCoordinateAxis = 0;
	m_UsingOpenglNurbsRender = TRUE;
	m_Tessellation = 50;
	m_SetCameraMode = 0;

	m_FileTextureName = "D:\\papers\\code\\data\\texture\\circles_1.bmp";

	m_CurrentObjectIndex = 0;
	//
	//OnGenerateNewSurface();

	//unlock_license();

	//// 启动ACIS模型处理器
	//api_start_modeller(0);
}

/////////////////////////////////////////////////////////////////////////////
CSDIViewSwitchDoc::~CSDIViewSwitchDoc()
{
	TRACE("destructing CSDIViewSwitchDoc\n");
}
/////////////////////////////////////////////////////////////////////////////
//a virtual member function of CDocument: 
BOOL CSDIViewSwitchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//This application uses a transaction method of reading and writing data from the document.
//If your app does not require this, it may be much easier to do just use the MFC framework's
//default way of doing this and implement the Serialize() function. 
void CSDIViewSwitchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CSDIViewSwitchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSDIViewSwitchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
void CSDIViewSwitchDoc::UpdateTreeControl()
{
	POSITION pos = pDoc->GetFirstViewPosition();
	CFormView3* pFormView;
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CFormView3)))
			pFormView = (CFormView3*)pView;
	}
	pFormView->UpdateTreeControl();
}

void CSDIViewSwitchDoc::ZoomToFitCurrentMesh()
{
	m_SceneGraph.ZoomToFitCurrentMesh();
}
void CSDIViewSwitchDoc::ZoomAll()
{
	m_SceneGraph.ZoomAll();
}
void CSDIViewSwitchDoc::ZoomAllToFitCurrentMesh()
{
	m_SceneGraph.ZoomAllToFitCurrentMesh();
}
void CSDIViewSwitchDoc::ReadNurbsSet(LPCTSTR lpszPathName, CSceneGraph3d* pSceneGraph)
{
	std::ifstream file(lpszPathName);
	int surfacenum;
	CString path = lpszPathName;
	path = path.Left(path.ReverseFind('\\') + 1);

	file >> surfacenum;
	CString filename;
	char   name[80];
	for (int i = 0; i < surfacenum; i++)
	{
		file >> name;
		filename = name;
		filename = path + filename;

		TRACE("Nurbs type\n");
		CParseNurbs parser;
		if (parser.Run((LPSTR)(LPCSTR)filename, &m_SceneGraph))
		{
		}
	}
}
BOOL CSDIViewSwitchDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	//p_FormView3->MatlabTest();
	CString file = lpszPathName;
	//m_SceneGraph.Free();
	//Extension
	CString extension = lpszPathName;
	extension = extension.Right(extension.GetLength() - extension.ReverseFind('.'));
	extension.MakeLower();

	// Path "c:\path\file.wrl" -> c:\path
	CString path = lpszPathName;
	path = path.Left(path.ReverseFind('\\'));

	// CDocument
	SetCurrentDirectory(path);

	TRACE("\nOpening document\n");
	TRACE("File      : %s\n", lpszPathName);
	TRACE("Path      : %s\n", path);
	TRACE("Extension : %s\n", extension);

	int flag;
	if (extension == ".set")
	{
		//many surface
		TRACE("Nurbs Set\n");
		ReadNurbsSet((char*)lpszPathName, &m_SceneGraph);

	}
	else
		if (extension == ".sur")
		{
			//start to read the NURBS surface
			TRACE("Nurbs type\n");
			CParseNurbs parser;
			if (parser.Run((char*)lpszPathName, &m_SceneGraph))
			{
			}
		}
		else
			if (extension == ".obj")
			{
				// Start reading obj file
				TRACE("obj type\n");
				// Parser OBJ
				CParseOBJ parser;
				if (parser.Run2((char*)lpszPathName, &m_SceneGraph))
				{
					m_SceneGraph.BuildAdjacency();
					m_SceneGraph.CalculateNormalPerFace();
					m_SceneGraph.CalculateNormalPerVertex();
				}
			}
			else
				if (extension == ".m")
				{
					// Start reading m file
					TRACE("m type\n");
					CParseM parser;
					if (parser.Run((char*)lpszPathName, &m_SceneGraph))
					{
						m_SceneGraph.BuildAdjacency();
						m_SceneGraph.CalculateNormalPerFace();
						m_SceneGraph.CalculateNormalPerVertex();
					}
				}
				else
					if (extension == ".cur")
					{
						TRACE("Nurbs curve type\n");
						CParseNurbsCurve parser;

						//Read Curve
						parser.Run((char*)lpszPathName, &m_SceneGraph);
					}
					else
						if (extension == ".poly")
						{
							CParsePOLY parser;

							//Read Curve
							parser.Run_PSLG((char*)lpszPathName, &m_SceneGraph);
						}
						else
							if (extension == ".ele")
							{
								CParsePOLY parser;

								//Read Curve
								if (parser.Run_Element((char*)lpszPathName, &m_SceneGraph))
								{
									m_SceneGraph.BuildAdjacency();
									m_SceneGraph.CalculateNormalPerFace();
									m_SceneGraph.CalculateNormalPerVertex();
								}
							}
							else
								if (extension == ".step" || extension == ".stp")
								{
									CParseSTEP parser;

									flag = parser.Run_STEP((char*)lpszPathName, &m_SceneGraph);
								}
								else
									if (extension == ".sat" || extension == ".SAT")
									{
										CParseSTEP parser;

										flag = parser.Run_Acis((char*)lpszPathName, &m_SceneGraph, "ACIS");
									}
									else
										if (extension == ".igs" || extension == ".iges")
										{
											CParseSTEP parser;

											flag = parser.Run_Acis((char*)lpszPathName, &m_SceneGraph, "IGES");
										}
										else if (extension == ".brep")
										{
											CParseSTEP parser;
											parser.Run_OCC_BREP((char*)lpszPathName, &m_SceneGraph);
										}

	UpdateTreeControl();

	//Redraw
	m_SceneGraph.SetModifyFlag(TRUE);
	return flag;
}

/////////////////////////////////////////////////////////////////////////////////
void CSDIViewSwitchDoc::DeleteContents()
{
	//The file was reopened in the override of CDocument::OnOpenDocument.
	//So, now the file should be closed when the framework wants to
	//clear out the CDocument object for potential reuse.

	if (m_file.m_hFile != CFile::hFileNull) //CFile::hFileNull = -1
		m_file.Close();

	((CSDIViewSwitchApp*)AfxGetApp())->m_pDoc = NULL;
	m_nNumberOfRecords = 0;
}
/////////////////////////////////////////////////////////////////////////////////
void CSDIViewSwitchDoc::WriteHeader()
{
	m_file.Seek(0L, CFile::begin);
	//writing of the signature only really needs to be done the first time the header is written:
	m_file.Write(&SWITCHSPLIT_FILE_SIGNATURE, sizeof(double));
	//WriteHeader is called on other occassions to update the number of records:
	m_file.Write(&m_nNumberOfRecords, sizeof(m_nNumberOfRecords));
}
/////////////////////////////////////////////////////////////////////////////////
//Checks if it is a file of the correct type, and reads in the number of records:
BOOL CSDIViewSwitchDoc::ReadHeader()
{
	m_file.Seek(0L, CFile::begin);
	double fileSignature;
	m_file.Read(&fileSignature, sizeof(double));
	m_file.Read(&m_nNumberOfRecords, sizeof(UINT));
	return(fileSignature == SWITCHSPLIT_FILE_SIGNATURE);
}
/////////////////////////////////////////////////////////////////////////////////
void CSDIViewSwitchDoc::FileSeekRecord(UINT nRecord)
{
	m_file.Seek(m_nHeaderSize + nRecord * m_nRecordLength, CFile::begin);
}
/////////////////////////////////////////////////////////////////////////////////
bool CSDIViewSwitchDoc::GetRecord(UINT nRecordIndex, void* pRecord)
{
	FileSeekRecord(nRecordIndex);
	if (m_file.Read(pRecord, m_nRecordLength) < m_nRecordLength)
	{
		AfxMessageBox("There is data corruption");
		return false;
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////
void CSDIViewSwitchDoc::UpdateRecord(UINT nRecordIndex, void* pRecord)
{
	FileSeekRecord(nRecordIndex);
	m_file.Write(pRecord, m_nRecordLength);
}
/////////////////////////////////////////////////////////////////////////////////
BOOL CSDIViewSwitchDoc::SaveModified()
{   //It is needed to override this virtual CDocument function because if the document 
	//is modified, CDocument::SaveModified() will produce a sharing violation because 
	//the file is open already. Since we're saving data on a transaction basis, the 
	//data is already saved. All we need to do is close the file:
	DeleteContents();

	return TRUE; // keep going   
}
//***********************************************
// RenderScene
//***********************************************

void CSDIViewSwitchDoc::RenderScene()
{

	//更新显示状态
	POSITION pos = pDoc->GetFirstViewPosition();
	CFormView3* pFormView;
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CFormView3)))
			pFormView = (CFormView3*)pView;
	}
	pFormView->UpdateObjectShowState();

	// Main drawing
	m_SceneGraph.glDraw();


	//m_SceneGraph.glDraw_Accelerate();
}

void CSDIViewSwitchDoc::SetTransform(float m_xTranslation, float m_yTranslation, float m_zTranslation,
	float m_xRotation, float m_yRotation, float m_zRotation,
	float m_xScaling, float m_yScaling, float m_zScaling)
{
	// Main drawing
	m_SceneGraph.SetTransform(m_xTranslation, m_yTranslation, m_zTranslation,
		m_xRotation, m_yRotation, m_zRotation,
		m_xScaling, m_yScaling, m_zScaling);
	//m_SceneGraph.glDraw_Accelerate();
}

void CSDIViewSwitchDoc::RenderSceneCDC()
{
	for (int i = 0; i < m_SceneGraph.NbObject(); i++)
	{
		CObject3d* pObject = m_SceneGraph.GetAt(i);
		if (pObject->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pNurbs = (CNurbsSuface*)pObject;
			pNurbs->ShowSubSurfaceCoordinate();
			pNurbs->ShowBilinearCoefficients();
		}
	}
}

void CSDIViewSwitchDoc::OnMeshColorHeight()
{
	// Rainbow height ramp
	CColorRamp ramp;
	ramp.BuildRainbow();
	((CMesh3d*)m_SceneGraph.GetAt(0))->ColorHeight(&ramp);
	UpdateAllViews(NULL);
}
//****************************
// Mode face
//****************************
void CSDIViewSwitchDoc::OnGlFace()
{
	m_Mode = GL_FILL;
	UpdateAllViews(NULL);
}
void CSDIViewSwitchDoc::OnUpdateGlFace(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_Mode == GL_FILL);
}
//****************************
// Mode vertex
//****************************
void CSDIViewSwitchDoc::OnGlVertex()
{
	m_Mode = GL_POINT;
	UpdateAllViews(NULL);
}
void CSDIViewSwitchDoc::OnUpdateGlVertex(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_Mode == GL_POINT);
}
//****************************
// Mode light
//****************************
void CSDIViewSwitchDoc::OnGlLight()
{
	m_Light = !m_Light;
	UpdateAllViews(NULL);
}
void CSDIViewSwitchDoc::OnUpdateGlLight(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_Light);
}
//****************************
// Mode line
//****************************
void CSDIViewSwitchDoc::OnGlLine()
{
	m_Mode = GL_LINE;
	UpdateAllViews(NULL);
}
void CSDIViewSwitchDoc::OnUpdateGlLine(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_Mode == GL_LINE);
}
//***********************************************
// Smooth subdivision
//***********************************************
void CSDIViewSwitchDoc::OnMeshLoop()
{
	BeginWaitCursor();
	int NbObject = m_SceneGraph.NbObject();
	for (int i = 0; i < NbObject; i++)
	{
		CObject3d* pObject3d = m_SceneGraph[i];
		if (pObject3d->GetType() == TYPE_MESH3D)
		{
			CMesh3d* pMesh = (CMesh3d*)pObject3d;
			pMesh->SubdivisionLoop();
		}
	}
	UpdateAllViews(NULL);
	EndWaitCursor();
}
void CSDIViewSwitchDoc::OnUpdateMeshLoop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_SceneGraph.NbObject() > 0);
}
//***********************************************
// Smooth shading
//***********************************************
void CSDIViewSwitchDoc::OnGlSmooth()
{
	m_Smooth = !m_Smooth;
	if (m_Smooth)
	{
		m_SceneGraph.SetNormalBinding(NORMAL_PER_VERTEX);
		::glShadeModel(GL_SMOOTH);
		m_SceneGraph.BuildAdjacency();
	}
	else
	{
		m_SceneGraph.SetNormalBinding(NORMAL_PER_FACE);
		::glShadeModel(GL_FLAT);
		m_SceneGraph.BuildAdjacency();
	}
	UpdateAllViews(NULL);
}

void CSDIViewSwitchDoc::OnUpdateGlSmooth(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_Smooth);
}
/////////////////////////////////////////////////////////////////////////////////

void CSDIViewSwitchDoc::OnGlAddWireframe()
{
	// TODO: Add your command handler code here
	m_AddWireframe = !m_AddWireframe;
	UpdateAllViews(NULL);
}

void CSDIViewSwitchDoc::OnUpdateGlAddWireframe(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_AddWireframe);
}

void CSDIViewSwitchDoc::OnFileClose()
{
	// TODO: Add your command handler code here
	m_SceneGraph.Free();
	UpdateAllViews(NULL);
}

void CSDIViewSwitchDoc::OnBnClickedNavigatestartposition()
{
	// TODO: Add your control notification handler code here
	int ii = 0;
}
void CSDIViewSwitchDoc::UpdateReadTree()
{
	POSITION pos = pDoc->GetFirstViewPosition();
	CFormView3* pFormView;
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CFormView3)))
			pFormView = (CFormView3*)pView;
	}
	pFormView->UpdateReadTree();
}

void CSDIViewSwitchDoc::UpdateShowView()
{
	CMyTreeView* pTreeView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
		{
			pTreeView = (CMyTreeView*)pView;
			pTreeView->Invalidate(TRUE);
			break;
		}
	}
}

