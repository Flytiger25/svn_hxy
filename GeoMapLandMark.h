#pragma once

class GeoMapLandMark
{
public:
	std::vector<int> landmark_points;
	int style;
	int deleteflag;
	int fixedflag;
	int isAuxillary;
	int isVisible;
	int flag;
	GeoMapLandMark(void);
	~GeoMapLandMark(void);

	//is equal to another landmark
	int IsEqual(GeoMapLandMark* pLandmark);

	//
	int IsPointOn(int vindex);
};
