#pragma once
class PickStruct
{
public:
	//Ray
	Point3D point;
	Vector3D dir;
	double mindepth;
	int m_SelectIndex;

	PickStruct();
};

