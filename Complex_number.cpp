#include "stdafx.h"
#include "math.h"
#include "Complex_number.h"

Complex_number::Complex_number(void)
{
}

Complex_number::~Complex_number(void)
{
}
Complex_number::Complex_number(double r,double i)
{
	real = r;
	imaginary = i;
}
Complex_number operator + (Complex_number& u,Complex_number& v)
{
	Complex_number result;
	result.real = u.real + v.real;
	result.imaginary = u.imaginary + v.imaginary;
	return result;
}
Complex_number operator - (Complex_number& u,Complex_number& v)
{
	Complex_number result;
	result.real = u.real - v.real;
	result.imaginary = u.imaginary - v.imaginary;
	return result;
}
Complex_number operator * (Complex_number& u,Complex_number& v)
{
	Complex_number result;
	result.real = u.real * v.real - u.imaginary * v.imaginary;
	result.imaginary = u.imaginary * v.real + u.real * v.imaginary;
	return result;
}
Complex_number operator / (Complex_number& u,Complex_number& v)
{
	//not implemented.
	Complex_number num(0,0);
	return num;
}
double Complex_number::length()
{
	return sqrt(SQR(real) + SQR(imaginary));
}
double Length(Complex_number a)
{
	return a.length();
}