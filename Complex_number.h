#pragma once
#include "math.h"
class Complex_number
{
public:
	double real,imaginary;
public:
	Complex_number(void);
	~Complex_number(void);
	Complex_number(double r,double i);
	double length();

	//
	void Set(double real,double imaginary)
	{
		this->real = real;
		this->imaginary = imaginary;
	}

	//the length
	double Mou()
	{
		return sqrt(SQR(real) + SQR(imaginary));
	}

	//zbar
	Complex_number Bar()
	{
		Complex_number v(real,-imaginary);
		return v;
	}

	//
	void operator = (Complex_number c)
	{
		this->Set(c.real,c.imaginary);
	}

	//
	Complex_number operator / (double c)
	{
		Complex_number c1;
		c1.Set(real / c,imaginary / c);
		return c1;
	}
};
Complex_number operator + (Complex_number& u,Complex_number& v);
Complex_number operator - (Complex_number& u,Complex_number& v);
Complex_number operator * (Complex_number& u,Complex_number& v);
Complex_number operator / (Complex_number& u,Complex_number& v);
double Length(Complex_number a);