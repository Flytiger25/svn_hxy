#include "stdafx.h"
#include "BilinearTransformation.h"
#include "math.h"

BilinearTransformation::BilinearTransformation(void)
{
	double m_Bilinear_Alpha1 = 0.5;
	double m_Bilinear_Alpha2 = 0.5;
	double m_Bilinear_Beta1 = 0.5;
	double m_Bilinear_Beta2 = 0.5;
}
BilinearTransformation::~BilinearTransformation(void)
{
}
int BilinearTransformation::Evaluation(double s,double t,double &u,double &v)
{
	double a1 = m_Bilinear_Alpha1;
	double a2 = m_Bilinear_Alpha2;
	double b1 = m_Bilinear_Beta1;
	double b2 = m_Bilinear_Beta2;

	double a = a1 * t + a2 * (1 - t);
	double b = b1 * s + b2 * (1 - s);

	u = (a - 1) * s / (2 * a * s - s - a);
	v = (b - 1) * t / (2 * b * t - t - b);
	return 1;
}
int BilinearTransformation::InverseEvaluation(double u,double v,double &s,double &t)
{
	double a1 = m_Bilinear_Alpha1;
	double a2 = m_Bilinear_Alpha2;
	double b1 = m_Bilinear_Beta1;
	double b2 = m_Bilinear_Beta2;

	double a = -u*a1*b1-2*u*a1*v+2*a2*v*b2-a2*v-2*a1*v*b2+a1*b2-a2*b2+u*a2*b2-a1+2*u*a1+2*u*a1*v*b2+a1*v+2*u*a2*v-2*u*a2*v*b2+2*u*a1*v*b1+u*a2*b1-u*a1*b2-2*u*a2*v*b1+a2-2*u*a2;
	double b = u*b2+1-u*a2*b1-u*a2*b2-2*u*a2*v+3*u*a2*v*b1-3*a2*v*b2+u*v+a2*v-u*a1*v*b1-a2-b2-u+2*u*a2+a1*v*b2-u*a1*v*b2-v+3*u*a2*v*b2-2*u*v*b2+2*v*b2+a2*b2;
	double c = -u*a2*v*b1+a2*v*b2-u*a2*v*b2+u*v*b2-v*b2;

	if(fabs(a) < 1e-5)
	{
		t = -c/b;
	}else
	{
		double t1 = (-b + sqrt(b * b - 4 * a * c))/(a*2);
		double t2 = (-b - sqrt(b * b - 4 * a * c))/(a*2);
		if(t1 >-1e-5 && t1 < 1+1e-5)
			t = t1;
		else if(t2 > 0 && t2 < 1)
		{
			t = t2;
		}else ASSERT(0);
	}

	s = u*(a1*t+a2-a2*t)/(-a1*t-a2+a2*t+1+2*u*a1*t+2*u*a2-2*u*a2*t-u);
	return 1;
}
Transformation* BilinearTransformation::copy()
{
	BilinearTransformation* pNewTrans = new BilinearTransformation();
	pNewTrans->m_ReparameterizationType = m_ReparameterizationType;
	pNewTrans->m_Bilinear_Alpha1 = m_Bilinear_Alpha1;
	pNewTrans->m_Bilinear_Alpha2 = m_Bilinear_Alpha2;
	pNewTrans->m_Bilinear_Beta1 = m_Bilinear_Beta1;
	pNewTrans->m_Bilinear_Beta2 = m_Bilinear_Beta2;
	pNewTrans->umax_Reparameterization = umax_Reparameterization;
	pNewTrans->umin_Reparameterization = umin_Reparameterization;
	pNewTrans->vmin_Reparameterization = vmin_Reparameterization;
	pNewTrans->vmax_Reparameterization = vmax_Reparameterization;
	return (Transformation*)pNewTrans;
}
double BilinearTransformation::Comput_u_s(double s,double t)
{
	double a1 = m_Bilinear_Alpha1;
	double a2 = m_Bilinear_Alpha2;
	double b1 = m_Bilinear_Beta1;
	double b2 = m_Bilinear_Beta2;
	double val = -(a1*t+a2-a2*t-1)*(a1*t+a2-a2*t)/SQR(2*s*a1*t+2*s*a2-2*s*a2*t-s-a1*t-a2+a2*t);
	return val;
}

double BilinearTransformation::Comput_u_t(double s,double t)
{
	double a1 = m_Bilinear_Alpha1;
	double a2 = m_Bilinear_Alpha2;
	double b1 = m_Bilinear_Beta1;
	double b2 = m_Bilinear_Beta2;
	double val = s*(s-1)*(a1-a2)/SQR(2*s*a1*t+2*s*a2-2*s*a2*t-s-a1*t-a2+a2*t);
	return val;
}

double BilinearTransformation::Comput_v_s(double s,double t)
{
	double a1 = m_Bilinear_Alpha1;
	double a2 = m_Bilinear_Alpha2;
	double b1 = m_Bilinear_Beta1;
	double b2 = m_Bilinear_Beta2;
	double val = t*(-1+t)*(b1-b2)/SQR(2*t*b1*s+2*t*b2-2*t*b2*s-t-b1*s-b2+b2*s);
	return val;
}

double BilinearTransformation::Comput_v_t(double s,double t)
{
	double a1 = m_Bilinear_Alpha1;
	double a2 = m_Bilinear_Alpha2;
	double b1 = m_Bilinear_Beta1;
	double b2 = m_Bilinear_Beta2;
	double val = -(b1*s+b2-b2*s-1)*(b1*s+b2-b2*s)/SQR(2*t*b1*s+2*t*b2-2*t*b2*s-t-b1*s-b2+b2*s);
	return val;
}
