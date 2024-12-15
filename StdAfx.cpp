// stdafx.cpp : source file that includes just the standard includes
//	SDIViewSwitch.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

int m_SetCameraMode;
double max(double a,double b)
{ return (((a) > (b)) ? (a) : (b));}
double min(double a,double b)
{ return (((a) < (b)) ? (a) : (b));}
double SQR(double x)
{
	return x * x;
}


double Factorial(double n, double i)
{
	int j;
	double numerator=1;
	double denominator=1;
	for(j=0;j<i;j++)
	{
		numerator=numerator*(n-j);
		denominator=denominator*(j+1);
	}
	return numerator/denominator;
}

void MatrixPointMultiple(double &x,double &y, double &z,GLdouble *mat)
{
	//double x1 = 
}
std::vector<double> operator + (const std::vector<double> &v1,const std::vector<double> &v2)
{

	std::vector<double> result;

	for(int i=0;i<v1.size();i++)
		result.push_back(v1[i]+v2[i]);

	return result;

}
std::vector<double> &operator += (std::vector<double> &v1,const std::vector<double> &v2)
{
	v1 = v1 + v2;
	return v1;
}
std::vector<double> operator * (const std::vector<double> &v1,double d)
{
	return d*v1;
}
std::vector<double> operator / (const std::vector<double> &v1,double d)
{
	return v1 * (1 / d);
}
std::vector<double> operator * (double d,const std::vector<double> &v1)
{
	std::vector<double> result;

	for(int i=0;i<v1.size();i++)
		result.push_back(v1[i]*d);

	return result;
}

Vector3D operator * (double num, Vector3D v) 
{
	return Vector3D(num*v.x,num*v.y,num*v.z);
}