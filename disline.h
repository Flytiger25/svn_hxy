#ifndef _C_Curve3D
#define _C_Curve3D
#include "Point3D.h"
#include "Curve.h"
#include "lib3d\Object3d.h"
class  C_Entity2D : public CObject
{

public:
	C_Entity2D() {};
	
};

class C_Point2D : public C_Entity2D
{
private:
	Point2D m_point;
public:
	C_Point2D();
	C_Point2D(Point2D point);
	C_Point2D(C_Point2D &point);
	C_Point2D(C_Point2D *point);
	
	Point2D GetPoint();
	void    SetPoint(Point2D point);
	
	C_Point2D *Copy();
	
};

class C_Curve2D : public C_Entity2D
{

		
public:
	C_Curve2D() {};
	
	virtual Point2D GetStartPoint();
	virtual Point2D GetEndPoint();
	virtual Point2D GetMiddlePoint();
	virtual Point2D GetParaPoint(double t);
	virtual double GetPointPara(Point2D p);
	virtual BOOL PointOn(Point2D p, double toler);
	virtual C_Curve2D *Copy();
	virtual C_Curve2D *Reverse();
	
	virtual void SplitD(double t, C_Curve2D **pC1, C_Curve2D **pC2);
	virtual void SplitP(Point2D& p, C_Curve2D **pC1, C_Curve2D **pC2);
	virtual void SplitN(int n, double t[], Point2D p[], C_Curve2D *newcurve[]);
	
    virtual Vector2D GetParaTangent(double t);
    virtual void CalBox(double &umin, double &vmin, double &umax, double &vmax);

};
class C_Disline2D : public C_Curve2D
{


public:
  CObArray m_PointA;  // 存放指向C_Point2D的指针
public:
  C_Disline2D(void);
  C_Disline2D(int initnum);
  ~C_Disline2D(void);

  int  GetDispointNum();
  C_Point2D* GetDispoint(int index);
  void AddDispoint(C_Point2D *dispoint2d);

  void AllocDispoint(int number);
  void SetDispoint(int index, C_Point2D *dispoint2d);

  Point2D GetStartPoint();
  Point2D GetEndPoint();
  Point2D GetMiddlePoint();
  Point2D GetParaPoint(double t);
  double GetPointPara(Point2D p);
  BOOL PointOn(Point2D p, double toler);
  C_Curve2D *Copy();
  C_Curve2D *Reverse();

  void SplitD(double t, C_Curve2D **pC1, C_Curve2D **pC2);
  void SplitP(Point2D& p, C_Curve2D **pC1, C_Curve2D **pC2);
  void SplitN(int n, double t[], Point2D p[], C_Curve2D *newcurve[]);

  Vector2D GetParaTangent(double t);
  void CalBox(double &umin, double &vmin, double &umax, double &vmax);
};
BOOL SamePoint(const Point3D& p1, const Point3D& p2, double toler = TOLER);
BOOL SamePoint (const Point2D& p1, const Point2D& p2, double toler= TOLER);
int PointPointInters2d(Point2D,Point2D,double tol=TOLER);
int PointLineSegInters2d(Point2D pnt2d,CS_LineSeg2d lineseg2d,double tol=TOLER);
int LineSegLineSegInters2d(CS_LineSeg2d lineseg1,CS_LineSeg2d lineseg2,Point2D &pnt2d,CS_LineSeg2d  &lineseg2d ,double tol=TOLER);
int DislineDisline2DInters(C_Disline2D **disline,Point2D *pt);
#endif