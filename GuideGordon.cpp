#include "stdafx.h"
#include "GuideGordon.h"
#include "Interpolate.h"
#include "Compatible.h"
#include "FitConstrainedBSplineSurf.h"

#include <Eigen/Dense>
#include <GeomAPI_ProjectPointOnSurf.hxx>


// 不拟合偏移曲面，在数据点处加上偏移量
void GuideGordon::GuideGordonSurf(Handle(Geom_Surface) originalGordon,
    std::vector<Handle(Geom_BSplineCurve)> guideCurves, Handle(Geom_BSplineSurface)& guidedGordon)
{
    Handle(Geom_BSplineSurface) originSurf = Handle(Geom_BSplineSurface)::DownCast(originalGordon);
    UniformSurface(originSurf);

    if (originSurf.IsNull() || guideCurves.size() == 0)
    {
        std::cout << "输入为空！" << std::endl;
        return;
    }

    //std::vector<Handle(Geom_BSplineCurve)> guideCurves1;
    //guideCurves1.push_back(guideCurves[13]);

    std::vector<gp_Pnt2d> pntParams;
    std::vector<gp_Pnt> offsets;
    for (Handle(Geom_BSplineCurve) guideCurve : guideCurves)
    {
        // 得到采样点
        std::vector<gp_Pnt> samples, projectPnts;
        samples = SampleGuideCurve(guideCurve, 9);

        // 得到投影点和参数
        std::vector<gp_Pnt2d> aPntParams;
        projectPnts = ProjectPointsToSurface(samples, originSurf, aPntParams);
        for (gp_Pnt2d aParam : aPntParams)
        {
            pntParams.push_back(aParam);
        }

        // 计算偏移量
        std::vector<gp_Pnt> anOffsets;
        anOffsets = CalculateOffsets(samples, projectPnts);
        for (gp_Pnt aPnt : anOffsets)
        {
            offsets.push_back(aPnt);
        }
    }

    TColStd_Array1OfReal knotsU = originSurf->UKnots();
    TColStd_Array1OfReal knotsV = originSurf->VKnots();
    TColStd_Array1OfInteger multiplicitiesU = originSurf->UMultiplicities();
    TColStd_Array1OfInteger multiplicitiesV = originSurf->VMultiplicities();

    Standard_Integer degreeU = originSurf->UDegree();
    Standard_Integer degreeV = originSurf->VDegree();

    Standard_Integer numU = originSurf->NbUPoles();
    Standard_Integer numV = originSurf->NbVPoles();

    std::vector<Standard_Real> paramsU;
    std::vector<Standard_Real> paramsV;

    for (Standard_Integer i = 1; i <= numU; ++i)
    {
        paramsU.push_back(Standard_Real(i - 1) / (numU - 1));  // U 参数均匀分布
    }
    for (Standard_Integer i = 1; i <= numV; ++i)
    {
        paramsV.push_back(Standard_Real(i - 1) / (numV - 1));  // V 参数均匀分布
    }


    // 计算新的控制点
    std::vector<gp_Pnt> newOffsets;
    std::vector<gp_Pnt2d> newPntParams;

    for (Standard_Integer i = 0; i < numU; i++)
    {
        for (Standard_Integer j = 0; j < numV; j++)
        {
            Standard_Real u = paramsU[i];  // 控制点的 U 参数位置
            Standard_Real v = paramsV[j];  // 控制点的 V 参数位置

            // 通过参数坐标计算原曲面上的点
            gp_Pnt originalPoint;
            originSurf->D0(u, v, originalPoint);  // 获取原曲面上的点

            // 查找与此参数坐标对应的偏移量
            gp_Pnt offset = gp_Pnt(0.0, 0.0, 0.0); // 默认偏移量为零
            bool foundOffset = false;

            // 查找该点的偏移量
            for (size_t k = 0; k < pntParams.size(); ++k)
            {
                if (std::abs(pntParams[k].X() - u) < 0.05 && std::abs(pntParams[k].Y() - v) < 0.05)
                {
                    offset = offsets[k];  // 找到偏移量
                    foundOffset = true;
                    break;
                }
            }

            // 将偏移量加到曲面点上，得到新的控制点
            gp_Pnt newControlPoint;
            newControlPoint.SetX(originalPoint.X() + offset.X());
            newControlPoint.SetY(originalPoint.Y() + offset.Y());
            newControlPoint.SetZ(originalPoint.Z() + offset.Z());
            newOffsets.push_back(newControlPoint);
            newPntParams.push_back(gp_Pnt2d(u, v));

            std::cout << foundOffset << std::endl;
        }
    }

    // 使用原曲面的节点向量、次数、重复度拟合新的 B 样条曲面
    TColgp_Array2OfPnt controlPoints(1, numU, 1, numV);

    for (Standard_Integer i = 1; i <= numU; ++i)
    {
        for (Standard_Integer j = 1; j <= numV; ++j)
        {
            Standard_Integer idx = (i - 1) * numV + (j - 1);
            controlPoints.SetValue(i, j, newOffsets[idx]);
        }
    }

    // 使用 OpenCascade 创建 B 样条曲面
    guidedGordon = new Geom_BSplineSurface(
        controlPoints,
        knotsU, knotsV,
        multiplicitiesU, multiplicitiesV,
        degreeU, degreeV);  // 使用原曲面的节点向量、次数、重复度

}

// 12.9 按照论文中实现
void GuideGordon::GuideGordonSurf(Handle(Geom_Surface) originalGordon, std::vector<Standard_Real> uIsoparamParams, std::vector<Standard_Real> vIsoparamParams,
    std::vector<Handle(Geom_BSplineCurve)> guideCurves, Handle(Geom_BSplineSurface)& guidedGordon)
{
    Handle(Geom_BSplineSurface) originSurf = Handle(Geom_BSplineSurface)::DownCast(originalGordon);
    UniformSurface(originSurf);

    if (originSurf.IsNull() || guideCurves.size() == 0)
    {
        std::cout << "输入为空！" << std::endl;
        return;
    }

    // 定义采样点个数
    Standard_Real num = 10;

    //std::vector<Handle(Geom_BSplineCurve)> guideCurves1;
    //guideCurves1.push_back(guideCurves[3]);

    std::vector<gp_Pnt2d> pntParams;
    std::vector<gp_Pnt> offsets;
    for (Handle(Geom_BSplineCurve) guideCurve : guideCurves)
    {
        // 得到采样点
        std::vector<gp_Pnt> samples, projectPnts;
        samples = SampleGuideCurve(guideCurve, num);

        // 得到投影点和参数
        std::vector<gp_Pnt2d> aPntParams;
        projectPnts = ProjectPointsToSurface(samples, originSurf, aPntParams);
        for (gp_Pnt2d aParam : aPntParams)
        {
            pntParams.push_back(aParam);
        }

        // 计算偏移量
        std::vector<gp_Pnt> anOffsets;
        anOffsets = CalculateOffsets(samples, projectPnts);
        for (gp_Pnt aPnt : anOffsets)
        {
            offsets.push_back(aPnt);
        }
    }

    // 输出偏移量距离
    Standard_Real sum = 0;
    for (gp_Pnt aPnt : offsets)
    {
        Standard_Real x = aPnt.X();
        Standard_Real y = aPnt.Y();
        Standard_Real z = aPnt.Z();
        Standard_Real dis = std::sqrt(x * x + y * y + z * z);
        sum += dis;
        std::cout << "distance: " << dis << std::endl;
    }
    std::cout << " average distance: " << sum / offsets.size() << std::endl;

    /*
    Standard_Integer numU = 11;
    Standard_Integer numV = 11;

    Standard_Real minU = 0.0, maxU = 1.0, minV = 0.0, maxV = 1.0;

    // 设置节点向量
    TColStd_Array1OfReal knotsU(1, numU);
    TColStd_Array1OfReal knotsV(1, numV);

    for (Standard_Integer i = 1; i <= numU; ++i)
    {
        knotsU.SetValue(i, 0.1 * (i - 1));
    }

    for (Standard_Integer i = 1; i <= numV; ++i)
    {
        knotsV.SetValue(i, 0.1 * (i - 1));
    }

    TColStd_Array1OfInteger multiplicitiesU(1, numU);
    TColStd_Array1OfInteger multiplicitiesV(1, numV);

    for (Standard_Integer i = 1; i <= numU; ++i)
    {
        multiplicitiesU.SetValue(i, (i == 1 || i == numU) ? 4 : 1);
    }
    for (Standard_Integer i = 1; i <= numV; ++i)
    {
        multiplicitiesV.SetValue(i, (i == 1 || i == numV) ? 4 : 1);
    }
    */

    /* 暂时不添加额外数据点
    // 在没有数据点支持的参数区域添加额外的零数据点

    std::vector<gp_Pnt> newOffsets;
    std::vector<gp_Pnt2d> newPntParams;

    // 遍历节点向量和已有的参数坐标，找到没有数据点支持的区域
    for (int i = 1; i <= knotsU.Size() - 1; ++i)
    {
        for (int j = 1; j <= knotsV.Size() - 1; ++j)
        {
            Standard_Real umin = knotsU.Value(i);
            Standard_Real umax = knotsU.Value(i + 1);
            Standard_Real vmin = knotsV.Value(j);
            Standard_Real vmax = knotsV.Value(j + 1);

            Standard_Real u = (umin + umax) / 2.0;
            Standard_Real v = (vmin + vmax) / 2.0;

            bool isInDataRegion = false;

            gp_Pnt2d targetParam(0.0, 0.0);  // 默认参数值
            gp_Pnt targetOffset(0.0, 0.0, 0.0);  // 默认偏移量
            Standard_Real weightSum = 0.0;  // 用于加权平均的权重总和

            // 遍历已有数据点，计算权重
            for (size_t k = 0; k < pntParams.size(); ++k)
            {
                Standard_Real curU = pntParams[k].X();
                Standard_Real curV = pntParams[k].Y();

                // 如果点在区域内，直接添加
                if (InterPolateTool::isLessThan(curU, umax, 0.001) &&
                    InterPolateTool::isGreaterThan(curU, umin, 0.001) &&
                    InterPolateTool::isLessThan(curV, vmax, 0.001) &&
                    InterPolateTool::isGreaterThan(curV, vmin, 0.001))
                {
                    newOffsets.push_back(offsets[k]);
                    newPntParams.push_back(pntParams[k]);
                    isInDataRegion = true;
                    break;
                }

                // 如果点不在区域内，计算距离和权重
                Standard_Real distance = std::sqrt((u - curU) * (u - curU) + (v - curV) * (v - curV));
                Standard_Real weight = std::exp(1.0 / (distance + 0.001));
                //if (distance >= 0.3) weight = 0.0;
                weightSum += weight;

                // 设置偏移量
                targetOffset.SetX(targetOffset.X() + offsets[k].X() * weight);
                targetOffset.SetY(targetOffset.Y() + offsets[k].Y() * weight);
                targetOffset.SetZ(targetOffset.Z() + offsets[k].Z() * weight);
            }

            // 如果区域内没有数据点，则补充点
            if (!isInDataRegion)
            {
                if (weightSum > 0.0)
                {
                    targetOffset.SetX(0);
                    targetOffset.SetY(0);
                    targetOffset.SetZ(0);
                }
                //gp_Pnt targetOffset1(0.0, 0.0, 0.0);
                newOffsets.push_back(targetOffset); // 添加插值结果
                newPntParams.push_back(gp_Pnt2d(u, v));
            }
        }
    }
    */

    // 将gp_Pnt和gp_Pnt2d类型转换为Eigen类型
    std::vector<Eigen::Vector3d> offsetsEigen;
    std::vector<Standard_Real> pntParamsU, pntParamsV;
    for (const gp_Pnt& offset : offsets)
    {
        offsetsEigen.push_back(Eigen::Vector3d(offset.X(), offset.Y(), offset.Z()));
    }
    for (const gp_Pnt2d& param : pntParams)
    {
        pntParamsU.push_back(param.X());
        pntParamsV.push_back(param.Y());
    }


    // 调用偏移曲面拟合函数
    // 12.16 用初始gordon的节点向量，后面无需compatible
    //TColStd_Array1OfReal uKnots = knotsU;
    //TColStd_Array1OfReal vKnots = knotsV;
    //TColStd_Array1OfInteger uMults = multiplicitiesU;
    //TColStd_Array1OfInteger vMults = multiplicitiesV;
    TColStd_Array1OfReal uKnots = originSurf->UKnots();
    TColStd_Array1OfReal vKnots = originSurf->VKnots();
    TColStd_Array1OfInteger uMults = originSurf->UMultiplicities();
    TColStd_Array1OfInteger vMults = originSurf->VMultiplicities();
    Standard_Integer uDegree = originSurf->UDegree();
    Standard_Integer vDegree = originSurf->VDegree();

    // 转换节点矢量为 std::vector<Standard_Real>
    std::vector<Standard_Real> stdUKnots, stdVKnots;
    for (Standard_Integer i = 1; i <= uKnots.Length(); ++i)
    {
        for (Standard_Integer j = 1; j <= uMults[i]; ++j)
        {
            stdUKnots.push_back(uKnots[i]);
        }
    }
    for (Standard_Integer i = 1; i <= vKnots.Length(); ++i)
    {
        for (Standard_Integer j = 1; j <= vMults[i]; ++j)
        {
            stdVKnots.push_back(vKnots[i]);
        }
    }

    Standard_Integer numCtrlPtsU = stdUKnots.size() - uDegree - 1;
    Standard_Integer numCtrlPtsV = stdVKnots.size() - vDegree - 1;
    std::vector<Eigen::Vector3d> controlPoints(numCtrlPtsU * numCtrlPtsV);

    FitConstrainedBSplineSurf::FitOffsetSurface(offsetsEigen, pntParamsU, pntParamsV, stdUKnots, stdVKnots, uIsoparamParams, vIsoparamParams, uDegree, vDegree, numCtrlPtsU, numCtrlPtsV, controlPoints);

    // 将拟合得到的控制点转换回gp_Pnt类型
    TColgp_Array2OfPnt poles(1, numCtrlPtsU, 1, numCtrlPtsV);

    for (Standard_Integer i = 1; i <= numCtrlPtsU; ++i)
    {
        for (Standard_Integer j = 1; j <= numCtrlPtsV; ++j)
        {
            poles(i, j).SetX(controlPoints[(i - 1) * numCtrlPtsV + (j - 1)](0));
            poles(i, j).SetY(controlPoints[(i - 1) * numCtrlPtsV + (j - 1)](1));
            poles(i, j).SetZ(controlPoints[(i - 1) * numCtrlPtsV + (j - 1)](2));
            //std::cout << controlPoints[(i - 1) * numCtrlPtsV + (j - 1)](0) << " " << controlPoints[(i - 1) * numCtrlPtsV + (j - 1)](1)
            //    << " " << controlPoints[(i - 1) * numCtrlPtsV + (j - 1)](2) << std::endl;
        }
    }

    // 创建新的引导后的曲面
    Handle(Geom_BSplineSurface) offsetSurf = new Geom_BSplineSurface(poles, uKnots, vKnots, uMults, vMults, uDegree, vDegree);

    if (offsetSurf.IsNull())
    {
        std::cout << "偏移曲面拟合失败！" << std::endl;
        return;
    }
    else
    {
        STEPControl_Writer stepWriter;
        TopoDS_Face face1 = BRepBuilderAPI_MakeFace(offsetSurf, Precision::Confusion());
        stepWriter.Transfer(face1, STEPControl_AsIs);
        std::string filePath = "D:\\work\\svn_hxy\\data\\offsetSurf\\offsetSurf.step";
        IFSelect_ReturnStatus status = stepWriter.Write(filePath.c_str());
    }

    // 对原始gordon曲面和偏移曲面compatible
    //Compatible::SetSurfaceCompatible(originSurf, offsetSurf);

    // 得到共同节点
    const TColStd_Array1OfReal knotsU1 = originSurf->UKnots();
    const TColStd_Array1OfReal knotsV1 = originSurf->VKnots();
    const TColStd_Array1OfReal knotsU2 = offsetSurf->UKnots();
    const TColStd_Array1OfReal knotsV2 = offsetSurf->VKnots();
    const TColStd_Array1OfInteger multsU1 = originSurf->UMultiplicities();
    const TColStd_Array1OfInteger multsV1 = originSurf->VMultiplicities();
    const TColStd_Array1OfInteger multsU2 = offsetSurf->UMultiplicities();
    const TColStd_Array1OfInteger multsV2 = offsetSurf->VMultiplicities();

    const Standard_Integer degreeU = originSurf->UDegree();
    const Standard_Integer degreeV = offsetSurf->VDegree();

    // 计算控制点 
    const TColgp_Array2OfPnt poles1 = originSurf->Poles();
    const TColgp_Array2OfPnt poles2 = offsetSurf->Poles();

    const Standard_Integer nbPolesU = originSurf->NbUPoles();
    const Standard_Integer nbPolesV = originSurf->NbVPoles();
    const Standard_Integer nbPolesU2 = offsetSurf->NbUPoles();
    const Standard_Integer nbPolesV2 = offsetSurf->NbVPoles();

    TColgp_Array2OfPnt resPole(1, nbPolesU, 1, nbPolesV);

    for (Standard_Integer i = 1; i <= nbPolesU; i++)
    {
        for (Standard_Integer j = 1; j <= nbPolesV; j++)
        {
            gp_XYZ coord = poles1(i, j).Coord() + poles2(i, j).Coord();
            //std::cout << "x=" << poles2(i, j).Coord().X() << " y=" << poles2(i, j).Coord().Y() << " z=" << poles2(i, j).Coord().Z() << std::endl;
            resPole(i, j).SetCoord(coord.X(), coord.Y(), coord.Z());
        }
    }

    guidedGordon = new Geom_BSplineSurface(resPole, knotsU1,
        knotsV1, multsU1, multsV1, degreeU, degreeV);

    std::vector<gp_Pnt2d> pntParams1;
    std::vector<gp_Pnt> offsets1;
    for (Handle(Geom_BSplineCurve) guideCurve : guideCurves)
    {
        // 得到采样点
        std::vector<gp_Pnt> samples, projectPnts;
        samples = SampleGuideCurve(guideCurve, num);

        // 得到投影点和参数
        std::vector<gp_Pnt2d> aPntParams;
        projectPnts = ProjectPointsToSurface(samples, guidedGordon, aPntParams);
        for (gp_Pnt2d aParam : aPntParams)
        {
            pntParams.push_back(aParam);
        }

        // 计算偏移量
        std::vector<gp_Pnt> anOffsets;
        anOffsets = CalculateOffsets(samples, projectPnts);
        for (gp_Pnt aPnt : anOffsets)
        {
            offsets1.push_back(aPnt);
        }
    }

    // 输出偏移量距离
    Standard_Real sum1 = 0;
    for (gp_Pnt aPnt : offsets1)
    {
        Standard_Real x = aPnt.X();
        Standard_Real y = aPnt.Y();
        Standard_Real z = aPnt.Z();
        Standard_Real dis = std::sqrt(x * x + y * y + z * z);
        sum1 += dis;
        std::cout << "distance: " << dis << std::endl;
    }
    std::cout << " average distance: " << sum1 / offsets1.size() << std::endl;
}


// 对引导线采样
std::vector<gp_Pnt> GuideGordon::SampleGuideCurve(const Handle(Geom_BSplineCurve)& curve, Standard_Integer numSamples) {
    std::vector<gp_Pnt> samples;
    for (Standard_Integer i = 0; i <= numSamples; ++i) {
        Standard_Real t = curve->FirstParameter() + i * (curve->LastParameter() - curve->FirstParameter()) / numSamples;
        gp_Pnt sample = curve->Value(t);
        samples.push_back(sample);
    }
    return samples;
}

// 获取投影点
std::vector<gp_Pnt> GuideGordon::ProjectPointsToSurface(const std::vector<gp_Pnt>& points, const Handle(Geom_Surface)& surface, std::vector<gp_Pnt2d>& PntParams) {
    std::vector<gp_Pnt> projectedPoints;
    for (const auto& point : points) {
        // 创建投影对象，不指定参数域
        GeomAPI_ProjectPointOnSurf projector(point, surface);
        Standard_Real u, v;

        if (projector.NbPoints() > 0) {
            gp_Pnt projectedPoint = projector.NearestPoint();
            projectedPoints.push_back(projectedPoint);
            projector.LowerDistanceParameters(u, v);
            gp_Pnt2d uv(u, v);
            PntParams.push_back(uv);
        }
    }
    return projectedPoints;
}

// 计算偏移量
std::vector<gp_Pnt> GuideGordon::CalculateOffsets(const std::vector<gp_Pnt>& samples, const std::vector<gp_Pnt>& projections) {
    std::vector<gp_Pnt> offsets;
    for (size_t i = 0; i < samples.size(); ++i) {
        gp_Pnt offset = samples[i].XYZ() - projections[i].XYZ();
        offsets.push_back(offset);
    }
    return offsets;
}

void GuideGordon::UniformSurface(Handle(Geom_BSplineSurface)& surface)
{
    TColStd_Array1OfReal surfaceUKnots(1, surface->NbUKnots());
    TColStd_Array1OfReal surfaceVKnots(1, surface->NbVKnots());
    surface->UKnots(surfaceUKnots);
    surface->VKnots(surfaceVKnots);
    if (!(surfaceUKnots(surfaceUKnots.Lower()) == 0 && surfaceUKnots(surfaceUKnots.Upper()) == 1))
    {
        BSplCLib::Reparametrize(0, 1, surfaceUKnots);
        surface->SetUKnots(surfaceUKnots);
    }
    if (!(surfaceVKnots(surfaceVKnots.Lower()) == 0 && surfaceVKnots(surfaceVKnots.Upper()) == 1))
    {
        BSplCLib::Reparametrize(0, 1, surfaceVKnots);
        surface->SetVKnots(surfaceVKnots);
    }
}