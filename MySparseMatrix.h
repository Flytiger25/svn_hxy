#pragma once
#include <map>
class SparseMatrixItem
{
public:
	int i,j;
	double val;
};
class MySparseMatrix
{
public:
	int m,n;
	//std::vector<SparseMatrixItem> m_Array;
	std::map<std::pair<int,int>,double> m_Array;
	void IncreaseItem(int i,int j, double val);
	void AddItem(int i,int j, double val);
	void SetDimension(int m1,int n1);
	void Clear();
	MySparseMatrix(void);
	~MySparseMatrix(void);
};
