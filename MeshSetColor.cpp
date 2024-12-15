// MeshSetColor.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "MeshSetColor.h"
#include "lib3d\Mesh3D.h"
// MeshSetColor dialog

IMPLEMENT_DYNAMIC(MeshSetColor, CDialog)

MeshSetColor::MeshSetColor(CWnd* pParent /*=NULL*/)
	: CDialog(MeshSetColor::IDD, pParent)
{
	pMesh = NULL;
	pMesh_Openmesh = NULL;
}

MeshSetColor::~MeshSetColor()
{
}

void MeshSetColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MeshSetColor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MESH_SETSHARPEDGECOLOR, &MeshSetColor::OnBnClickedButtonMeshSetsharpedgecolor)
	ON_BN_CLICKED(IDC_BUTTON_MESH_SETFACECOLOR, &MeshSetColor::OnBnClickedButtonMeshSetfacecolor)
	ON_BN_CLICKED(IDC_BUTTON_MESH_SETEDGECOLOR, &MeshSetColor::OnBnClickedButtonMeshSetedgecolor)
END_MESSAGE_MAP()


// MeshSetColor message handlers
void MeshSetColor::SetMesh(CMesh3d* pMesh)
{
	this->pMesh = pMesh;
}
void MeshSetColor::OnBnClickedButtonMeshSetsharpedgecolor()
{
	// TODO: Add your control notification handler code here
	COLORREF color = RGB(255, 0, 0);       
	CColorDialog colorDlg(color);            

	if (IDOK == colorDlg.DoModal())         
	{   
		//  
		COLORREF color = colorDlg.GetColor();
		if(pMesh)
			pMesh->m_SharpeEdgeColor.Set(GetRValue(color),GetGValue(color),GetBValue(color));  
	}   
}


void MeshSetColor::OnBnClickedButtonMeshSetfacecolor()
{
	// TODO: Add your control notification handler code here
	COLORREF color = RGB(255, 0, 0);
	CColorDialog colorDlg(color);

	if (IDOK == colorDlg.DoModal())
	{
		//  
		COLORREF color = colorDlg.GetColor();
		if (pMesh)
			pMesh->m_FaceColor.Set(GetRValue(color), GetGValue(color), GetBValue(color));
	}
}



void MeshSetColor::OnBnClickedButtonMeshSetedgecolor()
{
	// TODO: Add your control notification handler code here
	COLORREF color = RGB(255, 0, 0);
	CColorDialog colorDlg(color);

	if (IDOK == colorDlg.DoModal())
	{
		//  
		COLORREF color = colorDlg.GetColor();
		if (pMesh)
			pMesh->m_EdgeColor.Set(GetRValue(color), GetGValue(color), GetBValue(color));
	}
}
