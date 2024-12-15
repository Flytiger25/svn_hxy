#pragma once
#include <afxdlgs.h>
#include <TColgp_Array1OfPnt.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include "mysql.h"
class BRep_CurveOnSurface;
class BRep_Curve3D;
class BRep_CurveOn2Surfaces;
class Shape_OCC;
class MYSQL;
class EntityList_ACIS;
class Shape_OCC;
class MYSQL;
class MYSQL_RES;
class BRep_Curve3D;
class EDGE;
class Action_SurfaceHealing :
	public CPropertyPage
{
	DECLARE_DYNAMIC(Action_SurfaceHealing)

public:

	MYSQL* conn;
	MYSQL_RES* res;
	int theCurrentRowIndex;
	int theCurrentModelId;
	CString theCurrentFilePath;

	Action_SurfaceHealing();
	virtual ~Action_SurfaceHealing();

	//
	void CheckAcisHausdorffdistanceAcis_Save(EntityList_ACIS* pACIS, char* filename);

	//
	void UpdateGapTime(int modelid, int timeCost, int occTime);

	//
	void InserttoTableModelFaceLack(Shape_OCC* pocc, int originalmodelid_beforedelteface, int deleteFaceIndex, int modelid, char* deleteFaceType, double theoriginalFaceArea, double MinArea, double MaxArea, int FaceLackBelowMinArea, int FaceLackBetweenMinAreaAndMaxArea, int FaceLackAboveMaxArea, int computeTime);

	//
	void DetectHuaweiGap(MYSQL_ROW row);

	//
	void DeleteFaceRandom(MYSQL_ROW row);

	//
	void CheckAcisHausdorffdistanceAcis(EntityList_ACIS* pACIS);

	void UpdateModelInformation(int id);


	//
	int CheckWhetherHasSameEndandMiddlePoints(EDGE* edge, gp_Pnt p1, gp_Pnt p2, gp_Pnt p3);

	//
	int FindCorrespondingAcisEdgeHausdorffDistance(EntityList_ACIS* pACIS, std::vector<gp_Pnt>& edgepoints, double& acishausdorff, int& edgePairIndex);

	//
	//int FindCorrespondingAcisEdgeIndex(EntityList_ACIS* pACIS, std::vector<gp_Pnt>& points);

	//
	int AddEndPointsandMiddlePoints(BRep_Curve3D* geomCurve, BRep_CurveOn2Surfaces* CurveOn2Surfaces, std::vector<gp_Pnt>& gapEdgePoints);

	//
	int AddEndPointsandMiddlePoints(BRep_Curve3D* curve, std::vector<gp_Pnt>& gapEdgePoints);

	//
	int AddEndPointsandMiddlePoints(BRep_CurveOnSurface* GC, std::vector<gp_Pnt>& gapEdgePoints);

	//
	int UpdateTableModelFile_BoundingBox(MYSQL*& conn, int id, double MinX, double MinY, double MinZ, double MaxX, double MaxY, double MaxZ);

	//
	int UpdateTableModelFile_ReleaseTime(MYSQL*& conn, int id, int Releasereadtime);

	//
	int GetHuaweiModelFaceLackNumber(MYSQL*& conn);

	//
	int GetHuaweiModelGapNumber(MYSQL*& conn);

	//
	int UpdateTableModelFile_DebugTime(MYSQL*& conn, int id, int debugreadtime);

	//
	int UpdateTableModelFile_ModelInfo(MYSQL*& conn, int id, int facenum, int edgenum, int vertexnum);

	//
	int UpdateTableModelFile_HuaweiLackDataSet(MYSQL*& conn, int id, int flag);

	//
	int UpdateTableModelFile_HuaweiGapDataSet(MYSQL*& conn, int id);

	//
	int FindCorrespondingAcisEdgeHausdorffDistance(EntityList_ACIS* pACIS, gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, double& acishausdorff, int& edgePairIndex);

	//
	int ChecktheHausdorffEquality(Shape_OCC* pOCC, EntityList_ACIS* pACIS);

	//
	int FindCorrespondingAcisEdgeIndex(EntityList_ACIS* pACIS, gp_Pnt p1, gp_Pnt p2, gp_Pnt p3);

	//
	void CheckEdgeCorrespondenceACIS_OCC(EntityList_ACIS* pACIS, Shape_OCC* pOCC);

	//
	double ComputeAreabyGreenTheorem(const std::vector<gp_Pnt2d>& boundary_Sampling2D);

	//
	double EvaluateDistanceBetween2CurveOnSurfaces_PointToPoint(BRep_CurveOnSurface* GC1, BRep_CurveOnSurface* GC2);

	//
	double EvaluateDistanceBetween2CurveOnSurfaces_PointDistanceToCurve(BRep_CurveOnSurface* GC1, BRep_CurveOnSurface* GC2);

	//
	double EvaluateDistanceBetween2CurveOnSurfaces(BRep_Curve3D* geomCurve, BRep_CurveOnSurface* GC1, BRep_CurveOn2Surfaces* CurveOn2Surfaces);

	//
	double EvaluateDistanceBetweenCurveson2PlanarSurfaces(BRep_Curve3D* geomCurve, BRep_CurveOn2Surfaces* CurveOn2Surfaces);

	//
	double ComputeHausdorffDistanceBetweenTwoPointSets(TColgp_Array1OfPnt& pts1, TColgp_Array1OfPnt& pts2);

	//
	void FindStepFilesRecursive(CString dir, std::vector<CString>& arrayFiles);

	//
	int ComputeLeastSquarePlaneof3DPolygon(const std::vector<gp_Pnt>& boundary_Sampling, double& a, double& b, double& c);

	//
	int UpdateTableModelFile_Description(MYSQL*& conn, int id, char* des);

	//
	int ComputeLeastSquarePlaneof3DPolygon_SDM(const std::vector<gp_Pnt>& boundary_Sampling, gp_Pnt& P, gp_XYZ& N, gp_XYZ& X, gp_XYZ& Y);

	//
	int Project3DPolygontoPlane(const std::vector<gp_Pnt>& boundary_Sampling, gp_Pnt O, gp_XYZ N, gp_XYZ X, gp_XYZ Y, std::vector<gp_Pnt2d>& boundary_Sampling2D);

	//1 sucess no error, 2 has gaps.
	int CheckModelGap_OCC(Shape_OCC* pocc, double h_min, double h_max, double& maxhausdorff);

	//0 success, otherwise return the number of lack faces
	int CheckModelFaceLack_OCC(Shape_OCC* pocc, int h_min, int h_max);

	//
	BOOL CheckWhetherEdgeBelongtoCurrentBoundary(Shape_OCC* pocc, std::pair<TopoDS_Edge, TopoDS_Face>& edge, std::vector<std::pair<TopoDS_Edge, TopoDS_Face>>& edgeArrayonCurrentBoundary);

	//
	int InserttoTableModelFile(MYSQL*& conn, const std::string& line, int occopenflag, int& id);

	//
	int UpdateTableModelFile_OCCOpenOK(MYSQL*& conn, int id);

	//
	void InserttoTableModelGap_Detail(int id, double maxhausdorff, int gapedgenum, int successnumber, int failurenumber);

	//
	int CheckWhetherFileisChecked(MYSQL*& conn, char* filepath);

	//
	double GetClosestPointOnCurve(const gp_Pnt& P, const BRep_CurveOnSurface* GC, TColgp_Array1OfPnt& pts, std::vector<double>& pts_para);

	//
	void UpdateModelInformation(const MYSQL_ROW& row);

	//
	int CheckModelFaceLack_OCC_AREA(Shape_OCC* pocc, double h_min, double h_max);

	//
	double ComputeAreaSurroundedbyCurrentBoundary(Shape_OCC* pocc, const std::vector<std::pair<TopoDS_Edge, TopoDS_Face>>& edgeArrayonCurrentBoundary);

	//
	void SamplingOpenBoundary(Shape_OCC* pOCC, const std::vector<std::pair<TopoDS_Edge, TopoDS_Face>>& edgeArrayonCurrentBoundary, std::vector<gp_Pnt>& curveSectionArray1_Sampling);

	//
	void InserttoTableModelGap(int id, double maxhausdorff, int gapedgenum);

	//
	void InserttoTableModelFaceLack(int id, int theFaceLackNumber);

	// Dialog Data
	enum { IDD = IDD_DIALOG_SURFACEHEALING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckEntity();
	afx_msg void OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonHausdorffdistancecheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAcisCheckFiles();
	afx_msg void OnBnClickedButtonFacelackcheck();
	afx_msg void OnBnClickedButtonMysql();
	afx_msg void OnBnClickedButtonFacelackcheckArea();
	afx_msg void OnBnClickedButtonHausdorffdistanceAcis();
	afx_msg void OnBnClickedButtonVerificationHausdorffdistanceAcis();
	afx_msg void OnBnClickedButtonLoadfirstmodel();
	afx_msg void OnBnClickedButtonLoadnextmodel();
	afx_msg void OnBnClickedButtonModifymodeldescription();
	afx_msg void OnBnClickedButtonAddtohuaweigapdataset();
	afx_msg void OnBnClickedButtonAddtohuaweilackdataset();
	afx_msg void OnBnClickedButtonLoadthemodel();
	afx_msg void OnBnClickedButtonLoadtheacismodel();
	afx_msg void OnBnClickedButtonAddtofacelackset();
	afx_msg void OnBnClickedButtonLoadthemodelwithid();
	afx_msg void OnBnClickedButtonFacelackautomatic();
	afx_msg void OnBnClickedButtonGapdetectionautomatic();
};

