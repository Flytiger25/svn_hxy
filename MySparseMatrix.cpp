#include "stdafx.h"
#include "MySparseMatrix.h"

MySparseMatrix::MySparseMatrix(void)
{
}

MySparseMatrix::~MySparseMatrix(void)
{
}
void MySparseMatrix::Clear()
{
	m_Array.clear();
}
void MySparseMatrix::SetDimension(int m1,int n1)
{
	m = m1;
	n = n1;
}
void MySparseMatrix::IncreaseItem(int i,int j, double val)
{
	std::map<std::pair<int,int>,double>::iterator it;
	it = m_Array.find(std::pair<int,int>(i,j));
	if(it != m_Array.end())
	{
		it->second += val;
	}else
	{
		AddItem(i,j,val);
	}
	/*int ii;
	for(ii = 0; ii < m_Array.size(); ii++)
	{
		if(i == m_Array[ii].i && j == m_Array[ii].j)
		{
			break;
		}
	}
	if(ii < m_Array.size())
	{
		m_Array[ii].val += val;
	}else
	{
		AddItem(i,j,val);
	}*/
}
void MySparseMatrix::AddItem(int i,int j, double val)
{
	m_Array.insert(std::pair<std::pair<int,int>,double>(std::pair<int,int>(i,j),val));
	//SparseMatrixItem item;
	//item.i = i;
	//item.j = j;
	//item.val = val;
	//m_Array.push_back(item);
}