#pragma once
#include "lib3d\object3d.h"
#include "disline.h"
#include "lib3d\Transform.h"
class CDisline2D : public CObject3d
{
public:
	CDisline2D(void);
	~CDisline2D(void);
	 C_Disline2D* pDisline2D;
	 CTransform	 m_Transform;
	 int glDraw();
	 int m_Show;
	 double c_r,c_g,c_b;
};
