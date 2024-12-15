// Point3D.cpp: implementation of the Point3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "Point3D.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vector2D Point2D::operator - (const Point2D& v) const
{
	return Vector2D(x - v.x, y - v.y);
}

Point2D Point2D::operator - () const
{
	return Point2D(-x, -y);
}

Point2D Point2D::operator + (const Vector2D& v) const
{
	return Point2D(x + v.x, y + v.y);
}

Point2D Point2D::operator - (const Vector2D& v) const
{
	return Point2D(x - v.x, y - v.y);
}

double Length(const Vector2D& v)
{
	return sqrt(v * v);
}

double Length(const Vector3D& v)
{
	return sqrt(v * v);
}
double Length(Vector4D v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}

double Distance(const Point2D& p1, const Point2D& p2)
{
	return Length(p1 - p2);
}

double Distance(const Point3D& p1, const Point3D& p2)
{
	return Length(p1 - p2);
}


int Point2D::operator == (const Point2D& v) const
{
	return Distance(*this, v) <= TOLER;
}

int Point2D::operator != (const Point2D& v) const
{
	return !(operator == (v));
}


Point2D::operator Point3D ()
{
	return Point3D(x, y, 0);
}
/*-------------------------------------------------------*/

Vector3D Point3D::operator - (const Point3D& v) const
{
	return Vector3D(x - v.x, y - v.y, z - v.z);
}

Point3D Point3D::operator - () const
{
	return Point3D(-x, -y, -z);
}

Point3D Point3D::operator + (const Vector3D& v) const
{
	return Point3D(x + v.x, y + v.y, z + v.z);
}
Point3D Point3D::operator + (const Point3D& v) const
{
	return Point3D(x + v.x, y + v.y, z + v.z);
}
Point3D Point3D::operator - (const Vector3D& v) const
{
	return Point3D(x - v.x, y - v.y, z - v.z);
}

int Point3D::operator == (const Point3D& v) const
{
	return Distance(*this, v) <= TOLER;
}

int Point3D::operator != (const Point3D& v) const
{
	return !(operator == (v));
}

/*-------------------------------------------------------*/

Vector2D::operator Point2D ()
{
	return Point2D(x, y);
}

void Vector2D::SetUnit()
{
	double a = Length(*this); // * (1 + 1e-15);
	if (a > TOLER) (*this) /= a;
}

int Vector2D::operator == (const Vector2D& v) const
{
	Vector2D a(*this), b(v);

	a.SetUnit();
	b.SetUnit();

	return Length(a - b) <= TOLER;
}

int Vector2D::operator != (const Vector2D& v) const
{
	return !(operator == (v));
}

/*-------------------------------------------------------*/

Vector3D::operator Point3D ()
{
	return Point3D(x, y, z);
}

void Vector3D::SetUnit()
{
	double a = Length(*this); // * (1 + 1e-15);
	if (a > TOLER) (*this) /= a;
}

int Vector3D::operator == (const Vector3D& v) const
{
	Vector3D a(*this), b(v);

	a.SetUnit();
	b.SetUnit();

	return Length(a - b) <= TOLER;
}

int Vector3D::operator != (const Vector3D& v) const
{
	return !(operator == (v));
}
/*-------------------------------------------------------*/



Point2D MidPoint(const Point2D& p1, const Point2D& p2)
{
	return Point2D((p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5);
}

Point3D MidPoint(const Point3D& p1, const Point3D& p2)
{
	return Point3D((p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5, (p1.z + p2.z) * 0.5);
}

Point2D Min(const Point2D& p1, const Point2D& p2)
{
	return Point2D(min(p1.x, p2.x), min(p1.y, p2.y));
}

Point3D Min(const Point3D& p1, const Point3D& p2)
{
	return Point3D(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
}

Point2D Max(const Point2D& p1, const Point2D& p2)
{
	return Point2D(max(p1.x, p2.x), max(p1.y, p2.y));
}

Point3D Max(const Point3D& p1, const Point3D& p2)
{
	return Point3D(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
}




BOOL SamePoint(const Point2D& p1, const Point2D& p2, double toler)
{
	return Distance(p1, p2) < toler;
}

BOOL SamePoint(const Point3D& p1, const Point3D& p2, double toler)
{
	return Distance(p1, p2) < toler;
}

void SwapPoint(Point2D& p1, Point2D& p2)
{
	Point2D p0 = p1; p1 = p2; p2 = p0;
}

void SwapPoint(Point3D& p1, Point3D& p2)
{
	Point3D p0 = p1; p1 = p2; p2 = p0;
}

void SwapVector(Vector2D& p1, Vector2D& p2)
{
	Vector2D p0 = p1; p1 = p2; p2 = p0;
}

void SwapVector(Vector3D& p1, Vector3D& p2)
{
	Vector3D p0 = p1; p1 = p2; p2 = p0;
}

//---------------------------------------------------------
// I/O functions
//---------------------------------------------------------

CArchive& operator << (CArchive& ar, const Point2D& aPoint)
{
	ar << aPoint.x << aPoint.y;
	return ar;
}

CArchive& operator >> (CArchive& ar, Point2D& aPoint)
{
	ar >> aPoint.x >> aPoint.y;
	return ar;
}

CArchive& operator << (CArchive& ar, const Point3D& aPoint)
{
	ar << aPoint.x << aPoint.y << aPoint.z;
	return ar;
}

CArchive& operator >> (CArchive& ar, Point3D& aPoint)
{
	ar >> aPoint.x >> aPoint.y >> aPoint.z;
	return ar;
}

CArchive& operator << (CArchive& ar, const Vector2D& aVector)
{
	ar << aVector.x << aVector.y;
	return ar;
}

CArchive& operator >> (CArchive& ar, Vector2D& aVector)
{
	ar >> aVector.x >> aVector.y;
	return ar;
}

CArchive& operator << (CArchive& ar, const Vector3D& aVector)
{
	ar << aVector.x << aVector.y << aVector.z;
	return ar;
}

CArchive& operator >> (CArchive& ar, Vector3D& aVector)
{
	ar >> aVector.x >> aVector.y >> aVector.z;
	return ar;
}
