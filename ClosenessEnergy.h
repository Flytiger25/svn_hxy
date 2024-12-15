#pragma once
#include "energy.h"

class ClosenessEnergy :	public Energy
{
public:
	ClosenessEnergy(real_1d_array x1);
	~ClosenessEnergy(void);

private:
	real_1d_array x;

public:
	/*3X3阶B-spline*/
	virtual void getBSplineGrad33(double &J,real_1d_array &grad);
	/*1X1阶*/
	virtual void getGrad11(double &J,real_1d_array &grad);
	/*2X2阶*/
	virtual void getGrad22(double &J,real_1d_array &grad);
	/*3X3阶*/
	virtual void getGrad33(double &J,std::vector<double> &grad,std::vector<double> &x);
	/*For surface 2X2*/
	static int getIndex22(int i,int j,int k);
	/*For surface 3X3*/
	static int getIndex33(int i,int j,int k);
	/*For B-Spline Surface 3X3*/
	static int getIndexBSpline33(int i,int j,int k);
	/*For B-Spline Surface 3X3,取得第（i,j）个Bezier内的（m,n）点在整个surface中的位置,Bezier是从0开始计数的*/
	static int getIndexForIJBezierMNInBSpline(int i,int j,int m,int n,int k);
private:
	//计算组合数
	int c(int i,int n);
	/*For surface 2X2，仅仅是求a=k*(pi1j1-pi2j2)*(pi3j3-pi4j4),这里是2*a'*/
	std::vector<double> getDerivative22(double k,const real_1d_array &x,int i1,int j1,int i2,int j2,int i3,int j3,int i4,int j4);
	/*For surface 3X3, 仅仅是求a=k*(pi1j1-pi2j2)*(pi3j3-pi4j4),这里是2*a'*/
	std::vector<double> getDerivative33(double k,std::vector<double> &x,int i1,int j1,int i2,int j2,int i3,int j3,int i4,int j4);

};
