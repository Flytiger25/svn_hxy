// MeshDoc.cpp : implementation of the CMeshDoc class
//

#include "stdafx.h"
#include "Mesh.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <GL\glaux.h>
#include "MeshDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshDoc

IMPLEMENT_DYNCREATE(CMeshDoc, CDocument)

BEGIN_MESSAGE_MAP(CMeshDoc, CDocument)
	//{{AFX_MSG_MAP(CMeshDoc)
	ON_COMMAND(ID_GL_ADD_WIREFRAME, OnGlAddWireframe)
	ON_UPDATE_COMMAND_UI(ID_GL_ADD_WIREFRAME, OnUpdateGlAddWireframe)
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshDoc construction/destruction

CMeshDoc::CMeshDoc()
{
	m_AddWireframe = 0;
	m_Smooth = 1;
	m_PolygonOffset = -1.0f;
	m_Light = TRUE;
	m_Mode = GL_FILL;
}

CMeshDoc::~CMeshDoc()
{
}

BOOL CMeshDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMeshDoc serialization

void CMeshDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMeshDoc diagnostics

#ifdef _DEBUG
void CMeshDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMeshDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMeshDoc commands

BOOL CMeshDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	// Extension-based checking
	CString file = lpszPathName;

	// Extension
	CString extension = lpszPathName;
	extension = extension.Right(4);
	extension.MakeLower();

	// Path "c:\path\file.wrl" -> c:\path
	CString path = lpszPathName;
	path = path.Left(path.ReverseFind('\\'));

	// CDocument
	SetCurrentDirectory(path);

	TRACE("\nOpening document\n");
	TRACE("File      : %s\n",lpszPathName);
	TRACE("Path      : %s\n",path);
	TRACE("Extension : %s\n",extension);

	// Start reading VRML file
	if(extension == ".wrl")
	{
		TRACE("wrl type\n");
		// Parser VRML 2.0
		CParserVrml parser;
		if(parser.Run((char *)lpszPathName,&m_SceneGraph))
		{
			m_SceneGraph.BuildAdjacency();
			m_SceneGraph.CalculateNormalPerFace();
			m_SceneGraph.CalculateNormalPerVertex();
			return TRUE;
		}
	}else
	if(extension == ".smf")
	{
		TRACE("smf type\n");
		// Parser VRML 2.0
		CParseSmf parser;
		if(parser.Run((char *)lpszPathName,&m_SceneGraph))
		{
			m_SceneGraph.BuildAdjacency();
			m_SceneGraph.CalculateNormalPerFace();
			m_SceneGraph.CalculateNormalPerVertex();
			return TRUE;
		}
	}

	return TRUE;
}


//////////////////////////////////////////////
//////////////////////////////////////////////
// RENDERING
//////////////////////////////////////////////
//////////////////////////////////////////////

//***********************************************
// RenderScene
//***********************************************
void CMeshDoc::RenderScene()
{
	// Main drawing
	glPolygonMode(GL_FRONT_AND_BACK,m_Mode);
	if(m_Light)
		::glEnable(GL_LIGHTING);
	else
		::glDisable(GL_LIGHTING);
	m_SceneGraph.glDraw();

	// Add wireframe (no light, and line mode)
	if(m_AddWireframe)
	{
		// Set state
		::glDisable(GL_LIGHTING);
		::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		::glEnable(GL_POLYGON_OFFSET_LINE);
		::glPolygonOffset(m_PolygonOffset,-1.0f);

		// Draw again...
		m_SceneGraph.glDraw(TYPE_MESH3D);

		// Restore light and mode
		::glDisable(GL_POLYGON_OFFSET_LINE);
		::glEnable(GL_LIGHTING);
	}

}



//***********************************************
// Smooth subdivision
//***********************************************
void CMeshDoc::OnMeshLoop() 
{
	BeginWaitCursor();
	int NbObject = m_SceneGraph.NbObject();
	for(int i=0;i<NbObject;i++)
	{
		CObject3d *pObject3d = m_SceneGraph[i];
	  if(pObject3d->GetType() == TYPE_MESH3D)
		{
			CMesh3d *pMesh  = (CMesh3d *)pObject3d;
			pMesh->SubdivisionLoop();
		}
	}
	UpdateAllViews(NULL);
	EndWaitCursor();
}
void CMeshDoc::OnUpdateMeshLoop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SceneGraph.NbObject() > 0);
}



BOOL CMeshDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// Extension
	CString extension = lpszPathName;
	extension = extension.Right(4);
	extension.MakeLower();

	// Path "c:\path\file.wrl" -> c:\path
	CString path = lpszPathName;
	path = path.Left(path.ReverseFind('\\'));

	TRACE("\nSaving document\n");
	TRACE("File      : %s\n",lpszPathName);
	TRACE("Path      : %s\n",path);
	TRACE("Extension : %s\n",extension);

	// Start reading VRML file
	if(extension == ".wrl")
	{
		TRACE("wrl type\n");
		m_SceneGraph.SaveFile((char *)lpszPathName);
	}

	return TRUE;
}

//****************************
// Mode light
//****************************
void CMeshDoc::OnGlLight() 
{
	m_Light = !m_Light;
	UpdateAllViews(NULL);
}
void CMeshDoc::OnUpdateGlLight(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Light);
}

//****************************
// Mode line
//****************************
void CMeshDoc::OnGlLine() 
{
	m_Mode = GL_LINE;
	UpdateAllViews(NULL);
}
void CMeshDoc::OnUpdateGlLine(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Mode == GL_LINE);
}


//****************************
// Mode vertex
//****************************
void CMeshDoc::OnGlVertex() 
{
	m_Mode = GL_POINT;
	UpdateAllViews(NULL);
}
void CMeshDoc::OnUpdateGlVertex(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Mode == GL_POINT);
}

//****************************
// Mode face
//****************************
void CMeshDoc::OnGlFace() 
{
	m_Mode = GL_FILL;
	UpdateAllViews(NULL);
}
void CMeshDoc::OnUpdateGlFace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Mode == GL_FILL);
}


//***********************************************
// Add wireframe
//***********************************************
void CMeshDoc::OnGlAddWireframe() 
{
	m_AddWireframe = !m_AddWireframe;
	UpdateAllViews(NULL);
}

void CMeshDoc::OnUpdateGlAddWireframe(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_AddWireframe);
}

//***********************************************
// Smooth shading
//***********************************************
void CMeshDoc::OnGlSmooth() 
{
	m_Smooth = !m_Smooth;
	if(m_Smooth)
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

void CMeshDoc::OnUpdateGlSmooth(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Smooth);
}

void CMeshDoc::OnMeshColorHeight() 
{
	// Rainbow height ramp
	CColorRamp ramp;
	ramp.BuildRainbow();
	((CMesh3d *)m_SceneGraph.GetAt(0))->ColorHeight(&ramp);
	UpdateAllViews(NULL);
}
