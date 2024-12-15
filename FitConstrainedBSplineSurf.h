#pragma once
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Eigen/Dense>

namespace FitConstrainedBSplineSurf
{
    void FitOffsetSurface(const std::vector<Eigen::Vector3d>& offsets, const std::vector<double>& pntParamsU, const std::vector<double>& pntParamsV,
        const std::vector<double>& stdUKnots, const std::vector<double>& stdVKnots, std::vector<double> uIsoparamParams, std::vector<double> vIsoparamParams, int uDegree, int vDegree, int numCtrlPtsU, int numCtrlPtsV,
        std::vector<Eigen::Vector3d>& controlPoints);

    void BuildMatrixN(const std::vector<double>& uParams, const std::vector<double>& vParams, const std::vector<double>& stdUKnots, const std::vector<double>& stdVKnots,
        int uDegree, int vDegree, int numCtrlPtsU, int numCtrlPtsV, Eigen::MatrixXd& N);

    void BuildMatrixU(const std::vector<double>& uKnots, int uDegree, const std::vector<double>& uParams, int numCtrlPtsU, int numCtrlPtsV, Eigen::MatrixXd& U);

    void BuildMatrixV(const std::vector<double>& vKnots, int vDegree, const std::vector<double>& vParams, int numCtrlPtsU, int numCtrlPtsV, Eigen::MatrixXd& V);

    void BuildMatrixB(int numCtrlPtsU, int numCtrlPtsV, Eigen::MatrixXd& B);

    void BuildMatrixW(const std::vector<double>& pntParamsU, const std::vector<double>& pntParamsV, const std::vector<double>& stdUKnots, const std::vector<double>& stdVKnots,
        int uDegree, int vDegree, int numCtrlPtsU, int numCtrlPtsV, Eigen::MatrixXd& W);

    double BSplineBasisFunction(int j, int d, double u, const std::vector<double>& knots);
};

