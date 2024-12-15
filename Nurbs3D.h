#ifndef _NURBS_3D_
#define _NURBS_3D_
// //////////////////////////////
// NURBS曲面
// //////////////////////////////
#include "lib3D/transform.h"
#include "curve.h"
#include "Dib.h"
#include "C_NurbsSurf.h"
#include "InterFunction.h"	//引这个文件主要是为了使用它里面定义的结构CS_NurbsCurve
#include "lib3d/Object3d.h"
#include "Transformation.h"
struct CS_NurbsSurf {
	int nu, nv;
	int orderu, orderv;
	double* knotu, * knotv;
	Point3D* ctrl;
	double* weight;
	//--------------Revised by YangLei 060330----记录曲面求交库内部辅助表示-----------
	//	CS_NurbsSurfAuxRep* auxRep;
	//	CS_NurbsSType type;
	//-----------------------------

	//--------------Ruan Xiao Yu --2005-09-30----为提高弹簧速度，记录曲面的边界线-------------
	CS_NurbsCurve* curve_left, * curve_top;
	//-------------------------------

	CS_NurbsSurf();
};
class CMesh3d;
class CNurbsSuface : public CObject3d
{
public:
	//data
	GLfloat* m_uknot;
	GLfloat* m_vknot;
	int            m_udegree;
	int            m_vdegree;
	GLfloat        m_ControlPoint[200][200][4];
	int			    m_Show;
	int            m_showColorBoundary;
	int            m_showSubdivisionCurves;
	int			   m_showIsoParameterCurvesGeneralBilinear;
	int			   m_showOptimalIsoParameterCurvesNonOpitimal;//iso
	int            m_showOptimalIsoParameterCurvesMobiusOpitmal;//mobius
	int            m_showOptimalIsoParameterCurvesMobiusLinearOpitmal;//mobius+bilinear
	int            m_showOptimalIsoParameterCurvesMobiusQuadraticOptimal;//mobius+quadratic
	int			   m_showOptimalIsoParameterCurves;
	int            m_showAreaPreservingIsoParameterCurves;  //Area preserving
	int			   m_showAreaPreservingIsoParameterCurves_Modified; //Area preserving modified.
	int            m_showSurface;
	int			   m_showControlPointsNetwork;
	int            m_showIsoParameterCurves;
	int			   m_showSubSurfaceCoordinate;
	int			   m_showBilinearCoefficients;
	int			   m_showColorOrthogonality;
	//CTransform	   m_Transform;
	int			   m_uknotcount;
	int			   m_vknotcount;
	CDib* m_pDib;
	GLfloat textureCntrlPnt[2][2][2];

	std::vector<double> alphaarray;
	std::vector<double> betaarray;
	//double alphaarray[100][100];
	//double betaarray[100][100];
	int alphaarraynum1, alphaarraynum2;
	int betaarraynum1, betaarraynum2;

	//Specified Iso-Parameter Curves.
	std::vector<double> uarray, uarraymodified;
	std::vector<double> varray, varraymodified;

	//the u & v interpolation curves
	C_NurbsCurve3D* pUInterOptiCurve, * pVInterOptiCurve;
	C_NurbsCurve3D* pUInterOptiModifyCurve, * pVInterOptiModifyCurve;

	C_NurbsSurf* pNurbsSurf;

	int  m_showOptimalIsoParameterCurves_demo10;
	int  m_showOptimalIsoParameterCurves_demo11;
	int  m_showOptimalIsoParameterCurves_demo12;
	int  m_showOptimalIsoParameterCurves_demo13;

	std::vector<int> m_ArrayHalfEdgeStructure;
public:
	//colors
	float s_r, s_g, s_b;
	float c_r, c_g, c_b;

public:
	//-----------------------------------------------------------
	// 0 don't use the reparameterization function
	// 1 Mobius transformation
	// 2 bilinear transformation
	// 3 Quadratic transformation
	// 4 general bilinear transformation
	Transformation* ptransformation;
	//reparatype	m_ReparameterizationType;
	//std::vector<double> reparameterization_coefficients;

	//void SetReparameterizationFunction(int flag);
	//void DisableReparameterizationFunction();
	void GetReparameterizedCoordinates(double& s, double& t);
	//void SetReparameterizationCoefficients(reparatype type,std::vector<double> &c);
	//double orthogonalityweight;
	//------------------------------------------------------------
public:

	//
	void GenerateSamplingMesh(CMesh3d*& pMesh3D, CMesh3d*& pMesh2D, int samplesm, int samplesn);

	//Compute the normal of the surface
	Vector3D GetParaNormal(double u, double v, BOOL IsUnit);

	//Compute the conformal energy
	double ComputeConformalEnergy();
	//u & v optimal scale;
	void GetInterpolationUVScale(double u, double v, double& uscale, double& vscale);

	//u & v modified scale
	void GetInterpolationModifiedUVScale(double u, double v, double& uscale, double& vscale);

	//interpolation uv values.
	void GetInterpolationUV(double& u, double& v);

	//modified uv values
	void GetInterpolationModifiedUV(double& u, double& v);

	//Draw the area preserving iso-parameter curves
	void ShowAreaPreservingIsoParameterCurves();

	//Draw the area preserving iso-parameter curves modified.
	void ShowAreaPreservingIsoParameterCurves_modified();

	//Read bilinear coefficients
	void ReadReparameterizationCoefficients(std::ifstream& file);

	//Show bilinear coefficients
	void ShowBilinearCoefficients();

	//Show sub surface coordinate
	void ShowSubSurfaceCoordinate();

	//Show the surface subdivision curves.
	void ShowSurfaceSubdivisionCurves();
	//copy 
	CNurbsSuface* copy();

	//Iso parameter curves for general bilinear
	void DrawIsoParameterCurves_GeneralBilinear();

	//
	void DrawIsoParameterCurves();

	//
	void Read(std::ifstream& file);

	//
	void Save(std::ofstream& file);

	//
	void DrawLines(double alpha, double beta);//根据alpha,beta值画线
	void DrawLines(double alpha1, double beta1, double alpha2, double beta2);
	void DrawLines(double alpha1, double beta1, double alpha2, double beta2, double alpha3, double beta3);
	void DrawIsoParameterCurvesNonOpitimal();//alpha=beta=0.5
	void DrawIsoParameterCurvesMobiusOpitmal();//use mobius transformation
	void DrawIsoParameterCurvesMobiusLinearOpitmal();//use mobius and linear transformation
	void DrawIsoParameterCurvesMobiusQuadraticOpitmal();//use mobius and quadratic transformation
	void OptimalReparameterization();

	//
	Vector3D GetControlPoint(int i, int j);

	//
	void SetControlPoint(int i, int j, Vector3D p);

	//
	double GetWeight(int i, int j);

	//
	void SetWeight(int i, int j, double w);

	CNurbsSuface();
	virtual ~CNurbsSuface();
	virtual int glDraw();

	C_RationalBezierCurve3D* GetUCurve(double v);
	C_RationalBezierCurve3D* GetVCurve(double u);
	int     GetUIntervalIndex(double u);
	int     GetVIntervalIndex(double v);
	Point3D ComputPosition(double u, double v);
	Vector4D GetControlPoint4D(int i, int j);
	CNurbsSuface* reverse();
	int GetCtrlNumberV();
	int GetCtrlNumberU();
	CNurbsSuface* VSplitSurface(double vmin, double vmax);
	int GetProjectCurve(double u1, double v1, double u2, double v2, C_NurbsCurve3D*& pCurve);

	//convert to C_NurbsSurf
	C_NurbsSurf* Converte_C_NurbsSurface();

	//Get transformed surface
	C_NurbsSurf* GetTransformedSurface();

	//Draw Coordinate Axis.
	void DrawCoordinateAxis();

	//Draw the control network
	void DrawControlPointsNetwork();

	//
	void DrawOriginalControlPointsNetwork();

	//
	void ShowColorBoundary();

	//Render by Triangulation
	void glRenderTriangulation();

	//Build list for the triangulation
	void BuildTriangulationList();

	//Get the bounding box
	void GetBoundingBox(Vector3D& lower, Vector3D& upper);

	virtual int PickTest(PickStruct& pickstruct);

public:
	double m1, n1, c1, startu, endu, startv, endv;
};

#endif