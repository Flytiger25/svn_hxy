// Point3D.h: interface for the Point3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT3D_H__C77F83A2_3EFB_4580_B188_5E8736F6F009__INCLUDED_)
#define AFX_POINT3D_H__C77F83A2_3EFB_4580_B188_5E8736F6F009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Vector2D;
class Vector3D;
class Vector4D;
class Point2D;
class Point3D;
class CVertex3d;
/*-------------------------------------------------------*/

class Vector2D
{
public:
	double	x, y;

public:
	//构造函数
	Vector2D()
	{
		x = y = 0;
	}
	Vector2D(double newx, double newy)
	{
		x = newx; y = newy;
	}

	//赋值操作符
	void operator += (const Vector2D& v)
	{
		x += v.x; y += v.y;
	}
	void operator -= (const Vector2D& v)
	{
		x -= v.x; y -= v.y;
	}
	void operator *= (double num)
	{
		x *= num; y *= num;
	}
	void operator /= (double num)
	{ //ASSERT (fabs (num) < TOLER);
		x /= num; y /= num;
	}

	//矢量加
	Vector2D operator + (const Vector2D& v) const
	{
		return Vector2D(x + v.x, y + v.y);
	}
	//矢量减
	Vector2D operator - (const Vector2D& v) const
	{
		return Vector2D(x - v.x, y - v.y);
	}
	//矢量乘(点积)
	double  operator * (const Vector2D& v) const
	{
		return x * v.x + y * v.y;
	}
	//数乘
	Vector2D operator * (double num) const
	{
		return Vector2D(x * num, y * num);
	}
	//数除
	Vector2D operator / (double num) const
	{ //ASSERT (fabs (num) < TOLER);
		return Vector2D(x / num, y / num);
	}

	//单目减
	Vector2D operator - () const
	{
		return Vector2D(-x, -y);
	}

	//判等(不等)
	int operator == (const Vector2D& v) const;
	int operator != (const Vector2D& v) const;

	//类型转换
	operator Point2D ();

	//取元素
	double& operator [] (int i)
	{
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		default: return x;
		}
	}

	//设为单位矢量
	void SetUnit();

	void SetValue(double xx, double yy)
	{
		x = xx;
		y = yy;
	}
};

/*-------------------------------------------------------*/

class Vector3D
{
public:
	double	x, y, z;

public:
	//构造函数
	Vector3D()
	{
		x = y = z = 0;
	}
	Vector3D(double newx, double newy, double newz)
	{
		x = newx; y = newy; z = newz;
	}

	//赋值操作符
	void operator += (const Vector3D& v)
	{
		x += v.x; y += v.y; z += v.z;
	}
	void operator -= (const Vector3D& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
	}
	void operator *= (double num)
	{
		x *= num; y *= num; z *= num;
	}
	void operator /= (double num)
	{ //ASSERT (fabs (num) < TOLER);
		x /= num; y /= num; z /= num;
	}
	void operator ^= (const Vector3D& v)
	{
		double a = y * v.z - z * v.y;
		double b = -x * v.z + z * v.x;
		double c = x * v.y - y * v.x;

		x = a;
		y = b;
		z = c;
	}

	//矢量加
	Vector3D operator + (const Vector3D& v) const
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}
	//矢量减
	Vector3D operator - (const Vector3D& v) const
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}
	//矢量乘(点积)
	double  operator * (const Vector3D& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	//矢量乘(叉积)
	Vector3D operator ^ (const Vector3D& v) const
	{
		return Vector3D(y * v.z - z * v.y,
			-x * v.z + z * v.x,
			x * v.y - y * v.x);
	}
	//数乘
	Vector3D operator * (double num) const
	{
		return Vector3D(x * num, y * num, z * num);
	}




	//数除
	Vector3D operator / (double num) const
	{ //ASSERT (fabs (num) < TOLER);
		return Vector3D(x / num, y / num, z / num);
	}
	//单目减
	Vector3D operator - () const
	{
		return Vector3D(-x, -y, -z);
	}

	//判等(不等)
	int operator == (const Vector3D& v) const;
	int operator != (const Vector3D& v) const;

	//类型转换
	operator Point3D ();

	//setvalue
	void SetValue(double x_, double y_, double z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	//取元素
	double& operator [] (int i)
	{
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return x;
		}
	}

	//设为单位矢量
	void SetUnit();
};

/*-------------------------------------------------------*/

class Point2D
{
public:
	double	x, y;

public:
	//构造函数
	Point2D()
	{
		x = y = 0;
	}
	Point2D(double newx, double newy)
	{
		x = newx; y = newy;
	}

	//赋值操作符
	void operator += (const Vector2D& v)
	{
		x += v.x; y += v.y;
	}
	void operator -= (const Vector2D& v)
	{
		x -= v.x; y -= v.y;
	}

	//点加向量
	Point2D operator + (const Vector2D& v) const;
	//点减向量
	Point2D operator - (const Vector2D& v) const;
	//两点相减
	Vector2D operator - (const Point2D& v) const;

	//单目减
	Point2D operator - () const;

	//类型转换
	operator Point3D ();

	//判等(不等)
	int operator == (const Point2D& v) const;
	int operator != (const Point2D& v) const;

	//取元素
	double& operator [] (int i)
	{
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		default: return x;
		}
	}
};

class Point3D
{
public:
	double	x, y, z;

public:
	//构造函数
	Point3D()
	{
		x = y = z = 0;
	}
	Point3D(double newx, double newy, double newz)
	{
		x = newx; y = newy; z = newz;
	}

	//赋值操作符
	void operator += (const Vector3D& v)
	{
		x += v.x; y += v.y; z += v.z;
	}
	//赋值操作符
	void operator = (const Point3D& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	void operator -= (const Vector3D& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
	}

	//数乘
	Point3D operator * (double num) const
	{
		return Point3D(x * num, y * num, z * num);
	}
	//数除
	Point3D operator / (double num) const
	{
		return Point3D(x / num, y / num, z / num);
	}

	//点加向量
	Point3D operator + (const Point3D& v) const;

	//点加向量
	Point3D operator + (const Vector3D& v) const;
	//点减向量
	Point3D operator - (const Vector3D& v) const;
	//两点相减
	Vector3D operator - (const Point3D& v) const;

	//单目减
	Point3D operator - () const;

	//判等(不等)
	int operator == (const Point3D& v) const;
	int operator != (const Point3D& v) const;

	//取元素
	double& operator [] (int i)
	{
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return x;
		}
	}
	//付值

	void setValue(double newx, double newy, double newz)
	{
		x = newx; y = newy; z = newz;
	}
};
class Vector4D
{
public:
	double	x, y, z, w;

public:
	//构造函数
	Vector4D()
	{
		x = y = z = w = 0;
	}
	Vector4D(double newx, double newy, double newz, double neww)
	{
		x = newx; y = newy; z = newz; w = neww;
	}
	Vector4D(Vector3D v, double neww)
	{
		x = v.x; y = v.y; z = v.z; w = neww;
	}
	//赋值操作符
	void operator += (const Vector4D& v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
	}
	void operator -= (const Vector4D& v)
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
	}
	void operator *= (double num)
	{
		x *= num; y *= num; z *= num; w *= num;
	}
	void operator /= (double num)
	{ //ASSERT (fabs (num) < TOLER);
		x /= num; y /= num; z /= num; w /= num;
	}


	//矢量加
	Vector4D operator + (const Vector4D& v) const
	{
		return Vector4D(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	//矢量减
	Vector4D operator - (const Vector4D& v) const
	{
		return Vector4D(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	//矢量乘(点积)
	double  operator * (const Vector4D& v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	//数乘
	Vector4D operator * (double num) const
	{
		return Vector4D(x * num, y * num, z * num, w * num);
	}
	//数除
	Vector4D operator / (double num) const
	{ //ASSERT (fabs (num) < TOLER);
		return Vector4D(x / num, y / num, z / num, w / num);
	}
	//单目减
	Vector4D operator - () const
	{
		return Vector4D(-x, -y, -z, -w);
	}


	void setValue(Vector3D v, double neww)
	{
		x = v.x * neww; y = v.y * neww; z = v.z * neww; w = neww;
	}
	void setValue(double newx, double newy, double newz, double neww)
	{
		x = newx; y = newy; z = newz; w = neww;
	}
	void SetUnit();
	//类型转换
	//取元素
	double& operator [] (int i)
	{
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: return x;
		}
	}


};
// ///////////////////////////
// 三维线段
// ///////////////////////////
struct CS_LineSeg {
	Point3D pa, pb;          // pa,pb: 线段的两端点
};
struct CS_LineSeg2d {
	Point2D pa, pb;          // pa,pb: 线段的两端点
};
struct CS_Line2d {
	Point2D   pa;           // pa 为直线上的一个点
	Vector2D  vn;           // vn 为直线的方向
};

double Length(const Vector2D& v);
double Length(const Vector3D& v);
double Length(Vector4D v);
Point2D MidPoint(const Point2D& p1, const Point2D& p2);
Point3D MidPoint(const Point3D& p1, const Point3D& p2);
double Distance(const Point2D& p1, const Point2D& p2);
double Distance(const Point3D& p1, const Point3D& p2);
double Distance(CVertex3d* pVertex1, Point3D p2);
Point2D Min(const Point2D& p1, const Point2D& p2);


Point3D Min(const Point3D& p1, const Point3D& p2);

Point2D Max(const Point2D& p1, const Point2D& p2);

Point3D Max(const Point3D& p1, const Point3D& p2);
inline int     MAX(int v1, int v2) {
	return v1 > v2 ? v1 : v2;
}

#endif // !defined(AFX_POINT3D_H__C77F83A2_3EFB_4580_B188_5E8736F6F009__INCLUDED_)
