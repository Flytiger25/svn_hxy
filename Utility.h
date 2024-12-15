#pragma once
class C_NurbsCurve3D;
static class Utility
{
public:
	Utility(void);
	~Utility(void);

	static double ComputeBasis(int i,int p, double u,const std::vector<double> &m_knotArray);

	static C_NurbsCurve3D* FittingBsplineCurve_Constrained_Weighted(const std::vector<double> &m_knotVector,const std::vector<Point3D> &m_PointArray,const std::vector<double> &m_ParaArray,
		const std::vector<double> &m_WeightArray,const std::vector<double> &m_ConstrainedParaArray,const std::vector<Point3D> &m_ConstrainedPointArray);

	static C_NurbsCurve3D* FittingBsplineCurve(const std::vector<double> &m_knotVector,const std::vector<Point3D> &m_PointArray,const std::vector<double> &m_ParaArray);
};
