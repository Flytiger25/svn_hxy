#pragma once
#include "stdafx.h"
class Transformation
{
public:

	//the reparameterized parameter domain
	double vmin_Reparameterization,vmax_Reparameterization;
	double umin_Reparameterization,umax_Reparameterization;

	//the original parameter domain
	double umin,umax,vmin,vmax;

	reparatype m_ReparameterizationType;
	//evaluation
	virtual int Evaluation(double s,double t,double &u,double &v) = 0;

	//inverse evaluation
	virtual int InverseEvaluation(double u,double v,double &s,double &t) = 0;

	virtual void Read(std::ifstream &file);

	virtual void Save(std::ofstream &file);

	virtual Transformation* copy() = 0;

	virtual double Comput_u_s(double s,double t)
	{
		return 0;
	}

	virtual double Comput_u_t(double s,double t)
	{
		return 0;
	}

	virtual double Comput_v_s(double s,double t)
	{
		return 0;
	}

	virtual double Comput_v_t(double s,double t)
	{
		return 0;
	}
	Transformation(void);
	~Transformation(void);
};
