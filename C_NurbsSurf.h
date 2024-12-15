
#ifndef _PARA_SURF_H_
#define _PARA_SURF_H_
// //////////////////////////////
// 有理Bezier曲面
// //////////////////////////////
#include "curve.h"
#include <vector>

class CNurbsSuface;
using namespace std;
typedef vector<double>  KnotVector;
struct __declspec(dllexport) CS_RBezierSurf {
    int nu, nv;
    Point3D *ctrl;  	// j*nu+i, 0<=i<=nu-1, 0<=j<=nv-1,
    double  *weight;
    CS_RBezierSurf();
} ;
// //////////////////////////////
// 非均匀B样条曲面
// //////////////////////////////
struct __declspec(dllexport) CS_BSplineSurf {
    int nu, nv;
    int orderu, orderv;
    double *knotu, *knotv;
    Point3D *ctrl;
    CS_BSplineSurf();
} ;
class CompositeMobiusTransformation;
class CMyObject : public CObject
{
public:	
    void * operator new(size_t);
    void operator delete(void *);
    void * operator new[](size_t);
    void operator delete[](void *);
	
	//=====================denug needed=========================
	void*  operator new(size_t nSize, char* const lpszFileName, int nLine);
	void  operator delete(void* pData,  char* const lpszFileName ,int  nLine );
	void*  operator new[](size_t nSize,  char* const lpszFileName, int nLine);
	void  operator delete[](void* pData,  char* const lpszFileName ,int  nLine );
};

/////////////////////////////////////////////////////
/////// 3D entities, used in Euclidean space ////////
/////////////////////////////////////////////////////


class C_Surf3D : public C_Entity3D
{
private:
	
public:
	C_Surf3D();
	virtual ~C_Surf3D() ;
	
	// 用于数据存取和显示的成员函数
	
	// 求曲面上一点的参数值，返回值含义:
	//   1: 对应单个参数，uv为点参数值
	//   n(>1): 对应n个参数，alluv为点参数值(存储空间需由调用程序释放)
	//   -1: u值任意，uv为某一个参数
	//   -2: v值任意，uv为某一个参数
	//   -3: u值和v值都任意，uv为某一个参数
	virtual int GetPointPara(Point3D p, Point2D &uv, Point2D *&alluv);
	// 求曲面参数域中一点对应的空间点坐标值
	virtual Point3D GetParaPoint(double u, double v);
    virtual Point3D GetParaPoint(Point2D uv);
	// 求曲面参数域中一点对应点的法向量
	virtual Vector3D GetParaNormal(double u, double v);
    virtual Vector3D GetParaNormal(Point2D uv);
	//By Chen xiaodiao 2005-11-29
	//p  给定点
	//initUV  可能给定的参使兰?
	//pt 最近点
	//最近点对应的参数
	//返回 1---成功；0---失败
    virtual int GetClosestPoint(const Point3D& p, Point2D *initUV, Point3D& ptOnsurf, Point2D &uv);	
    // 判点是否在曲面上
	virtual BOOL PointOn(Point3D p, double toler);
	// 拷贝
	virtual C_Surf3D *Copy();
	// 等距
	virtual C_Surf3D *Offset(BOOL dirtag, double depth);
	// 延展
	virtual void Expand(double du, double dv);
	// 判点在面的正向
	virtual BOOL PointInPositiveSpace(Point3D point);
	// 求包围核
	
	//求曲率
	//virtual double ComputCurvature(Point3D point);
	
	
	// 记录离散的Bezier曲面片，以此加快反求的速度
	
	double *sArray, *tArray;                       //记录离散后的参数
	int snum, tnum;
};

class C_ParaSurf : public C_Surf3D
{
		
protected:
	int     m_number_u,m_number_v;  // 控制点数
	Point3D *m_control;             // 曲面控制点
	
protected:
	int     IndexMap(int i,int j);  // 从(i,j)得到表中序号
public:
	C_ParaSurf(void);
	C_ParaSurf(int number_u,int number_v);
	C_ParaSurf(int number_u,int number_v,Point3D *control);
	~C_ParaSurf(void); // 释放m_control的内存
	// 用于数据存取和显示的成员函数
	int     GetCtrlNumberU(void);
	int     GetCtrlNumberV(void);
	Point3D GetControlPoint(int i, int j);
	void    SetCtrlNumber(int number_u,int number_v);
	void    SetControlPoint(int i,int j,Point3D point);



};

class C_BSplineSurf : public C_ParaSurf
{
		
protected:
	int m_order_u,m_order_v; // 曲线阶数
public:
	// 构造函数和析构函数
	C_BSplineSurf(void);
	C_BSplineSurf(int number_u,int number_v, int order_u, int order_v);
	C_BSplineSurf(int number_u,int number_v, int order_u, 
		int order_v, Point3D *control);
	~C_BSplineSurf(void);
	// 用于数据存取和显示的成员函数
	int GetOrderU(void);
	int GetOrderV(void);
	int GetDegreeU(void);
	int GetDegreeV(void);
	void SetOrder(int order_u,int order_v);
	int PrintInformation(void);
};

class  C_NonuniformBSurf : public C_BSplineSurf
{
protected:
	double  *m_knot_u,*m_knot_v;   // 曲线节点向量
public:
	// 构造函数和析构函数
	C_NonuniformBSurf(void);
	C_NonuniformBSurf(int number_u,int number_v, int order_u, int order_v);
	C_NonuniformBSurf(int number_u,int number_v, int order_u, int order_v, 
		double *knot_u, double *knot_v, Point3D *control);
	~C_NonuniformBSurf(void);
	// 用于数据存取和显示的成员函数
	virtual void SetNumberOrder(int number_u, int number_v, int order_u, int order_v);
	double  GetUKnot(int index);
	double  GetVKnot(int index);
	void    SetUKnot(int index,double knot);
	void    SetVKnot(int index,double knot);
	int     PrintInformation(void);
	int     GetUIntervalIndex(double u);
	int     GetVIntervalIndex(double v);
	virtual Point3D  ComputPosition(double u, double v); 
	virtual Vector3D ComputUDerivative1(double u, double v); 
	virtual Vector3D ComputVDerivative1(double u, double v); 
	virtual void ComputAll(double u, double v, Point3D &position, Vector3D &xvector, Vector3D &yvector);
	virtual Vector3D ComputUDerivative2(double u, double v); 
	virtual Vector3D ComputVDerivative2(double u, double v); 
	virtual Vector3D ComputUVDerivative2(double u, double v); 
	virtual void     ComputDerivative2All(double u, double v, Point3D &position, Vector3D &Uvector, Vector3D &Vvector, Vector3D &U2vector, Vector3D &V2vector, Vector3D &UVvector);
	virtual C_Curve3D *GetUCurve(double v);
	virtual C_Curve3D *GetVCurve(double u);
	virtual int GetUKnotMulti(double value);
	virtual int GetVKnotMulti(double value);
	virtual void InsertMultiUKnot(int multiplicity, double value);
	virtual void InsertMultiVKnot(int multiplicity, double value);
	

	
	// 求曲面上一点的参数值，返回值含义:
	//   1: 对应单个参数，uv为点参数值
	//   n(>1): 对应n个参数，alluv为点参数值(存储空间需由调用程序释放)
	//   -1: u值任意，uv为某一个参数
	//   -2: v值任意，uv为某一个参数
	//   -3: u值和v值都任意，uv为某一个参数

	virtual Point3D GetParaPoint(double u, double v);
	// 求曲面参数域中一点对应的空间点的法向
	virtual Vector3D GetParaNormal(double u, double v);
	// 求曲面参数域中一点对应点的法向量
	virtual Vector3D GetParaNormal(double u, double v, BOOL IsUnit);

	// 拷贝
	virtual C_Surf3D *Copy(void);

	

};

class C_NurbsSurf : public C_NonuniformBSurf
{
		
private:
	double *m_weight;  // 控制点权值

public:
	// 构造函数和析构函数
	C_NurbsSurf(void);
	C_NurbsSurf(int number_u,int number_v, int order_u, int order_v);
	C_NurbsSurf(int number_u,int number_v, int order_u, int order_v, 
		double *knot_u, double *knot_v, Point3D *control, double *weight);
	~C_NurbsSurf(void);
	// 用于数据存取和显示的成员函数
	void   SetNumberOrder(int number_u, int number_v, int order_u, int order_v);
	double GetWeight(int i,int j);
	void   SetWeight(int i,int j,double weight);
	int    PrintInformation(void);

	//
	void ComputeControlPointBoundingBox(Point3D &minpoint, Point3D &maxpoint);

	Point3D  ComputPosition(double u, double v); 
	Vector3D ComputUDerivative1(double u, double v); 
	Vector3D ComputVDerivative1(double u, double v); 
	void     ComputAll(double u, double v, Point3D &position, Vector3D &xvector, Vector3D &yvector);
	Vector3D ComputUDerivative2(double u, double v); 
	Vector3D ComputVDerivative2(double u, double v); 
	Vector3D ComputUVDerivative2(double u, double v); 
	void     ComputDerivative2ALL(double u, double v, Point3D &position, Vector3D &Uvector, Vector3D &Vvector, Vector3D &U2vector, Vector3D &V2vector, Vector3D &UVvector);
	C_Curve3D *GetUCurve(double v);
	C_Curve3D *GetVCurve(double u);
	void InsertMultiUKnot(int multiplicity, double value);
	void InsertMultiVKnot(int multiplicity, double value);
	int  GetUKnotMulti(double u);
	int  GetVKnotMulti(double v);
	void NurbssRemoveKnot(int dir, int index);
	Vector3D GetParaNormal(double u, double v, BOOL IsUnit);
	void SubdividSurface();
	C_Surf3D *Copy(void);
	CNurbsSuface* GetCNurbsSurface();

	//Compute the subsurface from (minu,minv)->(maxu,maxv)
	C_NurbsSurf* GetSubSurface(double minu,double maxu,double minv,double maxv,int flag = 0);

	//Output the surface knots
	void TraceKnots();

	//----------------------Transform surface------------------------------
	//Get the Uindex
	int GetUIntervalIndex_CompositeMobius(double u,CompositeMobiusTransformation* pTransformation);

	//Get the Vindex
	int GetVIntervalIndex_CompositeMobius(double v,CompositeMobiusTransformation* pTransformation);

	//Compute its Composite Mobius transformed surface
	C_NurbsSurf* GetCompositeMobiusTransformedSurface(CompositeMobiusTransformation* pTransformation);

	//Compute its Mobius transformed surface
	C_NurbsSurf* GetMobiusTransformedSurface(double a, double b);

	//Compute its Bilinear reparameterized surface
	C_NurbsSurf* GetBilinearTransformedSurface(double a1,double a2,double b1,double b2);

	//Compute the general bilinear transformaiton surface
	C_NurbsSurf* GetGeneralBilinearTransformedSurface
		(double e00,double e01,double e10,double e11,double f00,double f01,double f10,double f11,
		 double g00, double g01,double g10,double g11,double h00,double h01,double h10,double h11,
		 double umin,double umax,double vmin,double vmax);

	//Compute its quadratic reparameterized surface
	C_NurbsSurf* QuadraticTransformedSurface(double a1,double a2,double a3,double b1,double b2,double b3);
	//----------------------End Transform Surface-------------------------------

	Vector3D ComputeQuadraticControlPoint(C_NurbsSurf* pSurface,int m,int n,int k1,int k2,double a1,double a2,double b1,double b2, double& weight);
	double ComputeQuadraticControlPoint_clk(int l,int k, int m, int n,double a1,double a2,double b1,double b2);
	double C_NurbsSurf::ComputeQuadraticControlPoint_dlk(int l,int k, int m, int n,double a1,double a2,double b1,double b2);
	double Compute_cubicclk(int l,int k,int m,int n,double a1,double a2,double a3);
	int  GetNextUknotIndex(int index);
	int  GetNextVknotIndex(int index);

	//
	Point3D GetClosestPoint(Point3D point,Point2D &para);

	//compute the ubasis
	double ComputeUBasis(int i,int p,double u);

	//compute the u value
	double ComputeUBasis(int i,double u);

	//compute the v value
	double ComputeVBasis(int i,double v);

	//compute the v basis
	double ComputeVBasis(int i,int p,double v);

	//compute the u basis partial
	double ComputeUBasis_PartialU(int i,int p, double u);

	//compute the u basis  partial
	double ComputeUBasis_PartialU(int i,double u);

	//compute the v basis partial
	double ComputeVBasis_PartialV(int i,int p, double v);

	//compute the v basis  partial
	double ComputeVBasis_PartialV(int i,double v);

	//compute the u basis partial u square
	double ComputeUBasis_PartialU_2(int i, double u);

	//compute the v basis partial v square
	double ComputeVBasis_PartialV_2(int i, double v);



	//Read
	void Read(std::ifstream &file);

	//Save
	void Save(std::ofstream &file);
};

#endif