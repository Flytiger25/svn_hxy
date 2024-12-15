#pragma once


// Actions_GeneralBilinear dialog
class CMesh3d;
class CNurbsSuface;
class Actions_GeneralBilinear : public CPropertyPage
{
	DECLARE_DYNAMIC(Actions_GeneralBilinear)

public:
	Actions_GeneralBilinear();
	virtual ~Actions_GeneralBilinear();

	int optimalxindex,optimalyindex;
	void ComputeInitialGeneralBilinearTransformation(CMesh3d* pMesh1,CMesh3d* pMesh2,std::vector<std::vector<double>> &optinumx,int flag);

	//Compute the sign of linear search gradient
	//0 the local minimum
	//-1 the minimum is the on the right
	//1 the minimum is the on the left
	int LinearSearch_GradientSign(int index,CNurbsSuface* pSurf);

	//Linear Search
	int LinearSearch(int index,CNurbsSuface* pSurf);

	//
	void OutPutConformity_PovRay_original(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);

	//
	void OutPutConformity_PovRay_result(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);

	//
	void OutPutTexture_PovRay_Terrain_Original(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);

	//
	void OutPutMeshEdges_PovRay_Terrain_Original(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);

	//
	void OutPutMeshFaces_PovRay_Terrain_Original(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);

	//Result
	void OutPutTexture_PovRay_Terrain_Result(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);

	//
	void OutPutMeshEdges_PovRay_Terrain_Result(CNurbsSuface* pSurface, int samplesm,int samplesn,char* filename);


	//Compute the current conformal energy
	double ComputeConformalEnergy(CNurbsSuface* pSurf);

	//Set conformal weight
	void SetConformalEnergyWeight(CNurbsSuface* pSurf);

	//for degenerate cases
	void OnBnClickedActionsGeneralbilinearInitialvalue_Degenerate();

	//Initial value
	void OnBnClickedActionsGeneralbilinearInitialvalue();

	//check 
	int CheckSingularLineInParameterDomain(double f00,double f01,double f10,double f11,double umin,double vmin,double umax,double vmax);

// Dialog Data
	enum { IDD = IDD_ActionsGeneralBilinear };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedActionsBezier();
	afx_msg void OnBnClickedIdcActionsGeneralbilinearGeneratesubmesh();
	afx_msg void OnBnClickedActionsGeneralbilinearOptimization();
	int m_orthogonalityweight;
	afx_msg void OnBnClickedGeneralbilinearPovray();
	afx_msg void OnBnClickedButtonCartop();
	afx_msg void OnBnClickedButtonFace();
	afx_msg void OnBnClickedButtonBone();
	afx_msg void OnBnClickedButtonVenus();
	afx_msg void OnBnClickedButtonSophie();
	afx_msg void OnBnClickedButtonTerrain();
	afx_msg void OnBnClickedActionsGeneralbilinearPlot();
	afx_msg void OnBnClickedButtonGeneralbilinearinitialvalue();
};
