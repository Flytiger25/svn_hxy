#include "stdafx.h"
#include "Compatible.h"

//Include File
//File 
#include <STEPControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <StepRepr_Representation.hxx>
#include <Standard_Type.hxx>
//basic geom
#include <Geom_Surface.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Pln.hxx>
#include <gp_Circ.hxx>
#include <gp_Ax2.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include "GeomAPI_PointsToBSpline.hxx"
#include <GeomAbs_SurfaceType.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GeomConvert.hxx>
#include <GeomLProp_CLProps.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <GCPnts_QuasiUniformAbscissa.hxx>
#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <GeomAPI_IntCS.hxx>
#include <GeomFill_ConstrainedFilling.hxx>
#include <Geom_Curve.hxx>
#include <Geom_BezierCurve.hxx>
#include <GeomFill_BSplineCurves.hxx>
#include <GeomFill_ConstrainedFilling.hxx>
#include <GeomFill_Boundary.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GeomFill_Filling.hxx>
#include <GeomFill_Coons.hxx>
#include <GeomFill_SimpleBound.hxx>
#include <ProjLib_ProjectedCurve.hxx>
#include <GeomConvert_CompCurveToBSplineCurve.hxx>
#include <ProjLib_CompProjectedCurve.hxx>
#include <Adaptor3d_CurveOnSurface.hxx>
#include <GeomFill_BoundWithSurf.hxx>
#include <Geom2dAdaptor_Curve.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <math_Matrix.hxx>
#include <Interface_Static.hxx>
#include <TopExp_Explorer.hxx>
#include <TopExp.hxx>
//Topo 
#include <TopoDS.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <Interface_EntityIterator.hxx>
#include <TCollection_AsciiString.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <BRep_Tool.hxx>
#include <Adaptor3d_Surface.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <BRepFill_Filling.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Curve2d.hxx>
#include <BRepProj_Projection.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>

//3rd part
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <Eigen/Dense>
#include <map>
#include <cmath>
#include <memory>
#include <algorithm>
#include <functional>
#include <numeric> 
#include <filesystem>
#include <iomanip>
//#include "engine.h"
//#include "matrix.h"
#include <functional>
#include <regex>
#include <Interpolate.h>

//Function
void Compatible::UniformSamplingOnBspline(const Handle(Geom_BSplineCurve)& BsplineCurve, int numSamples, std::vector<gp_Pnt>& points) {
	GeomAdaptor_Curve adaptor(BsplineCurve);
	double totalLength = GCPnts_AbscissaPoint::Length(adaptor);
	Standard_Real desiredLength = totalLength / numSamples;
	GCPnts_UniformAbscissa  uniformAbscissa(adaptor, desiredLength);
	if (uniformAbscissa.IsDone()) {
		for (int i = 1; i <= numSamples + 1; ++i) {
			gp_Pnt point = adaptor.Value(uniformAbscissa.Parameter(i));
			points.push_back(point);
		}
	}
	else {
		std::cout << "Failed to sample the curve." << std::endl;
	}
}

std::vector<double> Compatible::ComputeUniformParam(int numSamples, double left, double right) {
	std::vector<double> parameters;
	if (numSamples == 0) {
		return parameters;
	}
	for (int i = 1; i <= numSamples; i++) {
		Standard_Real param = left + (right - left) * (i - 1) / (numSamples - 1);
		parameters.push_back(param);
	}
	return parameters;
}

template <typename T>
std::vector<std::vector<T>> Compatible::transpose(const std::vector<std::vector<T>>& matrix) {
	if (matrix.empty()) return {};

	size_t rows = matrix.size();
	size_t cols = matrix[0].size();

	std::vector<std::vector<T>> transposed(cols, std::vector<T>(rows));

	for (size_t i = 0; i < rows; ++i) {
		if (matrix[i].size() != cols) {
			throw std::invalid_argument("所有行必须具有相同的列数。");
		}
		for (size_t j = 0; j < cols; ++j) {
			transposed[j][i] = matrix[i][j];
		}
	}

	return transposed;
}

Handle(Geom_BSplineCurve) Compatible::ApproximateC(const std::vector<gp_Pnt>& Pnts, std::vector<double>& params, std::vector<double>& FKnots, int degree) {
	std::vector<double> Knots;
	std::vector<int> Mutis;
	InterPolateTool_1::sequenceToKnots(FKnots, Knots, Mutis);
	TColStd_Array1OfReal Knots_OCC(1, Knots.size());
	TColStd_Array1OfInteger Mutis_OCC(1, Mutis.size());
	for (size_t i = 0; i < Knots.size(); i++) {
		Knots_OCC[i + 1] = Knots[i];
		Mutis_OCC[i + 1] = Mutis[i];
	}
	return ApproximateC(Pnts, params, Knots_OCC, Mutis_OCC, FKnots, degree);
}

gp_Vec Compatible::CalResPnt(int k, const std::vector<gp_Pnt>& dataPoints, const std::vector<double>& parameters, Standard_Integer p,
	std::vector<double>& Knots, int CtrlPntNum) {
	Standard_Real aCoeff1 = InterPolateTool_1::OneBasicFun(parameters[k], 0, p, Knots);
	Standard_Real aCoeff2 = InterPolateTool_1::OneBasicFun(parameters[k], CtrlPntNum, p, Knots);
	gp_Vec vecTemp0(dataPoints[0].Coord());
	gp_Vec vecTempm(dataPoints[dataPoints.size() - 1].Coord());
	gp_Vec vecTempk(dataPoints[k].Coord());
	gp_Vec vectemp = vecTempk - aCoeff1 * vecTemp0 - aCoeff2 * vecTempm;
	return vectemp;
}

Handle(Geom_BSplineCurve) Compatible::ApproximateC(const std::vector<gp_Pnt>& Pnts, std::vector<double>& PntParams, TColStd_Array1OfReal& Knots, TColStd_Array1OfInteger& Mutis, std::vector<double>& FKnots,
	int degree) {
	int n = FKnots.size() - degree - 2;
	int m = Pnts.size() - 1;

	//1.Construct matrix N
	Eigen::MatrixXd matN(m - 1, n - 1);
	for (size_t i = 0; i < m - 1; i++) {
		for (size_t j = 0; j < n - 1; j++) {
			double value = InterPolateTool_1::OneBasicFun(PntParams[i + 1], j + 1, degree, FKnots);
			matN(i, j) = value;
		}
	}
	Eigen::MatrixXd matTransposeN = matN.transpose();
	Eigen::MatrixXd NTN = matTransposeN * matN;
	//2.Construct matrix R
	//2.1 Compute Ri(2-(m-1))
	Eigen::VectorXd VRx(m - 1);
	Eigen::VectorXd VRy(m - 1);
	Eigen::VectorXd VRz(m - 1);
	for (size_t i = 1; i <= m - 1; i++) {
		gp_Vec VecTemp = CalResPnt(i, Pnts, PntParams, degree, FKnots, n);
		double x, y, z;
		VecTemp.Coord(x, y, z);
		VRx(i - 1) = x;
		VRy(i - 1) = y;
		VRz(i - 1) = z;
	}
	//2.2 Compute the component of R
	Eigen::VectorXd Rx = matTransposeN * VRx;
	Eigen::VectorXd Ry = matTransposeN * VRy;
	Eigen::VectorXd Rz = matTransposeN * VRz;

	//3.solve NtN P = R
	Eigen::VectorXd Sx = NTN.lu().solve(Rx);
	Eigen::VectorXd Sy = NTN.lu().solve(Ry);
	Eigen::VectorXd Sz = NTN.lu().solve(Rz);

	//4.construct bspline curve
	TColgp_Array1OfPnt ctrlPnts(1, n + 1);
	ctrlPnts[1] = Pnts[0];
	ctrlPnts[n + 1] = Pnts[m];
	for (size_t i = 2; i <= n; i++) {
		gp_Pnt pntTemp(Sx(i - 2), Sy(i - 2), Sz(i - 2));
		ctrlPnts[i] = pntTemp;
	}
	Handle(Geom_BSplineCurve) bspline = new Geom_BSplineCurve(ctrlPnts, Knots, Mutis, degree);
	return bspline;
}

double Compatible::SingleParamError(const Handle(Geom_BSplineCurve)& curve, double param, gp_Pnt point) {
	auto point_on_curve = curve->Value(param);
	return point.Distance(point_on_curve);
}

double Compatible::SingleParamError(const std::vector<Handle(Geom_BSplineCurve)>& curves, double param, std::vector<gp_Pnt> points) {
	double error = 0.;
	if (curves.size() != points.size()) {
		std::cerr << "curves num must equal to points num!" << std::endl;
	}
	for (int i = 0; i < curves.size(); i++) {
		error += SingleParamError(curves[i], param, points[i]);
	}
	return error;
}

double Compatible::MaxErrorOnCurve(const std::vector<gp_Pnt>& points, std::vector<double>& params, Handle(Geom_BSplineCurve)& BsplineCurve) {
	if (points.size() != params.size()) {
		std::cout << "SIZE NOT EQUAL :IN CXFUNC!" << std::endl;
		return -1;
	}
	double max_error = 0;
	for (int i = 0; i < params.size(); i++) {
		gp_Pnt pnt = BsplineCurve->Value(params[i]);
		double disTemp = pnt.Distance(points[i]);
		if (disTemp > max_error) {
			max_error = disTemp;
		}
	}
	return max_error;
}

std::vector<double> Compatible::ComputeNewKnot(std::vector<double>& InsertKnots, std::vector<double>& OldKnots, std::vector<double>& DisCovParams) {
	std::vector<double> NewKnots;
	int i = 0, j = 0;
	int SizeTemp = OldKnots.size() - 2;
	int DisCovParamsSize = DisCovParams.size();
	while (i < OldKnots.size() && j < DisCovParamsSize) {
		if (OldKnots[i] <= DisCovParams[j]) {
			NewKnots.push_back(OldKnots[i]);
			if (DisCovParams[j] < OldKnots[i + 1] || (i == SizeTemp && DisCovParams[j] == OldKnots[i + 1])) {
				double KnotTemp = (OldKnots[i] + OldKnots[i + 1]) / 2;
				NewKnots.push_back(KnotTemp);
				InsertKnots.push_back(KnotTemp);
				j++;
				if (j >= DisCovParamsSize) {
					++i;
					goto label;
				}
				while (DisCovParams[j] < OldKnots[i + 1] || (i == SizeTemp && DisCovParams[j] == OldKnots[i + 1])) {
					j++;
					if (j >= DisCovParamsSize) {
						++i;
						goto label;
					}
				}
			}
			++i;
		}
	}
label:
	while (i < OldKnots.size()) {
		NewKnots.push_back(OldKnots[i]);
		++i;
	}
	return NewKnots;
}

std::vector<Handle(Geom_BSplineCurve)> Compatible::IterateApproximate(std::vector<double>& errors, std::vector<double>& InsertKnots, const std::vector<std::vector<gp_Pnt>>& Pnts,
	std::vector<double>& Params, std::vector<double>& InitKnots,
	int degree, int MaxIterNum, double toler) {
	int current_iterate_num = 1;
	double current_max_error = 0.;
	double current_max_param = 0.;
	std::vector<Handle(Geom_BSplineCurve)> IterBspineCurves(Pnts.size());
	std::vector<double> CurrentKnots = InitKnots;
	auto trans_pnts = transpose<gp_Pnt>(Pnts);
	do {
		current_max_error = 0.;
		for (int i = 0; i < Pnts.size(); i++) {
			IterBspineCurves[i] = ApproximateC(Pnts[i], Params, CurrentKnots, degree);
		}
		//find max error param and max error
		for (int i = 0; i < Params.size(); i++) {
			auto error = SingleParamError(IterBspineCurves, Params[i], trans_pnts[i]);
			if (error > (current_max_error * IterBspineCurves.size())) {
				current_max_error = error / IterBspineCurves.size();
				current_max_param = Params[i];
			}
		}
		std::vector<double> current_max_params;
		current_max_params.push_back(current_max_param);
		std::cout << "Iteration Num = " << current_iterate_num << " The Current Error = " << current_max_error << " The Target Error = " << toler << std::endl;
		if (current_max_error > toler) {
			//update knot vector
			CurrentKnots = ComputeNewKnot(InsertKnots, CurrentKnots, current_max_params);
		}
		else {
			std::cout << "----------------End----------------" << std::endl;
			std::cout << "Find Curve,iteration ending--------------the current error is " << current_max_error << std::endl;
			std::cout << "----------------End Iteration----------------" << std::endl << std::endl;
			for (int i = 0; i < IterBspineCurves.size(); i++) {
				errors.push_back(MaxErrorOnCurve(Pnts[i], Params, IterBspineCurves[i]));
			}
			return IterBspineCurves;
		}
		current_iterate_num++;
	} while (current_max_error > toler && current_iterate_num <= MaxIterNum);
	//set check params
	std::cout << "----------------End----------------" << std::endl;
	std::cout << "Arrive max iteration number, iteration ending--------------the current error is " << current_max_error << std::endl;
	std::cout << "----------------End Iteration----------------" << std::endl << std::endl;

	//return bspline;
	for (int i = 0; i < IterBspineCurves.size(); i++) {
		errors.push_back(MaxErrorOnCurve(Pnts[i], Params, IterBspineCurves[i]));
	}
	return IterBspineCurves;
}

Handle(Geom_BSplineCurve) Compatible::IterateApproximate(std::vector<double>& InsertKnots, const std::vector<gp_Pnt>& Pnts, std::vector<double>& PntsParams, std::vector<double>& InitKnots, int degree, int MaxIterNum, double toler)
{
	int itNum = 1;
	double currentMaxError = 100;
	Handle(Geom_BSplineCurve) IterBspineCurve;
	std::vector<double> CurrentKnots = InitKnots;
#ifdef PRINT_FULL_INFO
	std::cout << std::endl;
	std::cout << "----------------Strat Iteration----------------" << std::endl;
	std::cout << "----------------Iteration Infor----------------" << std::endl;
	std::cout << "Max Iteration Num = " << MaxIterNum << " " << std::endl;
	std::cout << "Iteration degree = " << degree << " " << std::endl;
	std::cout << "Iteration toler = " << toler << " " << std::endl;
	std::cout << "----------------Strat----------------" << std::endl;
#endif // PRINT_FULL_INFO
	while (currentMaxError > toler && itNum <= MaxIterNum)
	{
		auto start = std::chrono::high_resolution_clock::now();

		IterBspineCurve = ApproximateC(Pnts, PntsParams, CurrentKnots, degree);

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> duration = end - start;

		cxBasicClass::KnotUpdate knotUpdate(IterBspineCurve, CurrentKnots, Pnts, PntsParams);
		auto newKnot = knotUpdate.SelfSingleUpdate(cxBasicEnum::PARAM_BASED_BY_INTERVAL_ERROR);
		currentMaxError = knotUpdate.getMaxError();
#ifdef PRINT_FULL_INFO
		std::cout << "Iteration Num = " << itNum << " The Current Error = " << currentMaxError << " The Target Error = " << toler << " The Time = " << duration.count() << " ms " << std::endl;
#endif // PRINT_FULL_INFO
		if (currentMaxError > toler)
		{
			//update knot vector
			InsertKnots.push_back(newKnot);
			CurrentKnots = knotUpdate.getSequences();
}
		else
		{
#ifdef PRINT_FULL_INFO
			std::cout << "----------------End----------------" << std::endl;
			std::cout << "Find Curve,iteration ending--------------the current error is " << currentMaxError << std::endl;
			std::cout << "----------------End Iteration----------------" << std::endl << std::endl;
#endif // PRINT_FULL_INFO	  

			return IterBspineCurve;
		}
		itNum++;
		}
	//set check params
#ifdef PRINT_FULL_INFO
	std::cout << "----------------End----------------" << std::endl;
	std::cout << "Arrive max iteration number, iteration ending--------------the current error is " << currentMaxError << std::endl;
	std::cout << "----------------End Iteration----------------" << std::endl << std::endl;
#endif // PRINT_FULL_INFO

	//return bspline;
	return IterBspineCurve;
}

std::vector<double> Compatible::KnotGernerationByParams(const std::vector<double>& params, int n, int p)
{
	int m = params.size() - 1;
	double d = (m + 1) / (n - p + 1);
	std::vector<double> Knots(n + p + 2);
	int temp;
	double alpha;
	for (size_t i = 0; i <= p; i++)
	{
		Knots[i] = 0.0;
	}
	for (size_t j = 1; j <= n - p; j++)
	{
		temp = int(j * d);
		alpha = j * d - temp;
		Knots[p + j] = (1 - alpha) * params[temp - 1] + alpha * params[temp];
	}
	for (size_t i = n + 1; i <= n + p + 1; i++)
	{
		Knots[i] = 1;
	}
	return Knots;
}


//std::vector<double> Compatible::ApproximateCompatible(std::vector<Handle(Geom_BSplineCurve)>& Curves, std::vector<Handle(Geom_BSplineCurve)>& compatibleCurves, double toler) {
//	if (Curves.size() <= 1) {
//		return std::vector<double>{0};
//	}
//	//arc sampling
//	int sampling_num = 50;
//	std::vector<std::vector<gp_Pnt>> sampling_points_matrix(Curves.size(), std::vector<gp_Pnt>());
//	for (int i = 0; i < Curves.size(); i++) {
//		UniformSamplingOnBspline(Curves[i], sampling_num, sampling_points_matrix[i]);//等弧长采样
//	}
//	//paramaterization
//	auto params = ComputeUniformParam(sampling_num + 1);//参数化，因为是等弧长采样、此处为了使得最终曲线为弧长参数化，所以此处为均匀参数化
//	//approximete
//	int appro_degree = Curves[0]->Degree();
//	for (int i = 1; i < Curves.size(); i++) {
//		appro_degree = appro_degree > Curves[i]->Degree() ? appro_degree : Curves[i]->Degree();
//	}
//	std::vector<double> insert_knots;
//	std::vector<double> init_knots;
//	for (int i = 0; i <= appro_degree; i++) {
//		init_knots.push_back(0.);
//	}
//	for (int i = 0; i <= appro_degree; i++) {
//		init_knots.push_back(1.);
//	}
//	std::vector<double> errors;
//	auto comp_curves = IterateApproximate(errors, insert_knots, sampling_points_matrix, params, init_knots, appro_degree, 100, 0.01);
//	compatibleCurves.clear();
//	for (const auto& cur : comp_curves) {
//		compatibleCurves.push_back(cur);
//	}
//	return errors;
//}

bool Compatible::isDegenerate(const Handle(Geom_BSplineCurve)& curve) {
	const TColgp_Array1OfPnt& poles = curve->Poles();
	gp_Pnt firstPole = poles(poles.Lower());
	for (Standard_Integer i = poles.Lower() + 1; i <= poles.Upper(); ++i) {
		if (!poles(i).IsEqual(firstPole, 0.1)) {
			return false;
		}
	}
	return true;
}

/// <summary>
/// compatible操作，不是精确compatible 而是拟合相容
/// </summary>
/// <param name="Curves">输入曲线</param>
/// <param name="toler">容差</param>
/// <returns>每条曲线误差</returns>
std::vector<double> Compatible::ApproximateCompatible(std::vector<Handle(Geom_BSplineCurve)>& Curves, std::vector<Handle(Geom_BSplineCurve)>& compatibleCurves, double toler) {
	if (Curves.size() <= 1) {
		return std::vector<double>{0};
	}

	bool firstDegenerate = false;
	bool lastDegenerate = false;
	std::vector<Handle(Geom_BSplineCurve)> middleCurves;

	for (size_t i = 0; i < Curves.size(); ++i) {
		if (i == 0) {
			if (isDegenerate(Curves[i])) {
				firstDegenerate = true;
			}
			else {
				middleCurves.push_back(Curves[i]);
			}
		}
		else if (i == Curves.size() - 1) {
			if (isDegenerate(Curves[i])) {
				lastDegenerate = true;
			}
			else {
				middleCurves.push_back(Curves[i]);
			}
		}
		else {
			if (isDegenerate(Curves[i])) {
				throw Standard_Failure("Degenerate curve found in middle of the sequence");
			}
			else {
				middleCurves.push_back(Curves[i]);
			}
		}
	}
	if (middleCurves.empty()) {
		// If all curves are degenerate, return empty errors and curves
		compatibleCurves.clear();
		return std::vector<double>{0};
	}

	int sampling_num = 50;
	std::vector<std::vector<gp_Pnt>> sampling_points_matrix(middleCurves.size(), std::vector<gp_Pnt>());
	for (size_t i = 0; i < middleCurves.size(); i++) {
		UniformSamplingOnBspline(middleCurves[i], sampling_num, sampling_points_matrix[i]);
	}

	auto params = ComputeUniformParam(sampling_num + 1);

	int appro_degree = middleCurves[0]->Degree();
	for (size_t i = 1; i < middleCurves.size(); i++) {
		appro_degree = std::max(appro_degree, middleCurves[i]->Degree());
	}

	std::vector<double> insert_knots;
	std::vector<double> init_knots(appro_degree + 1, 0.0);
	init_knots.insert(init_knots.end(), appro_degree + 1, 1.0);

	std::vector<double> errors;
	auto comp_curves = IterateApproximate(errors, insert_knots, sampling_points_matrix, params, init_knots, appro_degree, 100, toler);
	compatibleCurves.clear();
	size_t middleIndex = 0;

	for (size_t i = 0; i < Curves.size(); ++i) {
		if (i == 0 && firstDegenerate) {
			Handle(Geom_BSplineCurve) refCurve = comp_curves[0];
			int degree = refCurve->Degree();
			const TColStd_Array1OfReal& knots = refCurve->Knots();
			const TColStd_Array1OfInteger& multiplicities = refCurve->Multiplicities();

			int numPoles = 0;
			for (int k = multiplicities.Lower(); k <= multiplicities.Upper(); ++k) {
				numPoles += multiplicities.Value(k);
			}
			numPoles -= (degree + 1);

			gp_Pnt degeneratePoint = Curves[i]->Pole(1);
			TColgp_Array1OfPnt poles(1, numPoles);
			for (int j = poles.Lower(); j <= poles.Upper(); ++j) {
				poles.SetValue(j, degeneratePoint);
			}

			Handle(Geom_BSplineCurve) newCurve = new Geom_BSplineCurve(
				poles, knots, multiplicities, degree);

			compatibleCurves.push_back(newCurve);
		}
		else if (i == Curves.size() - 1 && lastDegenerate) {
			Handle(Geom_BSplineCurve) refCurve = comp_curves.back();
			int degree = refCurve->Degree();
			const TColStd_Array1OfReal& knots = refCurve->Knots();
			const TColStd_Array1OfInteger& multiplicities = refCurve->Multiplicities();

			int numPoles = 0;
			for (int k = multiplicities.Lower(); k <= multiplicities.Upper(); ++k) {
				numPoles += multiplicities.Value(k);
			}
			numPoles -= (degree + 1);

			gp_Pnt degeneratePoint = Curves[i]->Pole(1);
			TColgp_Array1OfPnt poles(1, numPoles);
			for (int j = poles.Lower(); j <= poles.Upper(); ++j) {
				poles.SetValue(j, degeneratePoint);
			}
			Handle(Geom_BSplineCurve) newCurve = new Geom_BSplineCurve(
				poles, knots, multiplicities, degree);

			compatibleCurves.push_back(newCurve);
		}
		else {
			compatibleCurves.push_back(comp_curves[middleIndex]);
			middleIndex++;
		}
	}

	return errors;
}

// 两张曲面compatible
void Compatible::SetSurfaceCompatible(Handle(Geom_BSplineSurface)& L1, Handle(Geom_BSplineSurface)& L2)
{
    // uniform
	TColStd_Array1OfReal knotsu(1, L1->NbUKnots());
	L1->UKnots(knotsu);
	BSplCLib::Reparametrize(0, 1, knotsu);
	L1->SetUKnots(knotsu);

	TColStd_Array1OfReal knotsv(1, L1->NbVKnots());
	L1->VKnots(knotsv);
	BSplCLib::Reparametrize(0, 1, knotsv);
	L1->SetVKnots(knotsv);

	TColStd_Array1OfReal knotsu2(1, L2->NbUKnots());
	L2->UKnots(knotsu2);
	BSplCLib::Reparametrize(0, 1, knotsu2);
	L2->SetUKnots(knotsu2);

	TColStd_Array1OfReal knotsv2(1, L2->NbVKnots());
	L2->VKnots(knotsv2);
	BSplCLib::Reparametrize(0, 1, knotsv2);
	L2->SetVKnots(knotsv2);

	// Get the u knot vector
	Standard_Integer NbUKnot1 = L1->NbUKnots();
	TColStd_Array1OfReal    UKnots1(1, NbUKnot1);
	TColStd_Array1OfInteger UMults1(1, NbUKnot1);
	L1->UKnots(UKnots1);
	L1->UMultiplicities(UMults1);
	// Get the v knot vector
	Standard_Integer NbVKnot1 = L1->NbVKnots();
	TColStd_Array1OfReal    VKnots1(1, NbVKnot1);
	TColStd_Array1OfInteger VMults1(1, NbVKnot1);
	L1->VKnots(VKnots1);
	L1->VMultiplicities(VMults1);

	for (int i = 1; i <= NbUKnot1; i++)
	{
		L2->InsertUKnot(UKnots1(i), UMults1(i), 1.e-15, false);
	}
	for (int i = 1; i <= NbVKnot1; i++)
	{
		L2->InsertVKnot(VKnots1(i), VMults1(i), 1.e-15, false);
	}

	// Get the u knot vector
	Standard_Integer NbUKnot2 = L2->NbUKnots();
	TColStd_Array1OfReal    UKnots2(1, NbUKnot2);
	TColStd_Array1OfInteger UMults2(1, NbUKnot2);
	L2->UKnots(UKnots2);
	L2->UMultiplicities(UMults2);
	// Get the v knot vector
	Standard_Integer NbVKnot2 = L2->NbVKnots();
	TColStd_Array1OfReal    VKnots2(1, NbVKnot2);
	TColStd_Array1OfInteger VMults2(1, NbVKnot2);
	L2->VKnots(VKnots2);
	L2->VMultiplicities(VMults2);

	for (int i = 1; i <= NbUKnot2; i++)
	{
		L1->InsertUKnot(UKnots2(i), UMults2(i), 1.e-15, false);
	}
	for (int i = 1; i <= NbVKnot2; i++)
	{
		L1->InsertVKnot(VKnots2(i), VMults2(i), 1.e-15, false);
	}
}

// 三张曲面compatible
void Compatible::SetSurfaceCompatible(Handle(Geom_BSplineSurface)& L1, Handle(Geom_BSplineSurface)& L2, Handle(Geom_BSplineSurface)& L3)
{
	// Get the u knot vector
	Standard_Integer NbUKnot1 = L1->NbUKnots();
	TColStd_Array1OfReal    UKnots1(1, NbUKnot1);
	TColStd_Array1OfInteger UMults1(1, NbUKnot1);
	L1->UKnots(UKnots1);
	L1->UMultiplicities(UMults1);
	// Get the v knot vector
	Standard_Integer NbVKnot1 = L1->NbVKnots();
	TColStd_Array1OfReal    VKnots1(1, NbVKnot1);
	TColStd_Array1OfInteger VMults1(1, NbVKnot1);
	L1->VKnots(VKnots1);
	L1->VMultiplicities(VMults1);

	for (int i = 1; i <= NbUKnot1; i++)
	{
		L2->InsertUKnot(UKnots1(i), UMults1(i), 1.e-15, false);
	}
	for (int i = 1; i <= NbVKnot1; i++)
	{
		L2->InsertVKnot(VKnots1(i), VMults1(i), 1.e-15, false);
	}

	// Get the u knot vector
	Standard_Integer NbUKnot2 = L2->NbUKnots();
	TColStd_Array1OfReal    UKnots2(1, NbUKnot2);
	TColStd_Array1OfInteger UMults2(1, NbUKnot2);
	L2->UKnots(UKnots2);
	L2->UMultiplicities(UMults2);
	// Get the v knot vector
	Standard_Integer NbVKnot2 = L2->NbVKnots();
	TColStd_Array1OfReal    VKnots2(1, NbVKnot2);
	TColStd_Array1OfInteger VMults2(1, NbVKnot2);
	L2->VKnots(VKnots2);
	L2->VMultiplicities(VMults2);

	for (int i = 1; i <= NbUKnot2; i++)
	{
		L3->InsertUKnot(UKnots2(i), UMults2(i), 1.e-15, false);
	}
	for (int i = 1; i <= NbVKnot2; i++)
	{
		L3->InsertVKnot(VKnots2(i), VMults2(i), 1.e-15, false);
	}

	// Get the u knot vector
	Standard_Integer NbUKnot3 = L3->NbUKnots();
	TColStd_Array1OfReal    UKnots3(1, NbUKnot3);
	TColStd_Array1OfInteger UMults3(1, NbUKnot3);
	L3->UKnots(UKnots3);
	L3->UMultiplicities(UMults3);
	// Get the v knot vector
	Standard_Integer NbVKnot3 = L3->NbVKnots();
	TColStd_Array1OfReal    VKnots3(1, NbVKnot3);
	TColStd_Array1OfInteger VMults3(1, NbVKnot3);
	L3->VKnots(VKnots3);
	L3->VMultiplicities(VMults3);

	for (int i = 1; i <= NbUKnot3; i++)
	{
		L1->InsertUKnot(UKnots3(i), UMults3(i), 1.e-15, false);
		L2->InsertUKnot(UKnots3(i), UMults3(i), 1.e-15, false);
	}
	for (int i = 1; i <= NbVKnot3; i++)
	{
		L1->InsertVKnot(VKnots3(i), VMults3(i), 1.e-15, false);
		L2->InsertVKnot(VKnots3(i), VMults3(i), 1.e-15, false);
	}
	//至此，三张曲面的节点向量完全相同
}