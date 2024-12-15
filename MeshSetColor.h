#pragma once


// MeshSetColor dialog
class CMesh3d;
class CMesh3d_OpenMesh;
class MeshSetColor : public CDialog
{
	DECLARE_DYNAMIC(MeshSetColor)

public:
	MeshSetColor(CWnd* pParent = NULL);   // standard constructor
	virtual ~MeshSetColor();

	CMesh3d* pMesh;
	CMesh3d_OpenMesh* pMesh_Openmesh;
// Dialog Data
	enum { IDD = IDD_MESH_SETCOLORS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	void SetMesh(CMesh3d* pMesh);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMeshSetsharpedgecolor();
	afx_msg void OnBnClickedButtonMeshSetfacecolor();
	afx_msg void OnBnClickedButtonMeshSetedgecolor();
};
