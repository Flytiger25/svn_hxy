#include "stdafx.h"
#include "Transformation.h"
#include <fstream>
Transformation::Transformation(void)
{
	m_ReparameterizationType = none;
	umin_Reparameterization = 0;
	umax_Reparameterization = 1;
	vmin_Reparameterization = 0;
	vmax_Reparameterization = 1;
}

Transformation::~Transformation(void)
{
}
void Transformation::Save(std::ofstream &file)
{
	file << m_ReparameterizationType << "\n";
	file << umin_Reparameterization << "\n";
	file << umax_Reparameterization << "\n";
	file << vmin_Reparameterization << "\n";
	file << vmax_Reparameterization << "\n";
	file << umin <<"\n";
	file << umax <<"\n";
	file << vmin <<"\n";
	file << vmax <<"\n";
}

void Transformation::Read(std::ifstream &file)
{
	file >> umin_Reparameterization;
	file >> umax_Reparameterization;
	file >> vmin_Reparameterization;
	file >> vmax_Reparameterization;
	file >> umin;
	file >> umax;
	file >> vmin;
	file >> vmax;
}