#pragma once
#include "energy.h"

class IsotropicEnergy :
	public Energy
{
public:
	IsotropicEnergy(const real_1d_array & x);
	~IsotropicEnergy(void);

private:
	real_1d_array x;

public:

	/*1X1論*/
	virtual void getGrad11(double &J,real_1d_array &grad);

	/*2X2論*/
	virtual void getGrad22(double &J,real_1d_array &grad);

	/*3X3論*/
	virtual void getGrad33(double &J,std::vector<double> &grad,std::vector<double> &x);

	/*For surface of degree m * n*/
	static int getIndex(int m, int n, int i, int j, int k);

	/*3X3論B-spline*/
	virtual void getBSplineGrad33(double &J,real_1d_array &grad);
};
