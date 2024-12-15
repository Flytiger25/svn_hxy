#pragma once
#include <unordered_map>
#include <Standard.hxx>
#include <Standard_Type.hxx>
#include <vector>
#include <TopTools_IndexedDataMapOfShapeShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_DataMapOfShapeShape.hxx>
#include <Standard_Integer.hxx>
#include <TopTools_DataMapOfShapeListOfShape.hxx>
#include <TopTools_MapOfShape.hxx>
#include <Standard_Transient.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_SequenceOfShape.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <TColStd_SequenceOfBoolean.hxx>
#include <TColStd_SequenceOfInteger.hxx>
#include <TColStd_Array1OfBoolean.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_SequenceOfReal.hxx>

#include <Message_ProgressRange.hxx>
#include "EdgePair.h"

class BRepTools_ReShape;
class TopoDS_Edge;
class TopoDS_Face;
class Geom_Surface;
class TopLoc_Location;
class Geom2d_Curve;
class Geom_Curve;


class BRepSewing;
DEFINE_STANDARD_HANDLE(BRepSewing, Standard_Transient)

class BRepSewing : public Standard_Transient
{

public:

	Standard_EXPORT BRepSewing(const Standard_Real tolerance = 1.0e-06, const Standard_Boolean option1 = Standard_True, const Standard_Boolean option2 = Standard_True, const Standard_Boolean option3 = Standard_True, const Standard_Boolean option4 = Standard_False);

	//! initialize the parameters if necessary
	Standard_EXPORT void Init(const Standard_Real tolerance = 1.0e-06, const Standard_Boolean option1 = Standard_True, const Standard_Boolean option2 = Standard_True, const Standard_Boolean option3 = Standard_True, const Standard_Boolean option4 = Standard_False);

	//! Loades the context shape.
	Standard_EXPORT void Load(const TopoDS_Shape& shape);

	//! Defines the shapes to be sewed or controlled
	Standard_EXPORT void Add(const TopoDS_Shape& shape);

	//! Computing
	//! theProgress - progress indicator of algorithm
	Standard_EXPORT void Perform(const Message_ProgressRange& theProgress = Message_ProgressRange());

	//! Gives the sewed shape
	//! a null shape if nothing constructed
	//! may be a face, a shell, a solid or a compound
	Standard_EXPORT const TopoDS_Shape& SewedShape() const;

	//! set context
	Standard_EXPORT void SetContext(const Handle(BRepTools_ReShape)& theContext);

	//! return context
	Standard_EXPORT const Handle(BRepTools_ReShape)& GetContext() const;

	//! Gives the number of free edges (edge shared by one face)
	Standard_EXPORT Standard_Integer NbFreeEdges() const;

	//! Gives each free edge
	Standard_EXPORT const TopoDS_Edge& FreeEdge(const Standard_Integer index) const;

	//! Gives the number of multiple edges
	//! (edge shared by more than two faces)
	Standard_EXPORT Standard_Integer NbMultipleEdges() const;

	//! Gives each multiple edge
	Standard_EXPORT const TopoDS_Edge& MultipleEdge(const Standard_Integer index) const;

	//! Gives the number of contiguous edges (edge shared by two faces)
	Standard_EXPORT Standard_Integer NbContigousEdges() const;

	//! Gives each contiguous edge
	Standard_EXPORT const TopoDS_Edge& ContigousEdge(const Standard_Integer index) const;

	//! Gives the sections (edge) belonging to a contiguous edge
	Standard_EXPORT const TopTools_ListOfShape& ContigousEdgeCouple(const Standard_Integer index) const;

	//! Indicates if a section is bound (before use SectionToBoundary)
	Standard_EXPORT Standard_Boolean IsSectionBound(const TopoDS_Edge& section) const;

	//! Gives the original edge (free boundary) which becomes the
	//! the section. Remember that sections constitute  common edges.
	//! This imformation is important for control because with
	//! original edge we can find the surface to which the section
	//! is attached.
	Standard_EXPORT const TopoDS_Edge& SectionToBoundary(const TopoDS_Edge& section) const;

	//! Gives the number of degenerated shapes
	Standard_EXPORT Standard_Integer NbDegeneratedShapes() const;

	//! Gives each degenerated shape
	Standard_EXPORT const TopoDS_Shape& DegeneratedShape(const Standard_Integer index) const;

	//! Indicates if a input shape is degenerated
	Standard_EXPORT Standard_Boolean IsDegenerated(const TopoDS_Shape& shape) const;

	//! Indicates if a input shape has been modified
	Standard_EXPORT Standard_Boolean IsModified(const TopoDS_Shape& shape) const;

	//! Gives a modifieded shape
	Standard_EXPORT const TopoDS_Shape& Modified(const TopoDS_Shape& shape) const;

	//! Indicates if a input subshape has been modified
	Standard_EXPORT Standard_Boolean IsModifiedSubShape(const TopoDS_Shape& shape) const;

	//! Gives a modifieded subshape
	Standard_EXPORT TopoDS_Shape ModifiedSubShape(const TopoDS_Shape& shape) const;

	//! print the information
	Standard_EXPORT void Dump() const;

	//! Gives the number of deleted faces (faces smallest than tolerance)
	Standard_EXPORT Standard_Integer NbDeletedFaces() const;

	//! Gives each deleted face
	Standard_EXPORT const TopoDS_Face& DeletedFace(const Standard_Integer index) const;

	//! Gives a modified shape
	Standard_EXPORT TopoDS_Face WhichFace(const TopoDS_Edge& theEdg, const Standard_Integer index = 1) const;

	//! Gets same parameter mode.
	Standard_Boolean SameParameterMode() const;

	//! Sets same parameter mode.
	void SetSameParameterMode(const Standard_Boolean SameParameterMode);

	//! Gives set tolerance.
	Standard_Real Tolerance() const;

	//! Sets tolerance
	void SetTolerance(const Standard_Real theToler);

	//! Gives set min tolerance.
	Standard_Real MinTolerance() const;

	//! Sets min tolerance
	void SetMinTolerance(const Standard_Real theMinToler);

	//! Gives set max tolerance
	Standard_Real MaxTolerance() const;

	//! Sets max tolerance.
	void SetMaxTolerance(const Standard_Real theMaxToler);

	//! Returns mode for sewing faces By default - true.
	Standard_Boolean FaceMode() const;

	//! Sets mode for sewing faces By default - true.
	void SetFaceMode(const Standard_Boolean theFaceMode);

	//! Returns mode for sewing floating edges By default - false.
	Standard_Boolean FloatingEdgesMode() const;

	//! Sets mode for sewing floating edges By default - false.
	//! Returns mode for cutting floating edges By default - false.
	//! Sets mode for cutting floating edges By default - false.
	void SetFloatingEdgesMode(const Standard_Boolean theFloatingEdgesMode);

	//! Returns mode for accounting of local tolerances
	//! of edges and vertices during of merging.
	Standard_Boolean LocalTolerancesMode() const;

	//! Sets mode for accounting of local tolerances
	//! of edges and vertices during of merging
	//! in this case WorkTolerance = myTolerance + tolEdge1+ tolEdg2;
	void SetLocalTolerancesMode(const Standard_Boolean theLocalTolerancesMode);

	//! Sets mode for non-manifold sewing.
	void SetNonManifoldMode(const Standard_Boolean theNonManifoldMode);

	//! Gets mode for non-manifold sewing.
	//!
	//! INTERNAL FUNCTIONS ---
	Standard_Boolean NonManifoldMode() const;



	DEFINE_STANDARD_RTTIEXT(BRepSewing, Standard_Transient)

protected:


	//! Performs cutting of sections
	//! theProgress - progress indicator of processing
	Standard_EXPORT void Cutting(const Message_ProgressRange& theProgress = Message_ProgressRange());

	Standard_EXPORT void Merging(const Standard_Boolean passage, const Message_ProgressRange& theProgress = Message_ProgressRange());

	Standard_EXPORT Standard_Boolean IsMergedClosed(const TopoDS_Edge& Edge1, const TopoDS_Edge& Edge2, const TopoDS_Face& fase) const;

	Standard_EXPORT Standard_Boolean FindCandidates(TopTools_SequenceOfShape& seqSections, TColStd_IndexedMapOfInteger& mapReference, TColStd_SequenceOfInteger& seqCandidates, TColStd_SequenceOfBoolean& seqOrientations);

	Standard_EXPORT void AnalysisNearestEdges(const TopTools_SequenceOfShape& sequenceSec, TColStd_SequenceOfInteger& seqIndCandidate, TColStd_SequenceOfBoolean& seqOrientations, const Standard_Boolean evalDist = Standard_True);

	//! Merged nearest edges.
	Standard_EXPORT Standard_Boolean MergedNearestEdges(const TopoDS_Shape& edge, TopTools_SequenceOfShape& SeqMergedEdge, TColStd_SequenceOfBoolean& SeqMergedOri);

	Standard_EXPORT void EdgeProcessing(const Message_ProgressRange& theProgress = Message_ProgressRange());

	//! Recompute regularity on merged edges
	Standard_EXPORT void EdgeRegularity(const Message_ProgressRange& theProgress = Message_ProgressRange());

	Standard_EXPORT void CreateOutputInformations();

	//! Defines if surface is U closed.
	Standard_EXPORT virtual Standard_Boolean IsUClosedSurface(const Handle(Geom_Surface)& surf, const TopoDS_Shape& theEdge, const TopLoc_Location& theloc) const;

	//! Defines if surface is V closed.
	Standard_EXPORT virtual Standard_Boolean IsVClosedSurface(const Handle(Geom_Surface)& surf, const TopoDS_Shape& theEdge, const TopLoc_Location& theloc) const;


	//! This method is called from Perform only
	//! theProgress - progress indicator of processing
	Standard_EXPORT virtual void FaceAnalysis(const Message_ProgressRange& theProgress = Message_ProgressRange());


	//! This method is called from Perform only
	Standard_EXPORT virtual void FindFreeBoundaries();


	//! This method is called from Perform only
	//! theProgress - progress indicator of processing
	Standard_EXPORT virtual void VerticesAssembling(const Message_ProgressRange& theProgress = Message_ProgressRange());


	//! This method is called from Perform only
	Standard_EXPORT virtual void CreateSewedShape();

	//! Get wire from free edges.
	//! This method is called from EdgeProcessing only
	Standard_EXPORT virtual void GetFreeWires(TopTools_IndexedMapOfShape& MapFreeEdges,
		TopTools_SequenceOfShape& seqWires);


	//! This method is called from MergingOfSections only
	Standard_EXPORT virtual void EvaluateAngulars(TopTools_SequenceOfShape& sequenceSec, TColStd_Array1OfBoolean& secForward, TColStd_Array1OfReal& tabAng, const Standard_Integer indRef) const;


	//! This method is called from MergingOfSections only
	Standard_EXPORT virtual void EvaluateDistances(TopTools_SequenceOfShape& sequenceSec, TColStd_Array1OfBoolean& secForward, TColStd_Array1OfReal& tabAng, TColStd_Array1OfReal& arrLen, TColStd_Array1OfReal& tabMinDist, const Standard_Integer indRef) const;

	//修改代码20240604
	void EvaluateEdgeDistancesModified(const TopoDS_Edge& edge1, const TopoDS_Edge& edge2, Standard_Real& maxOfMinDistances, Standard_Real& minOfMinDistances);


	//! This method is called from SameParameterEdge only
	Standard_EXPORT virtual Handle(Geom2d_Curve) SameRange(const Handle(Geom2d_Curve)& CurvePtr, const Standard_Real FirstOnCurve, const Standard_Real LastOnCurve, const Standard_Real RequestedFirst, const Standard_Real RequestedLast) const;


	//! This method is called from SameParameterEdge only
	Standard_EXPORT virtual void SameParameter(const TopoDS_Edge& edge) const;


	//! This method is called from Merging only
	Standard_EXPORT virtual TopoDS_Edge SameParameterEdge(const TopoDS_Shape& edge, const TopTools_SequenceOfShape& seqEdges, const TColStd_SequenceOfBoolean& seqForward, TopTools_MapOfShape& mapMerged, const Handle(BRepTools_ReShape)& locReShape);


	//! This method is called from Merging only
	Standard_EXPORT virtual TopoDS_Edge SameParameterEdge(const TopoDS_Edge& edge1, const TopoDS_Edge& edge2, const TopTools_ListOfShape& listFaces1, const TopTools_ListOfShape& listFaces2, const Standard_Boolean secForward, Standard_Integer& whichSec, const Standard_Boolean firstCall = Standard_True);

	//! Projects points on curve
	//! This method is called from Cutting only
	Standard_EXPORT void ProjectPointsOnCurve(const TColgp_Array1OfPnt& arrPnt, const Handle(Geom_Curve)& Crv, const Standard_Real first, const Standard_Real last, TColStd_Array1OfReal& arrDist, TColStd_Array1OfReal& arrPara, TColgp_Array1OfPnt& arrProj, const Standard_Boolean isConsiderEnds) const;

	//! Creates cutting vertices on projections
	//! This method is called from Cutting only
	Standard_EXPORT virtual void CreateCuttingNodes(const TopTools_IndexedMapOfShape& MapVert, const TopoDS_Shape& bound, const TopoDS_Shape& vfirst, const TopoDS_Shape& vlast, const TColStd_Array1OfReal& arrDist, const TColStd_Array1OfReal& arrPara, const TColgp_Array1OfPnt& arrPnt, TopTools_SequenceOfShape& seqNode, TColStd_SequenceOfReal& seqPara);

	//! Performs cutting of bound
	//! This method is called from Cutting only
	Standard_EXPORT virtual void CreateSections(const TopoDS_Shape& bound, const TopTools_SequenceOfShape& seqNode, const TColStd_SequenceOfReal& seqPara, TopTools_ListOfShape& listEdge);

	//! Makes all edges from shape same parameter
	//! if SameParameterMode is equal to Standard_True
	//! This method is called from Perform only
	Standard_EXPORT virtual void SameParameterShape();

	Standard_Real myTolerance;
	Standard_Boolean mySewing;
	Standard_Boolean myAnalysis;
	Standard_Boolean myCutting;
	Standard_Boolean myNonmanifold;
	TopTools_IndexedDataMapOfShapeShape myOldShapes;
	TopoDS_Shape mySewedShape;
	TopTools_IndexedMapOfShape myDegenerated;
	TopTools_IndexedMapOfShape myFreeEdges;
	TopTools_IndexedMapOfShape myMultipleEdges;
	TopTools_IndexedDataMapOfShapeListOfShape myContigousEdges;
	TopTools_DataMapOfShapeShape myContigSecBound;
	Standard_Integer myNbShapes;
	Standard_Integer myNbVertices;
	Standard_Integer myNbEdges;
	TopTools_IndexedDataMapOfShapeListOfShape myBoundFaces;//Bound与Faces索引map序列
	TopTools_DataMapOfShapeListOfShape myBoundSections;//如果需要cut,Bound与Sections索引map序列
	TopTools_DataMapOfShapeShape mySectionBound;//如果需要cut,每一节Section与对应的Bound索引map序列
	TopTools_IndexedDataMapOfShapeShape myVertexNode; //自由边界VertexNode序列
	TopTools_IndexedDataMapOfShapeShape myVertexNodeFree;//浮动边界VertexNode序列
	TopTools_DataMapOfShapeListOfShape myNodeSections;//node-edges索引map序列
	TopTools_DataMapOfShapeListOfShape myCuttingNode;
	TopTools_IndexedMapOfShape myLittleFace;
	TopoDS_Shape myShape;
	Handle(BRepTools_ReShape) myReShape;

private:


	Standard_Boolean myFaceMode;
	Standard_Boolean myFloatingEdgesMode;
	Standard_Boolean mySameParameterMode;
	Standard_Boolean myLocalToleranceMode;
	Standard_Real myMinTolerance;
	Standard_Real myMaxTolerance;
	TopTools_MapOfShape myMergedEdges;//合并边的set

	// 修改2024.06.04
public:

	TopTools_IndexedDataMapOfShapeShape myOldEdgeNewEdge; //old边查找NewEdge的map
	TopTools_DataMapOfShapeListOfShape myNewEdgeOldEdges;//合并的new边OldEdges的map
	std::vector<EdgePair> myEdgePairs;//旧边序列

};
