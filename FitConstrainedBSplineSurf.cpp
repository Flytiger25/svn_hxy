#include "stdafx.h"
#include "stdafx.h"
#include "GuideGordon.h"
#include "Interpolate.h"
#include <Eigen/Dense>

#include <GeomAPI_ProjectPointOnSurf.hxx>
#include "FitConstrainedBSplineSurf.h"


// 偏移曲面拟合
void FitConstrainedBSplineSurf::FitOffsetSurface(const std::vector<Eigen::Vector3d>& offsets, const std::vector<double>& pntParamsU, const std::vector<double>& pntParamsV,
    const std::vector<double>& stdUKnots, const std::vector<double>& stdVKnots, std::vector<double> uIsoparamParams, std::vector<double> vIsoparamParams, int uDegree, int vDegree, int numCtrlPtsU, int numCtrlPtsV,
    std::vector<Eigen::Vector3d>& controlPoints) {

    // 构建U矩阵
    Eigen::MatrixXd U;
    BuildMatrixU(stdUKnots, uDegree, uIsoparamParams, numCtrlPtsU, numCtrlPtsV, U);
    //std::cout << U << std::endl;

    // 构建V矩阵
    Eigen::MatrixXd V;
    BuildMatrixV(stdVKnots, vDegree, vIsoparamParams, numCtrlPtsU, numCtrlPtsV, V);
    //std::cout << V << std::endl;

    // 构建B矩阵
    Eigen::MatrixXd B;
    BuildMatrixB(numCtrlPtsU, numCtrlPtsV, B);
    //std::cout << B << std::endl;

    // 构建M矩阵
    Eigen::MatrixXd M = Eigen::MatrixXd::Zero(U.rows() + V.rows() + B.rows(), numCtrlPtsU * numCtrlPtsV);
    M.block(0, 0, U.rows(), numCtrlPtsU * numCtrlPtsV) = U;
    M.block(U.rows(), 0, V.rows(), numCtrlPtsU * numCtrlPtsV) = V;
    M.block(U.rows() + V.rows(), 0, B.rows(), numCtrlPtsU * numCtrlPtsV) = B;

    //M可能不满秩，需要删除某行
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(M);
    int MRank = svd.rank();
    if (M.rows() != MRank)
    {
        std::cout << "M不满秩" << std::endl;
        std::cout << "M rank: " << MRank << std::endl;
        std::cout << "M rows: " << M.rows() << " M cols: " << M.cols() << std::endl;
        Eigen::MatrixXd fullM;
        fullM = M;
        while (fullM.rows() > MRank)
        {
            fullM = BuildFullRankMatrix(fullM);
        }
        M = fullM;
    }

    // 构建W矩阵（暂时不用）
    //Eigen::MatrixXd W;
    //BuildMatrixW(pntParamsU, pntParamsV, stdUKnots, stdVKnots, uDegree, vDegree, numCtrlPtsU, numCtrlPtsV, W);
    //std::cout << W << std::endl;

    // 计算N矩阵
    Eigen::MatrixXd N;
    BuildMatrixN(pntParamsU, pntParamsV, stdUKnots, stdVKnots, uDegree, vDegree, numCtrlPtsU, numCtrlPtsV, N);
    //std::cout << N << std::endl;

    // 构建O矩阵
    Eigen::MatrixXd O = Eigen::MatrixXd::Zero(N.rows(), 3);
    for (int i = O.rows() - offsets.size(); i < O.rows(); ++i) {
        O(i, 0) = offsets[i - O.rows() + offsets.size()](0);
        O(i, 1) = offsets[i - O.rows() + offsets.size()](1);
        O(i, 2) = offsets[i - O.rows() + offsets.size()](2);
    }
    //std::cout << O << std::endl;

    Eigen::MatrixXd NT = N.transpose();
    //std::cout << NT << std::endl;

    // 计算N^T * W * N
    Eigen::MatrixXd NTN = N.transpose() * N;
    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(NTN.rows(), NTN.cols());
    Eigen::MatrixXd NTNI = NTN.llt().solve(I);

    //Eigen::MatrixXd NTWN = N.transpose() * N;
    //std::cout << NTN << std::endl;

    // 计算M * (N^T * W * N)^-1 * M^T
    Eigen::MatrixXd MInvNN = M * NTNI * M.transpose();
    Eigen::FullPivLU<Eigen::MatrixXd> lu(MInvNN);
    if (lu.isInvertible()) {
        std::cout << "MInvNN矩阵可逆" << std::endl;
    }
    else {
        std::cout << "MInvNN矩阵不可逆" << std::endl;
    }

    // 计算拉格朗日乘子矩阵Lambda
    Eigen::MatrixXd Lambda = MInvNN.inverse() * M * NTNI * N.transpose() * O;
    //Eigen::MatrixXd Lambda = MInvNWN.inverse() * M * NTWN.inverse() * N.transpose() * O;

    // 计算控制顶点矩阵P
    Eigen::MatrixXd P = NTNI * (N.transpose() * O - M.transpose() * Lambda);
    //Eigen::MatrixXd P = NTWN.inverse() * (N.transpose() * O - M.transpose() * Lambda);
    //std::cout << P << std::endl;

    // 将P转换为eigen的vector类型
    for (int i = 0; i < numCtrlPtsU * numCtrlPtsV; i++) {
        controlPoints[i] = Eigen::Vector3d(P(i, 0), P(i, 1), P(i, 2));
    }
}

// 构建 U 矩阵
void FitConstrainedBSplineSurf::BuildMatrixU(const std::vector<double>& uKnots, int uDegree, const std::vector<double>& uParams, int numCtrlPtsU, int numCtrlPtsV, Eigen::MatrixXd& U)
{
    // 计算矩阵 U 的尺寸
    int numParams = uParams.size(); // u等参线的数量n_0+1
    int numRows = (numCtrlPtsV - 2) * numParams; // 行数为(m-1)(n_0+1)
    int numCols = numCtrlPtsU * numCtrlPtsV; // 列数为控制点数量 (m+1)(n+1)

    // 初始化 U 矩阵
    U = Eigen::MatrixXd::Zero(numRows, numCols);

    // 遍历每个参数值
    for (int paramIdx = 0; paramIdx < numParams; ++paramIdx) {
        
        double param = uParams[paramIdx]; // 当前等参线参数值
        
        // 遍历 u 方向上的每个控制点索引
        for (int i = 0; i < numCtrlPtsU; ++i) {
            // 计算 B 样条基函数值
            double Ni = InterPolateTool::OneBasicFun(param, i, uDegree, uKnots);

            // 设置矩阵 U 中对应的块值
            Eigen::MatrixXd B = Eigen::MatrixXd::Zero(numCtrlPtsV - 2, numCtrlPtsV);
            for (int j = 0; j < numCtrlPtsV - 2; j++)
            {
                B(j, j + 1) = Ni;
            }

            // 将块添加到矩阵 U 中
            int rowstart = (numCtrlPtsV - 2) * paramIdx;
            int colstart = numCtrlPtsV * i;
            int blockrows = numCtrlPtsV - 2;
            int blockcols = numCtrlPtsV;
            U.block(rowstart, colstart, blockrows, blockcols) = B;
        }
    }

    //std::cout << "矩阵U：" << std::endl;
    //Eigen::IOFormat HeavyFmt(Eigen::FullPrecision, 0, ", ", ",\n", "[", "]", "[", "]");
    //std::cout << U.format(HeavyFmt) << std::endl;
}

// 构建 V 矩阵
void FitConstrainedBSplineSurf::BuildMatrixV(const std::vector<double>& vKnots, int vDegree, const std::vector<double>& vParams, int numCtrlPtsU, int numCtrlPtsV, Eigen::MatrixXd& V)
{
    // 计算矩阵 V 的尺寸
    int numParams = vParams.size(); // v等参线的数量m_0+1
    int numRows = (numCtrlPtsU - 2) * numParams; // 行数为(n-1)(m_0+1)
    int numCols = numCtrlPtsU * numCtrlPtsV; // 列数为控制点数量 (m+1)(n+1)

    // 初始化 V 矩阵
    V = Eigen::MatrixXd::Zero(numRows, numCols);

    // 初始化基函数矩阵 N
    Eigen::MatrixXd N = Eigen::MatrixXd::Zero(numParams, numCtrlPtsV);

    // 遍历每个参数值
    for (int paramIdx = 0; paramIdx < numParams; ++paramIdx) {
        double param = vParams[paramIdx]; // 当前参数值

        // 遍历 v 方向上的每个控制点索引
        for (int i = 0; i < numCtrlPtsV; ++i) {
            // 计算 B 样条基函数值
            double Ni = InterPolateTool::OneBasicFun(param, i, vDegree, vKnots);
            N(paramIdx, i) = Ni;
        }
    }

    for (int j = 0; j < numCtrlPtsU - 2; j++)
    {
        // 将块 N 添加到矩阵 V 中
        int rowstart = numParams * j;
        int colstart = numCtrlPtsV + numCtrlPtsV * j;
        int blockrows = numParams;
        int blockcols = numCtrlPtsV;
        V.block(rowstart, colstart, blockrows, blockcols) = N;
    }

    //std::cout << "矩阵V：" << std::endl;
    //Eigen::IOFormat HeavyFmt(Eigen::FullPrecision, 0, ", ", ",\n", "[", "]", "[", "]");
    //std::cout << V.format(HeavyFmt) << std::endl;
}

// 构建 B 矩阵
void FitConstrainedBSplineSurf::BuildMatrixB(int numCtrlPtsU, int numCtrlPtsV, Eigen::MatrixXd& B) {
    // 计算矩阵 B 的尺寸
    int numRows = 2 * (numCtrlPtsU + numCtrlPtsV - 2);
    int numCols = numCtrlPtsU * numCtrlPtsV;

    // 初始化矩阵 B
    B = Eigen::MatrixXd::Zero(numRows, numCols);

    // 定义n+1阶单位矩阵 I
    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(numCtrlPtsV, numCtrlPtsV);

    // 定义J
    // 1 0 · · · 0 0
    // 0 0 · · · 0 1
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(2, numCtrlPtsV);
    J(0, 0) = 1;
    J(1, numCtrlPtsV - 1) = 1;

    // 填充矩阵 B 中 I 的左上部分
    B.block(0, 0, numCtrlPtsV, numCtrlPtsV) = I;
    // 填充矩阵 B 中 I 的右下部分
    int rowstart = numCtrlPtsV + 2 * numCtrlPtsU - 4;
    int colstart = (numCtrlPtsU - 1) * numCtrlPtsV;
    int blockrows = numCtrlPtsV;
    int blockcols = numCtrlPtsV;
    B.block(rowstart, colstart, blockrows, blockcols) = I;

    // 填充矩阵 B 中 J 的部分
    for (int i = 0; i < numCtrlPtsU - 2; i++)
    {
        int rowstart = numCtrlPtsV + i * 2;
        int colstart = numCtrlPtsV + i * numCtrlPtsV;
        int blockrows = 2;
        int blockcols = numCtrlPtsV;
        B.block(rowstart, colstart, blockrows, blockcols) = J;
    }

    //std::cout << "矩阵B：" << std::endl;
    //Eigen::IOFormat HeavyFmt(Eigen::FullPrecision, 0, ", ", ",\n", "[", "]", "[", "]");
    //std::cout << B.format(HeavyFmt) << std::endl;
}

// 构建 N 矩阵
void FitConstrainedBSplineSurf::BuildMatrixN(const std::vector<double>& uParams, const std::vector<double>& vParams, const std::vector<double>& stdUKnots, const std::vector<double>& stdVKnots,
    int uDegree, int vDegree, int numCtrlPtsU, int numCtrlPtsV, Eigen::MatrixXd& N) {
    // 构造矩阵 N^
    Eigen::MatrixXd N_hat;
    Eigen::MatrixXd Ni(uParams.size(), numCtrlPtsU);
    Eigen::MatrixXd Nj(vParams.size(), numCtrlPtsV);
    double valueTemp = 0.0;
    for (size_t i = 0; i < uParams.size(); i++) {
        for (size_t j = 0; j < numCtrlPtsU; j++) {
            valueTemp = InterPolateTool::OneBasicFun(uParams[i], j, uDegree, stdUKnots);
            Ni(i, j) = valueTemp;
        }
        for (size_t j = 0; j < numCtrlPtsV; j++) {
            valueTemp = InterPolateTool::OneBasicFun(vParams[i], j, vDegree, stdVKnots);
            Nj(i, j) = valueTemp;
        }
    }
    N_hat = InterPolateTool::composeTensowMat(Ni, Nj);

    // 定义单位矩阵 I 
    // 12.19 alpha改为0.01，单位矩阵里的1改为0.1
    //Eigen::MatrixXd I = Eigen::MatrixXd::Identity(N_hat.cols(), N_hat.cols());
    Eigen::MatrixXd I = Eigen::MatrixXd::Zero(N_hat.cols(), N_hat.cols());
    for (int i = 0; i < N_hat.cols(); i++)
    {
        I(i, i) = 0.01;
    }

    // 构造矩阵 N
    Eigen::MatrixXd res = Eigen::MatrixXd::Zero(I.rows() + N_hat.rows(), I.cols());
    res.block(0, 0, I.rows(), I.cols()) = I;
    res.block(I.rows(), 0, N_hat.rows(), N_hat.cols()) = N_hat;
    N = res;
}

// 构建W矩阵 
// todo: 参数(u'i,v'j) 采样点的投影点参数(u^k,v^k) 区别？
void FitConstrainedBSplineSurf::BuildMatrixW(const std::vector<double>& pntParamsU, const std::vector<double>& pntParamsV, const std::vector<double>& stdUKnots, const std::vector<double>& stdVKnots,
    int uDegree, int vDegree, int numCtrlPtsU, int numCtrlPtsV, Eigen::MatrixXd& W)
{
    int numParams = pntParamsU.size();
    W = Eigen::MatrixXd::Identity(numParams, numParams);

    // 计算每个参数位置的权重
    for (int i = 0; i < numParams; i++)
    {
        double minDistanceSquared = std::numeric_limits<double>::max();
        for (int j = 0; j < stdUKnots.size() - uDegree - 1; j++)
        {
            for (int k = 0; k < stdVKnots.size() - vDegree - 1; k++)
            {
                //double u = (stdUKnots[j] + stdUKnots[j + uDegree + 1]) / 2.0;
                //double v = (stdVKnots[k] + stdVKnots[k + vDegree + 1]) / 2.0;
                double u;
                double v;
                double distanceSquared = (pntParamsU[i] - u) * (pntParamsU[i] - u) + (pntParamsV[i] - v) * (pntParamsV[i] - v);
                minDistanceSquared = std::min(minDistanceSquared, distanceSquared);
            }
        }

        // 根据最小距离平方设置权重
        if (minDistanceSquared >= 1.0)
        {
            W(i, i) = 1.0;
        }
        else
        {
            W(i, i) = minDistanceSquared;
        }
    }
}

// 删除矩阵的某行
void FitConstrainedBSplineSurf::removeRow(Eigen::MatrixXd& matrix, int rowToRemove)
{
    int numRows = matrix.rows() - 1;
    int numCols = matrix.cols();

    if (rowToRemove < numRows)
        matrix.block(rowToRemove, 0, numRows - rowToRemove, numCols) 
        = matrix.block(rowToRemove + 1, 0, numRows - rowToRemove, numCols);

    matrix.conservativeResize(numRows, numCols);
}

// 将行不满秩的矩阵改为行满秩
Eigen::MatrixXd FitConstrainedBSplineSurf::BuildFullRankMatrix(Eigen::MatrixXd& matrix)
{
    double threshold = 1e-2;

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(matrix, Eigen::ComputeFullU | Eigen::ComputeFullV);
    const Eigen::MatrixXd& U = svd.matrixU();
    const Eigen::MatrixXd& singularValues = svd.singularValues();

    std::vector<int> redundantRows;
    int numRows = matrix.rows();
    int numSingularValues = singularValues.size();

    // 遍历奇异值
    for (int i = 0; i < numSingularValues; ++i) {
        if (singularValues(i) < threshold) {
            const Eigen::VectorXd& leftSingularVector = U.col(i);
            int maxIndex = 0;
            double maxAbsValue = std::abs(leftSingularVector(0));
            // 找到左奇异向量中绝对值最大的元素对应的索引
            for (int j = 1; j < numRows; ++j) {
                double absValue = std::abs(leftSingularVector(j));
                if (absValue > maxAbsValue) {
                    maxAbsValue = absValue;
                    maxIndex = j;
                }
            }
            redundantRows.push_back(maxIndex);
        }
    }

    // 去掉重复的
    std::sort(redundantRows.begin(), redundantRows.end());
    redundantRows.erase(std::unique(redundantRows.begin(), redundantRows.end()), redundantRows.end());

    int numCols = matrix.cols();
    int newNumRows = numRows - redundantRows.size();
    Eigen::MatrixXd newMatrix(newNumRows, numCols);
    int newRow = 0;
    // 遍历原矩阵的行，跳过冗余行
    for (int i = 0; i < numRows; ++i) {

        bool isRedundant = false;
        for (int index : redundantRows) {
            if (i == index) {
                isRedundant = true;
                break;
            }
        }
        if (!isRedundant) {
            newMatrix.row(newRow++) = matrix.row(i);
        }
    }
    return newMatrix;

}
