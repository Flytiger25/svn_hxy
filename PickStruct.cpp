#include "stdafx.h"
#include "PickStruct.h"
#include "Lib3D/Base3d.h"
PickStruct::PickStruct()
{
	m_SelectIndex = -1;
	mindepth = MAX_DOUBLE;
}