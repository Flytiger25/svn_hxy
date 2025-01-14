#pragma once
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Eigen/Dense>

namespace GuideGordon {

	void GuideGordonSurf(Handle(Geom_Surface) originalGordon,
		std::vector<Handle(Geom_BSplineCurve)> guideCurves, Handle(Geom_BSplineSurface)& guidedGordon);

	void GuideGordonSurf(Handle(Geom_Surface) originalGordon, std::vector<double> uIsoparamParams, std::vector<double> vIsoparamParams,
		std::vector<Handle(Geom_BSplineCurve)> guideCurves, Handle(Geom_BSplineSurface)& guidedGordon, double tol = 5.0);

	std::vector<gp_Pnt> SampleGuideCurve(const Handle(Geom_BSplineCurve)& curve, Standard_Integer numSamples);

	std::vector<gp_Pnt> ProjectPointsToSurface(const std::vector<gp_Pnt>& points, const Handle(Geom_Surface)& surface, std::vector<gp_Pnt2d>& PntParams);

	std::vector<gp_Pnt> CalculateOffsets(const std::vector<gp_Pnt>& samples, const std::vector<gp_Pnt>& projections);

	void UniformSurface(Handle(Geom_BSplineSurface)& surface);

	void ExpandKnots(Handle(Geom_BSplineSurface)& surface);
};

