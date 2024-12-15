#pragma once

//! 缝隙临边edge对
struct EdgePair {

    //int id;//序列号没啥用，不进索引
    TopoDS_Edge edge1;
    TopoDS_Edge edge2;
    Standard_Real myHausdorffDistances;

    EdgePair() {};
    EdgePair(const TopoDS_Edge& edge1, const TopoDS_Edge& edge2,
        Standard_Real& HausdorffDistances) :
        edge1(edge1), edge2(edge2), myHausdorffDistances(HausdorffDistances)
    {}
};