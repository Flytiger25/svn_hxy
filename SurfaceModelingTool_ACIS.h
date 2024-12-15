#pragma once
#include "bs3c_def.hxx"
#include "bs3curve.hxx"
#include "position.hxx"
class curve;
class SurfaceModelingTool_ACIS
{
public:
	//
	static int OffsetCurve(const curve* theOriginalCurve, curve* theOffsetCurve, double theOffsetDistance);

	//
	static int JudgeAndSplitCurve(const curve* theOriginalCurve, std::vector<SPAposition>& ctrlpts_offset, std::vector<double>& knots_offset, int index, double theOffsetDistance, SPAunit_vector thePlaneNormal);
};

