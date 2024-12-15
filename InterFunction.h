#ifndef _InterFunc_3D_
#define _InterFunc_3D_
// MAF迭代方法中的曲线曲面延伸长度
#define  Extend_Length   0.15
struct CS_DisLine {
    int       np;       // number of intersection point
    Point3D   *p;       // 3D point coordinate of intersection
};
struct CS_FreeCurve {
    int       cn;       // number of control point
    int       order;    // order of the curve
    Point3D   *ctrl;    // control point array
    double    *weight;  // weight value array
    double    *knot;    // knot vector array
};
struct CS_IntCurve {
    int          type;  // INTER_POINT1P, ... , INTER_LINE, ...
    Point3D      pt[8], cp;
    Vector3D     vx, vy, vn;
    double       r, a, b, p;
    double       st, ed, z_min, z_max;
    CS_DisLine   disline;
    CS_FreeCurve curve;
};
// //////////////////////////////
// 有理Bezier曲线
// //////////////////////////////
struct CS_RBezierCurve2d {
    int n;
    Point2D *ctrl;
    double  *weight;
    CS_RBezierCurve2d()
    {
		ctrl = NULL;
		weight = NULL;
    }
//    ~CS_RBezierCurve2d()
//    {
//		if (ctrl != NULL)
//		{
//			delete []ctrl;
//			ctrl = NULL;
//		}
//		if (weight != NULL)
//		{
//			delete []weight;
//			weight = NULL;
//		}	
//    }	
} ;
// 自由曲线曲面的包围盒
struct CS_Box{
    Point3D  m_min,m_max;
};
// //////////////////////////
// 三维平面
// //////////////////////////
struct CS_Plane{
    Point3D   pa;           // pa 为平面上一个点
    Vector3D  vn;           // vn 为平面的法向
}; 
// //////////////////////////////
// 有理Bezier曲线
// //////////////////////////////
struct  CS_RBezierCurve {
    int n;
    Point3D *ctrl;
    double  *weight;
    CS_RBezierCurve(){;}
} ;
// 离散法求初始交点及MAF方法所用的特殊交点结构
// 线线求交
struct CS_CurvCurvIntPoint{
    Point3D p;           //  近似交点
    double  t1,t2;       //  交点在两条曲线上的参数
};
// //////////////////////////////
// NURBS曲线
// //////////////////////////////
enum CS_NurbsType {
		CSNT_UNKNOWN = 0, 
		CSNT_IsBSpline, CSNT_IsNURBS, 	// NURBS曲线曲面为B样条、NURBS
		CSNT_Close, CSNT_Open, 			// NURBS曲线曲面为开放、封闭
		CSNT_Revolut, CSNT_Extrude,		// 回转面、拉伸面
};
struct CS_NurbsCType {
	CS_NurbsType isBSpline;
	CS_NurbsType close;
};
struct CS_SubRBezierCurve {   // 用于求交算法的有理Bezier曲线段结构
    CS_RBezierCurve curve;    // 有理Bezier曲线段
    double ts, te;            // 曲线段起点和终点相对于原曲线的参数
    CS_Box box;               // 曲线段的包围盒
    double linear;			//线性指数
    int level;				//所在的分割层数
    struct CS_SubRBezierCurve *subCurve[2]; // 细分成的两个子曲线段
} ;
struct CS_NurbsCurve;
// 用于自由曲线求交库内部辅助表示
struct CS_NurbsCurveAuxRep {    // 组织Nurbs曲线分割树的头结点
    CS_NurbsCurve* curve;      // 所属Nurbs曲线
    CS_Box box;               // Nurbs曲线的包围盒
    int subNum;               // 分成的子曲线段的段数
    CS_SubRBezierCurve **subCurve;  // 每条子曲线段结点
} ;
struct CS_NurbsCurve {
    int n;
    int order;
    double *knot;
    Point3D *ctrl;
    double *weight;
		//--------------Revised by YangLei 060330----记录曲线求交库内部辅助表示-----
	CS_NurbsCurveAuxRep* auxRep;
	CS_NurbsCType type;
	//-----------------------------

	//---------Modified by Ruan Xiao Yu 05-09-30---记录曲线上离散的点信息-------
	int numb;
	double *t;
	Point3D *poin;
	//-----------------------------------
} ;
// Yang Lei 060330

// ///////////////////////////
//  三维直线
// ///////////////////////////
struct CS_Line{
    Point3D   pa;           // pa 为直线上的一个点
    Vector3D  vn;           // vn 为直线的方向
};

struct CS_BSplineCurve {
    int n;				   // n为顶点个数
    int order;			   // order为阶数
    double *knot;
    Point3D *ctrl;
} ;

class Line2D
{
private:
  Point2D m_start,m_end;
public:
  Line2D(void);
  Line2D(Point2D start, Point2D end);

  double distanceTo(Point2D pt);
};
struct  CS_GRBezierCurve {
    double start,end;
    CS_RBezierCurve rbezier;
} ;
struct CS_GRBezierCurve2d {
    double start,end;
    CS_RBezierCurve2d rbezier;
} ;
// 自由曲线曲面的包围盒
struct CS_Box2d{
    Point2D  m_min,m_max;
};
class Line3D
{
private:
  Point3D m_start,m_end;
public:
  Line3D(void);
  //Line3D(double u1,double v1,double u2,double v2);
  Line3D(Point3D start, Point3D end);

  double distanceTo(Point3D pt);
};

#define  INTER_POINT8P        8
#define  INTER_POINT1P        1
extern int LineNurbscInters(CS_Line line, CS_NurbsCurve curve,  CS_IntCurve *inp, int &numb);
extern int LineBezier4Inters(CS_Line line, CS_NurbsCurve curve,  CS_IntCurve *inp, int &numb);
extern double NurbscGetPointPara(CS_NurbsCurve curve, Point3D p);
extern int NurbscIntervalIndex(CS_NurbsCurve curve, double value);
extern void NurbscInsertMultiKnot(CS_NurbsCurve &curve, int multiplicity, double value);
extern int BscurvIntervalIndex(CS_BSplineCurve curve, double value);
extern double SPL_RbezcHeightEstimate(CS_RBezierCurve curve);
extern double SPL_RbezcHeightEstimate(CS_RBezierCurve2d curve);
extern int NurbscToRBezier(CS_NurbsCurve curve, CS_RBezierCurve bezier[], double *&t);
extern void SPL_GRbezcSubdivision(CS_GRBezierCurve2d curve, double Epsilon, Point2D *&vp, double *&t, int &number);
extern void SPL_GRbezcSubdivision(CS_GRBezierCurve curve, double Epsilon, Point3D *&vp, double *&t, int &number)	;
extern CS_Box SPL_GetCurvSurfBox3d(int number,Point3D points[]);
extern void SPL_GRBezcRecursive(CS_GRBezierCurve curve, double length, Point3D *&vp, double *&t, int &number) ;
extern void SPL_GRBezcSplit(CS_GRBezierCurve curve, double value, CS_GRBezierCurve &pc1, CS_GRBezierCurve &pc2);
extern void SPL_RbezcSplit(CS_RBezierCurve curve, double value, CS_RBezierCurve &pc1, CS_RBezierCurve &pc2);
extern bool CS_NurbsCIsBspl(CS_NurbsCurve& curv);
extern void PutNurbsCToBspl(const CS_NurbsCurve& ncurv, CS_BSplineCurve& bcurv);
extern void BscurvComputAll(CS_BSplineCurve curve, double value, Point3D &iterp, Vector3D &axis);
extern Point3D BscurvPosition(CS_BSplineCurve curve, double value);
extern Vector3D BscurvDerivative1(CS_BSplineCurve curve, double value);
extern Point3D NurbscPosition(CS_NurbsCurve curve, double value);
extern Point3D PointProjectToLine(Point3D point, Point3D origin, Vector3D xaxis) ;
#endif
