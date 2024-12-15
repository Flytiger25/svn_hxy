#include "stdafx.h"
#include "SurfaceModelingTool_ACIS.h"
#include "position.hxx"
#include <sps3crtn.hxx>
#include <algorithm>
#include "intdef.hxx"

int SurfaceModelingTool_ACIS::OffsetCurve(const curve* theOriginalCurve, curve* theOffsetCurve, double theOffsetDistance)
{
	//当前只处理次数为三次的样条曲线(非有理)，且样条曲线的起始和终止节点的重复度为 4 , 且内部的节点重复度小于等于2
	//其余的情况可以进行扩展和完善，核心算法一致

	int dim, deg, num_ctrlpts, num_knots;
	logical rat;
	SPAposition* ctrlpts;
	double* weights, * knots;

	int dim_offset, deg_offset, num_ctrlpts_offset, num_knots_offset;

	bs3_curve theOriginalCurve_bs3curve = ((intcurve*)theOriginalCurve)->cur();

	//检查曲线是否为平面曲线，如果不是返回0
	SPAunit_vector thePlaneNormal;
	BOOL isPlanarCurve = bs3_curve_planar(theOriginalCurve_bs3curve, thePlaneNormal);
	if (!isPlanarCurve)
		return 0;

	//首先获取原始曲线的信息
	bs3_curve_to_array(theOriginalCurve_bs3curve, dim, deg, rat, num_ctrlpts, ctrlpts, weights, num_knots, knots);

	//bs3_curve_offset_by_dist //这是acis对应的offset方法，要进行比对

	int mul;
	//添加节点向量，确保所有内部节点的重复度为 2 
	for (int i = 4; i < num_knots - 4; i++)
	{
		mul = bs3_curve_knot_mult(theOriginalCurve_bs3curve, knots[i], SPAresabs);
		if (mul == 1)
			bs3_curve_add_knot(theOriginalCurve_bs3curve, knots[i], 2, SPAresabs);
	}

	//删除原先的
	ACIS_DELETE(ctrlpts);
	ACIS_DELETE(knots);

	//重新获取插入节点后的曲线信息
	bs3_curve_to_array(theOriginalCurve_bs3curve, dim, deg, rat, num_ctrlpts, ctrlpts, weights, num_knots, knots);

	std::vector<SPAposition> ctrlpts_offset;
	std::vector<double> knots_offset;

	//offset曲线的控制顶点和节点向量
	//offset曲线开始的节点向量等于原始曲线的节点向量
	ctrlpts_offset.reserve(num_ctrlpts * 2);
	knots_offset.reserve(num_knots);

	//copy the knots of the original curve to the offset curve
	//std::generate(knots, knots + num_knots, knots_offset);
	for (int i = 0; i < num_knots; i++)
		knots_offset.push_back(knots[i]);

	//计算初始offset曲线控制顶点
	//首先计算所有节点处的点的offset，然后根据切向量计算前后的控制顶点
	double param;
	SPAposition pos, offsetpos;
	SPAvector firstvec, offsetfirstvec;
	SPAvector curvature_vec;
	double curvature;
	double offsetcurvature;
	for (int i = 3; i <= num_knots - 4; i = i + 2)
	{
		param = knots[i];
		theOriginalCurve->eval(param, pos, firstvec);

		//curvature whose direction is orthogonal to the tangent vector
		curvature_vec = theOriginalCurve->eval_curvature(param);
		SPAvector theOffsetVec = thePlaneNormal * firstvec;

		//offset direction
		SPAunit_vector theOffsetVec_unit = normalise(theOffsetVec);
		curvature = curvature_vec.len();

		//计算offset后点的curvature
		offsetcurvature = 1.0 / (1.0 / curvature - theOffsetDistance);

		//计算offset后的点坐标
		offsetpos = pos + theOffsetVec_unit * theOffsetDistance;

		//因为是同参数化，计算offset后点处的一阶切向量
		offsetfirstvec = firstvec * curvature / offsetcurvature;

		if (i == 3)
		{
			ctrlpts_offset.push_back(offsetpos);
			ctrlpts_offset.push_back(offsetpos + offsetfirstvec / 3);
		}
		else
			if (i == num_knots - 4)
			{
				ctrlpts_offset.push_back(offsetpos - offsetfirstvec / 3);
				ctrlpts_offset.push_back(offsetpos);
			}
			else
			{
				ctrlpts_offset.push_back(offsetpos - offsetfirstvec / 3);
				ctrlpts_offset.push_back(offsetpos);
				ctrlpts_offset.push_back(offsetpos + offsetfirstvec / 3);
			}
	}

	//从后往前检测offset曲线的逼近精度，对不满足精度要求的曲线进行迭代细分
	//从最后一个内部节点开始
	for (int i = num_knots - 5; i >= 3; i = i - 2)
	{
		//判断knot[i]和knot[i + 1]之间是否满足精度要求，不满足要求则细分

		JudgeAndSplitCurve(theOriginalCurve, ctrlpts_offset, knots_offset, i, theOffsetDistance, thePlaneNormal);
	}

	//
	//std::vector<SPAposition> ctrlpts_offset;

	//bs3_curve_from_ctrlpts() 可以通过这个函数生成对应的offset结果曲线
	return 1;
}

int SurfaceModelingTool_ACIS::JudgeAndSplitCurve(const curve* theOriginalCurve, std::vector<SPAposition>& ctrlpts_offset, std::vector<double>& knots_offset, int index, double theOffsetDistance, SPAunit_vector thePlaneNormal)
{
	//判断ctrlpts_offset[(i - 3) * 3 / 2] - ctrlpts_offset[(i - 3) * 3 / 2 + 3]之间的控制顶点
	//节点区间 knots_offset[index] - knots_offset[index + 1]对应的曲线是否满足要求

	std::vector<SPAposition> ctrlpts_bezier;
	int ctrlfirstindex = (index - 3) * 3 / 2;
	ctrlpts_bezier.insert(ctrlpts_bezier.begin(), ctrlpts_offset.begin() + ctrlfirstindex, ctrlpts_offset.begin() + ctrlfirstindex + 3);

	double startpara = knots_offset[index];
	double endpara = knots_offset[index + 1];

	double middlepara = (startpara + endpara) / 2;

	ctrlpts_bezier.reserve(12);

	//一共生成10个点
	SPAposition p0 = ctrlpts_bezier[0];
	SPAposition p1 = ctrlpts_bezier[1];
	SPAposition p2 = ctrlpts_bezier[2];
	SPAposition p3 = ctrlpts_bezier[3];
	SPAposition p4 = interpolate(0.5, p0, p1);
	SPAposition p5 = interpolate(0.5, p1, p2);
	SPAposition p6 = interpolate(0.5, p2, p3);

	SPAposition p7 = interpolate(0.5, p0, p4);
	SPAposition p8 = interpolate(0.5, p4, p5);
	SPAposition p9 = interpolate(0.5, p5, p6);
	SPAposition p10 = interpolate(0.5, p6, p3);

	SPAposition p11 = interpolate(0.5, p8, p9);
	//SPAposition p11 = interpolate(0.5, interpolate(0.5, interpolate(0.5, p0, p1), interpolate(0.5, p1, p2)), interpolate(0.5, interpolate(0.5, p1, p2), p6));


	//第一次细分
	for (int i = 0; i < 3; i++)
	{
		ctrlpts_bezier[4 + i] = interpolate(0.5, ctrlpts_bezier[i], ctrlpts_bezier[i + 1]);
	}

	for (int i = 0; i < 2; i++)
	{
		ctrlpts_bezier[8 + i] = interpolate(0.5, ctrlpts_bezier[i + 4], ctrlpts_bezier[i + 5]);
	}
	ctrlpts_bezier[7] = interpolate(0.5, ctrlpts_bezier[0], ctrlpts_bezier[4]);
	ctrlpts_bezier[10] = interpolate(0.5, ctrlpts_bezier[6], ctrlpts_bezier[3]);
	ctrlpts_bezier[11] = interpolate(0.5, ctrlpts_bezier[8], ctrlpts_bezier[9]);


	//for (int i = 0; i < 8; i++)
	//{
	//	ctrlpts_bezier.push_back(interpolate(0.5, ctrlpts_offset[i], ctrlpts_offset[i + 1]));
	//}

	SPAposition pos;
	SPAvector theFirstDeriv;
	theOriginalCurve->eval(middlepara, pos, theFirstDeriv);

	SPAvector curvature_vec = theOriginalCurve->eval_curvature(middlepara);

	//offset direction
	SPAvector theOffsetVec = thePlaneNormal * theFirstDeriv;
	SPAunit_vector theOffsetVec_unit = normalise(theOffsetVec);

	//计算offset后的点坐标
	SPAposition offsetpos = pos + theOffsetVec_unit * theOffsetDistance;

	//计算误差
	double dis = (offsetpos - p11).len();
	if (dis > SPAresfit)
	{
		//细分
		//删除两个控制顶点，增加四个控制顶点，增加两个节点向量
		ctrlpts_offset[index + 1] = ctrlpts_bezier[4];
		ctrlpts_offset[index + 2] = ctrlpts_bezier[7];
		ctrlpts_offset.insert(ctrlpts_offset.begin() + index + 2, offsetpos);
		ctrlpts_offset.insert(ctrlpts_offset.begin() + index + 3, ctrlpts_bezier[8]);
		ctrlpts_offset.insert(ctrlpts_offset.begin() + index + 4, ctrlpts_bezier[6]);

		knots_offset.insert(knots_offset.begin() + index + 3, middlepara);
		knots_offset.insert(knots_offset.begin() + index + 3, middlepara);

		//
		JudgeAndSplitCurve(theOriginalCurve, ctrlpts_offset, knots_offset, index + 2, theOffsetDistance, thePlaneNormal);
		JudgeAndSplitCurve(theOriginalCurve, ctrlpts_offset, knots_offset, index, theOffsetDistance, thePlaneNormal);
	}
	return 1;
}