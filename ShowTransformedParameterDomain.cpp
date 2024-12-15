// ShowTransformedParameterDomain.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "ShowTransformedParameterDomain.h"
#include "Nurbs3D.h"
#include "Transformation.h"
#include "Lib3D/Base3d.h"
#include "Lib3D/SceneGraph3d.h"
#include "FormView3.h"
// C_ShowTransformedParameterDomain dialog

IMPLEMENT_DYNAMIC(C_ShowTransformedParameterDomain, CDialog)

C_ShowTransformedParameterDomain::C_ShowTransformedParameterDomain(CWnd* pParent /*=NULL*/)
	: CDialog(C_ShowTransformedParameterDomain::IDD, pParent)
{

}

C_ShowTransformedParameterDomain::~C_ShowTransformedParameterDomain()
{
}

void C_ShowTransformedParameterDomain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(C_ShowTransformedParameterDomain, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// C_ShowTransformedParameterDomain message handlers

void C_ShowTransformedParameterDomain::DrawMeshes()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num=pDoc->m_SceneGraph.NbObject();
	if(num != 3)
		return;

	CMesh3d* pMesh1 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(0);
	CMesh3d* pMesh2 = (CMesh3d*)pDoc->m_SceneGraph.GetAt(1);

	Point3D lower,upper;
	pMesh2->ComputeBoundingBox(lower,upper);
	Point3D center((lower.x + upper.x)/2, (lower.y + upper.y) /2, (lower.z + upper.z)/2);
	double length = upper.x - lower.x;
	if(length < upper.y - lower.y)
		length = upper.y - lower.y;

	int startx = 100;
	int starty = 100;
	double length1 = 500;
	double scale = length1 / length;

	int uknotnum = 10;
	int vknotnum = 10;

	//draw all the parameter grid

	double umin = lower.x;
	double umax = upper.x;
	double vmin = lower.y;
	double vmax = upper.y;

	CClientDC dc(this);
	
	dc.MoveTo(startx,starty);
	dc.LineTo(startx + (upper.x - lower.x) * scale,starty);
	dc.LineTo(startx + (upper.x - lower.x) * scale, starty +  (upper.y - lower.y) * scale);
	dc.LineTo(startx,starty +  (upper.y - lower.y) * scale);
	dc.LineTo(startx,starty);

	for(int i  = 1; i <= uknotnum; i++)
	{
		dc.MoveTo(startx + (upper.x - lower.x) * i * scale / (uknotnum + 1) , starty);
		dc.LineTo(startx + (upper.x - lower.x) * i * scale / (uknotnum + 1) , starty +  (upper.y - lower.y) * scale);
	}

	for(int i  = 1; i <= vknotnum; i++)
	{
		dc.MoveTo(startx,starty + (upper.y - lower.y) * i * scale / (vknotnum + 1) );
		dc.LineTo(startx + (upper.x - lower.x) * scale,starty + (upper.y - lower.y) * i * scale / (vknotnum + 1));
	}

	for(int i = 0; i < pMesh2->NbVertex(); i++)
	{
		CVertex3d* pVertex = pMesh2->GetVertex(i);
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();

		dc.Ellipse(startx + (x - lower.x) * scale - 1,starty + (y - lower.y) * scale - 1, startx + (x - lower.x) * scale + 1,starty + (y - lower.y) * scale + 1);
	}
}
void C_ShowTransformedParameterDomain::OnPaint()
{
	//DrawMeshes();
	//DrawGeneralBilinear();
}
