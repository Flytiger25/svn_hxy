#include "stdafx.h"
#include <iostream>

using namespace std;

#include "kd tree.h"

int _tmain(int argc, _TCHAR* argv[])
{
	kd_tree<3> Tree;
	kd_tree<3>::kd_point Point = { 0.5f, 0.5f, 0.5f }, nearPoint,
			MinCorner = { 0.0f, 0.0f, 0.0f }, MaxCorner = { 2.0f, 2.0f, 2.0f };
	vector<kd_tree<3>::kd_point> Points, nearPoints;

	// Create a tree

	Points.push_back(kd_tree<3>::kd_point({ 0.0f, 0.0f, 0.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 0.0f, 0.0f, 1.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 0.0f, 1.0f, 0.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 0.0f, 1.0f, 1.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 1.0f, 0.0f, 0.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 1.0f, 0.0f, 1.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 1.0f, 1.0f, 0.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 1.0f, 1.0f, 1.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 0.0f, 0.0f, 2.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 0.0f, 2.0f, 0.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 0.0f, 2.0f, 2.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 2.0f, 0.0f, 0.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 2.0f, 0.0f, 2.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 2.0f, 2.0f, 0.0f }));
	Points.push_back(kd_tree<3>::kd_point({ 2.0f, 2.0f, 2.0f }));

	Tree.insert(Points);

	// Call tree height & nodes count methods

	cout << "Tree height is " << Tree.TreeHeight() << endl;
	cout << "There are " << Tree.nodeCount() << " points in the tree." << endl;

	// Use an iterator to print all points in tree

	cout << endl;
	cout << "Points in cloud are:" << endl;

	for (kd_tree<3>::iterator Iter = Tree.begin(); Iter != Tree.end(); Iter++)
	{
		kd_tree<3>::kd_point iterPoint = *Iter;
		cout << iterPoint[0] << " " << iterPoint[1] << " " << iterPoint[2] << endl;
	}
	cout << endl;

	Tree.nearestNeighbor(Point, nearPoint);

	cout << "Closest point to ("
		<< Point[0] << "," << Point[1] << "," << Point[2] << ") is ("
		<< nearPoint[0] << "," << nearPoint[1] << "," << nearPoint[2] << ")" << endl;

	Tree.KNearestNeighbors(Point, nearPoints, 8);

	cout << endl;
	cout << "8 closest point to ("
		<< Point[0] << "," << Point[1] << "," << Point[2] << ") are\n";

	for (unsigned i = 0; i < nearPoints.size(); i++)
		cout << " (" << nearPoints[i][0] << "," << nearPoints[i][1] << "," << nearPoints[i][2] << ")" << endl;

	Tree.pointsInBox(MinCorner, MaxCorner, nearPoints);

	cout << endl;
	cout << "Points in bounding box ";
	cout << " (" << MinCorner[0] << "," << MinCorner[1] << "," << MinCorner[2] << ") ->";
	cout << " (" << MaxCorner[0] << "," << MaxCorner[1] << "," << MaxCorner[2] << ") are\n";

	for (unsigned i = 0; i < nearPoints.size(); i++)
		cout << " (" << nearPoints[i][0] << "," << nearPoints[i][1] << "," << nearPoints[i][2] << ")" << endl;

	cout << endl;
	cout << "Tree dump is" << endl;
	Tree.PrintTree();

	return 0;
}
