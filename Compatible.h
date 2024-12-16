#pragma once
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


namespace Compatible{

	void UniformSamplingOnBspline(const Handle(Geom_BSplineCurve)& BsplineCurve, int numSamples, std::vector<gp_Pnt>& points);
	std::vector<double> ComputeUniformParam(int numSamples, double left = 0, double right = 1);
	template <typename T>
	std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>>& matrix);
	Handle(Geom_BSplineCurve) ApproximateC(const std::vector<gp_Pnt>& Pnts, std::vector<double>& params, std::vector<double>& FKnots, int degree);
	gp_Vec CalResPnt(int k, const std::vector<gp_Pnt>& dataPoints, const std::vector<double>& parameters, Standard_Integer p,
		std::vector<double>& Knots, int CtrlPntNum);
	Handle(Geom_BSplineCurve) ApproximateC(const std::vector<gp_Pnt>& Pnts, std::vector<double>& PntParams, TColStd_Array1OfReal& Knots, TColStd_Array1OfInteger& Mutis, std::vector<double>& FKnots,
		int degree);
	double SingleParamError(const Handle(Geom_BSplineCurve)& curve, double param, gp_Pnt point);
	double SingleParamError(const std::vector<Handle(Geom_BSplineCurve)>& curves, double param, std::vector<gp_Pnt> points);
	double MaxErrorOnCurve(const std::vector<gp_Pnt>& points, std::vector<double>& params, Handle(Geom_BSplineCurve)& BsplineCurve);
	std::vector<double> ComputeNewKnot(std::vector<double>& InsertKnots, std::vector<double>& OldKnots, std::vector<double>& DisCovParams);
	std::vector<Handle(Geom_BSplineCurve)> IterateApproximate(std::vector<double>& errors, std::vector<double>& InsertKnots, const std::vector<std::vector<gp_Pnt>>& Pnts,
		std::vector<double>& Params, std::vector<double>& InitKnots,
		int degree, int MaxIterNum, double toler);
	
	bool isDegenerate(const Handle(Geom_BSplineCurve)& curve);
	std::vector<double> ApproximateCompatible(std::vector<Handle(Geom_BSplineCurve)>& Curves, std::vector<Handle(Geom_BSplineCurve)>& compatibleCurves, double toler);

	void SetSurfaceCompatible(Handle(Geom_BSplineSurface)& L1, Handle(Geom_BSplineSurface)& L2);
	void SetSurfaceCompatible(Handle(Geom_BSplineSurface)& L1, Handle(Geom_BSplineSurface)& L2, Handle(Geom_BSplineSurface)& L3);
};

