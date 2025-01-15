#pragma once
#include "Geom_BSplineCurve.hxx"
#include "Geom_BSplineSurface.hxx"
class SurfaceModelingTool_OCC
{
public:
	//约定输入四条边满足如下的方向要求
	// 四条边连接和方向满足要求 G0连续的Coons曲面构造
	// 
	//                -------------curve1--->-------
	//                |                             |
	//              curve4                        curve 2
	//                v                             v
	//                |                             |
	//                |-----------curve3--->--------
	static void Coons_G0(Handle(Geom_BSplineCurve)& curve1, Handle(Geom_BSplineCurve)& curve2, Handle(Geom_BSplineCurve)& curve3, Handle(Geom_BSplineCurve)& curve4, Handle(Geom_BSplineSurface)& mySurface_coons);

	//              约定输入四条边和四条边的跨界导矢满足如下的方向要求
	//                           G1连续的Coons曲面构造
	// 
	//                ----------------------c1--->------------
	//                |                     |                |
	//               c4-->c4_d            c1_d              c2 -->-c2_d-->
	//                v                     v                v
	//                |                     |                |
	//                |--------------------c3--->-------------
	//                                      |
	//                                      v
	//                                    c3_d  
	//                                      | 
	static void Coons_G1(Handle(Geom_BSplineCurve)& c1, Handle(Geom_BSplineCurve)& c2, Handle(Geom_BSplineCurve)& c3, Handle(Geom_BSplineCurve)& c4, Handle(Geom_BSplineCurve)& c1_derivative, Handle(Geom_BSplineCurve)& curve2_derivative, Handle(Geom_BSplineCurve)& curve3_derivative, Handle(Geom_BSplineCurve)& curve4_derivative, Handle(Geom_BSplineSurface)& mySurface_coons);

	//make the curve same degree and knots
	static Standard_Integer SetSameDistribution(Handle(Geom_BSplineCurve)& C1, Handle(Geom_BSplineCurve)& C2);

	//make the curve arranged for the G0 construction
	// Input: curveArray, the curve array
	// Output: bslpineCurve1-bslpineCurve4 the arranged four boundary curves
	// IsModify: If the curve are not connected end to end, whether change the end point of the curve
	// Tol: the tolerance to check whether the four curves are connected or not

	static int Arrange_Coons_G0(std::vector<Handle(Geom_BSplineCurve)>& curveArray, Handle(Geom_BSplineCurve)& bslpineCurve1, Handle(Geom_BSplineCurve)& bslpineCurve2, Handle(Geom_BSplineCurve)& bslpineCurve3, Handle(Geom_BSplineCurve)& bslpineCurve4, double Tol, int IsModify);

	static void ApproximateBoundaryCurves(std::vector<Handle(Geom_BSplineCurve)>& curves, Standard_Integer samplingNum = 50);

	static void TrimInternalCurves(
		std::vector<Handle(Geom_BSplineCurve)>& theInternalBSplineCurves,
		const std::vector<Handle(Geom_BSplineCurve)>& theBoundaryCurveArray,
		Standard_Real theToleranceDistance);

	static Standard_Real ComputeCurveCurveDistance(const Handle(Geom_BSplineCurve)& theCurve, const Handle(Geom_BSplineCurve)& theBoundaryCurve);
};

