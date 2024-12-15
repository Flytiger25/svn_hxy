#include "stdafx.h"
#include "sdiviewswitch.h"
#include "math.h"
#include "curve.h"
#include "InterFunction.h"

void NurbscLineConvert(CS_NurbsCurve curve, CS_Line line, CS_NurbsCurve &ncurv, Point3D &cp, Vector3D &vx, Vector3D &vy, Vector3D &vz)
{
    Vector3D vz0(0,0,1), vy0(0,1,0);
    cp = line.pa;
    vx = line.vn;
    if ( Length(vx^vz0) < CS_EPS )
        vz0 = vy0;
    vy = vz0^vx;
    vz = vx^vy;
    vx.SetUnit();
    vy.SetUnit();
    vz.SetUnit();

    int i, n = ncurv.n;
    for ( i = 0; i < n; i++ )
    {
        ncurv.ctrl[i].x = (curve.ctrl[i]-cp)*vx;
        ncurv.ctrl[i].y = (curve.ctrl[i]-cp)*vy;
        ncurv.ctrl[i].z = (curve.ctrl[i]-cp)*vz;
    }
}

void GetKnotNoRepeat(int n, int &nn, double *knot, double *nknot)
{
    nknot[0] = knot[0];
    nn = 1;
    for ( int i = 1; i < n; i++ )
    {
        if ( knot[i]-knot[i-1] > TOLER )
        {
            nknot[nn] = knot[i];
            nn++;
        }
    }
}

CS_NurbsCurve NurbscCopy(CS_NurbsCurve curve)
{   
	int order,number;
	order = curve.order;
	number = curve.n;
	
	CS_NurbsCurve  Rcurve;
	Rcurve.knot = new double[number+order]; 
	Rcurve.ctrl = new Point3D[number]; 
	Rcurve.weight = new double[number]; 
	Rcurve.n = number;
	Rcurve.order = order;
	memcpy(Rcurve.knot, curve.knot, (number+order) * sizeof(double));
	memcpy(Rcurve.weight, curve.weight, number * sizeof(double));
	memcpy(Rcurve.ctrl, curve.ctrl, number * sizeof(Point3D));
/*	Revised by Yang Lei 06-03-08
    for( i = 0; i <= number+order-1; i++ )       
       	Rcurve.knot[i] = curve.knot[i];
    for( i = 0; i < number; i++ )       
       	Rcurve.weight[i] = curve.weight[i]; 
    for( i = 0; i < number; i++ )       
		Rcurve.ctrl[i] = curve.ctrl[i];
*/
	return Rcurve;
}

int NurbscIntervalIndex(CS_NurbsCurve curve, double value)
// 判别value在那个节点区间
{
	int a, b, c, ret2 = -1;
	a = curve.order - 1;
	b = curve.n;
	value -= TOLER;
	while (b - a > 1)
	{
		c = (a + b) / 2;
		if (value <= curve.knot[c])
			b = c;
		else
			a = c;
	}
	// if( (value >= curve.knot[a]-(TOLER*2)) && (value <= curve.knot[b]) )// Removed by Jun-Hai Yong 2002-11-20
		ret2 = a;
    return ret2;	// 非法值 // Removed by Jun-Hai Yong 2002-11-20
/*
	int i,order,number;
	order = curve.order;
	number = curve.n;
	for( i = order - 1; i <= number; i++ )
    	if( (value >= curve.knot[i]-TOLER) && (value <= curve.knot[i+1]+TOLER) 
    	 && (curve.knot[i+1] - curve.knot[i] > TOLER) ) //sysTolZerovalue))
    		return i; 
    return -1;	// 非法值	
*/
}
int NurbscKnotMulti(CS_NurbsCurve curve, double value) 
{
    int order, number;
    order = curve.order;
    number = curve.n; 
  
    int i,j,multiple;
    for(i = 0; i < number + order; i++)
    	if(fabs(curve.knot[i] - value) < TOLER)
    		break;
    if(i == number + order)
    	return 0;
    multiple = 1; 		
    for(j = i+1; j < number + order; j++)
	{
    	if(fabs(curve.knot[j] - value) < TOLER)
    		multiple++;
    	else
    		break;
	}

	if ( multiple == 5 )
		multiple = 5;
	return multiple;
}
int BscurvIntervalIndex(CS_BSplineCurve curve, double value)
// 判别value在那个节点区间
{
//    return(b_get_BeloPosi(curve.knot, value, curve.order - 1, curve.n));
//*
	int a, b, c, ret2 = -1;
	a = curve.order - 1;
	b = curve.n;
	value -= TOLER;
	while (b - a > 1)
	{
		c = (a + b) / 2;
		if (value <= curve.knot[c])
			b = c;
		else
			a = c;
	}
	if( (value >= curve.knot[a]-(TOLER*2)) && (value <= curve.knot[b]) )
		ret2 = a;
    else if (value<= curve.knot[curve.order - 1])
		ret2 = curve.order - 1;
    else ret2 = curve.n - 1;
    return ret2;	// 非法值
//*/
/*
	int i,order,number;
	order = curve.order;
	number = curve.n;
	for( i = order - 1; i <= number; i++ )
    	if( (value >= curve.knot[i]-TOLER) && (value <= curve.knot[i+1]+TOLER) 
    	 && (curve.knot[i+1] - curve.knot[i] > TOLER) ) //sysTolZerovalue))
    		return i; 
    return -1;	// 非法值	
*/
}

void NurbscInsertMultiKnot(CS_NurbsCurve &curve, int multiplicity, double value)
{
	int order,number;
	number = curve.n;
	order = curve.order;
	
	int      i,j,k;
   	double   *newknot, *newweight, *weight, pnewknot[100], pweight[100], pnewweight[100]; 
	Vector3D *newvect, *vect, pnewvect[100], pvect[100];
    int keynumber = number+multiplicity;
    // 申请空间
    if(keynumber+order > 100)
        newknot = new double[keynumber+order]; 
    else 
        newknot = pnewknot;
    if(keynumber > 100)
    {
        vect = new Vector3D[keynumber]; 
        newvect = new Vector3D[keynumber];
        weight = new double[keynumber];
        newweight = new double[keynumber];
    }
    else 
    {
        vect = pvect;
        newvect = pnewvect;
        weight = pweight;
        newweight = pnewweight;
    }
    // 赋初值
    for(i = 0; i < number; i++)
    	vect[i] = (curve.ctrl[i] - ZeroP3D)*curve.weight[i]; 
    for(i = 0; i < number; i++)
    	weight[i] = curve.weight[i]; 
    	
    j = NurbscIntervalIndex(curve, value);    // Value 在第j个节点区间
    // 计算新的节点向量
    for(i = 0; i < number + order + multiplicity; i++)
    {
    	if(i <= j)
    		newknot[i] = curve.knot[i];
    	else if(i <= j+multiplicity)
    	    newknot[i] = value;
    	else                         
    		newknot[i] = curve.knot[i-multiplicity];
    }
    // 插节点
    double alpha;
	for(k = 1; k <= multiplicity; k++) 
	{
	    for(i = 0; i < number + k; i++)
	    {                         
	    	if(i <= j - order + k)                                                     
	    	{
		    	newvect[i] = vect[i]; 
	            newweight[i] = weight[i];
	    	}
	    	else if(i >= j+1)
	    	{
		    	newvect[i] = vect[i-1]; 
	            newweight[i] = weight[i-1];
	    	}
	    	else                         
	     	{
	     		alpha = (value - curve.knot[i])/(curve.knot[i+order-k] - curve.knot[i]);
		    	newvect[i] = vect[i-1]*(1-alpha) + vect[i]*alpha; 
	            newweight[i] = weight[i-1]*(1-alpha) + weight[i]*alpha;
			}
	    } 
        for(i = 0; i < number + k; i++)
	    	vect[i] = newvect[i];
        for(i = 0; i < number + k; i++)
	    	weight[i] = newweight[i];
    }

    //构造新的对象
    number += multiplicity;
    curve.n = number;
    curve.order = order;
    delete [] curve.knot;
    curve.knot = new double[number+order]; 
    delete [] curve.ctrl;
    curve.ctrl = new Point3D[number]; 
	delete [] curve.weight;
    curve.weight = new double[number]; 
	for(i = 0; i < number + order; i++)
		curve.knot[i] = newknot[i];
	for(i = 0; i < number; i++)
	{
		curve.ctrl[i] = ZeroP3D + newvect[i]/newweight[i];
	    curve.weight[i] = newweight[i];
	}
    // 释放空间
    if(keynumber+order > 100)
    	delete [] newknot;
    if(keynumber > 100)
    {
        delete [] vect;                                   
	    delete [] newvect;
        delete [] weight;                              
	    delete [] newweight;
    }
}
void NurbscSplit(CS_NurbsCurve curve, double t, CS_NurbsCurve &pc1, CS_NurbsCurve &pc2)
{
    int order, number;
    order = curve.order;
    number = curve.n;
    
    int i,j,r,number1,number2;
    CS_NurbsCurve pcurve = NurbscCopy(curve);        
    t = curve.knot[order-1] + (curve.knot[number] - curve.knot[order-1])*t;
    j = NurbscIntervalIndex(curve,t);
    r = NurbscKnotMulti(curve,t);
    NurbscInsertMultiKnot(pcurve,order-r,t);
    number1 = j+1;
    number2 = number+order-r-j-1;

    pc1.knot = new double[number1+order]; 
    pc1.ctrl = new Point3D[number1]; 
    pc1.weight = new double[number1]; 
    pc2.knot = new double[number2+order]; 
    pc2.ctrl = new Point3D[number2]; 
    pc2.weight = new double[number2]; 
    
    pc1.n = number1;
    pc1.order = order;
    for(i = 0; i <= j; i++)
    	pc1.knot[i] = curve.knot[i];
    for(i = j+1; i <= number1+order-1; i++)
    	pc1.knot[i] = t;
    for(i = 0; i < number1; i++)
    {
    	pc1.ctrl[i] = pcurve.ctrl[i];
    	pc1.weight[i] = pcurve.weight[i];
    }
    pc2.n = number2;
    pc2.order = order;
    for(i = number2+order-1; i >= order; i--)
    	pc2.knot[i] = curve.knot[i+number-number2];
    for(i = order-1; i >=0; i--)
    	pc2.knot[i] = t;
    for(i = number2-1; i >= 0; i--)
    {
    	pc2.ctrl[i] = pcurve.ctrl[i+pcurve.n-number2];
    	pc2.weight[i] = pcurve.weight[i+pcurve.n-number2];
    }

	// ----------------------------------------------------------------
    // liuyushen 2002/11/28 对节点向量重新整理，规划为0、1之间
    // ----------------------------------------------------------------
    double length = pc1.knot[pc1.n] - pc1.knot[pc1.order-1];
    if (length > 0)
    {
        for(i = pc1.order; i < pc1.n; i++)
            pc1.knot[i] = (pc1.knot[i] - pc1.knot[pc1.order-1])/length;
        for(i = 0; i < pc1.order; i++)
            pc1.knot[i] = 0.0;
        for(i = pc1.n; i < pc1.n + pc1.order; i++)
            pc1.knot[i] = 1.;
    }
    length = pc2.knot[pc2.n] - pc2.knot[pc2.order-1];

    if (length > 0)
    {
        for(i = pc2.order; i < pc2.n; i++)
            pc2.knot[i] = (pc2.knot[i] - pc2.knot[pc2.order-1])/length;
        for(i = 0; i < pc2.order; i++)
            pc2.knot[i] = 0.0;
        for(i = pc2.n; i < pc2.n + pc2.order; i++)
            pc2.knot[i] = 1.;
    }
	// ----------------------------------------------------------------

    delete [] pcurve.knot;
    delete [] pcurve.ctrl;
    delete [] pcurve.weight;
}

void NurbscMultiSplit(CS_NurbsCurve curve, int n, double t[], CS_NurbsCurve newcurve[])
// n为t[]的个数，即t[0],t[1],...t[n-1]
{ 
	int i,k;
	CS_NurbsCurve tm1,tm2;
	NurbscSplit(curve,t[0],newcurve[0],tm1);
	for (i = 1; i < n; i++)
	{
		NurbscSplit(tm1,(t[i]-t[i-1])/(1-t[i-1]), newcurve[i], tm2);
		// "delete tm1;,tm1 = tm2;"
		tm1.n = tm2.n;
		tm1.order = tm2.order;
		delete [] tm1.knot;
		tm1.knot = new double[tm1.n+tm1.order]; 
		delete [] tm1.ctrl;
		tm1.ctrl = new Point3D[tm1.n]; 
		delete [] tm1.weight;
		tm1.weight = new double[tm1.n]; 

		for(k = 0; k < tm1.n + tm1.order; k++)
			tm1.knot[k] = tm2.knot[k];
		for(k = 0; k < tm1.n; k++)
		{
			tm1.ctrl[k] = tm2.ctrl[k];
		    tm1.weight[k] = tm2.weight[k];
		}
		delete [] tm2.knot;
		delete [] tm2.ctrl;
		delete [] tm2.weight; 
	}
	//newcurve[n] = tm1;
	newcurve[n].n = tm1.n;
	newcurve[n].order = tm1.order; 
	newcurve[n].knot = new double[tm1.n+tm1.order]; 
	newcurve[n].ctrl = new Point3D[tm1.n]; 
	newcurve[n].weight = new double[tm1.n]; 
	for(k = 0; k < tm1.n + tm1.order; k++)
		newcurve[n].knot[k] = tm1.knot[k];
	for(k = 0; k < tm1.n; k++)
	{
		newcurve[n].ctrl[k] = tm1.ctrl[k];
	    newcurve[n].weight[k] = tm1.weight[k];
	}    
	delete [] tm1.knot;
	delete [] tm1.ctrl;
	delete [] tm1.weight; 
}
int NurbscToRBezier(CS_NurbsCurve curve, CS_RBezierCurve bezier[], double *&t)
// 返回RBezier曲线的段数
{
    int i,j,k,number,order,bez_num;
    number = curve.n;
    order = curve.order;
    bez_num = number - order + 1;  // 曲线段数

    if(bez_num == 1)
    {   
        t = NULL;
        bezier[0].n = order;
    	bezier[0].ctrl = new Point3D[order];
    	bezier[0].weight = new double[order];
    	for(i = 0; i < number; i++)
    	{
    		bezier[0].ctrl[i] = curve.ctrl[i];	 
    		bezier[0].weight[i] = curve.weight[i];	 
    	}	                    
    	return bez_num;
    }
    else 
    {
	    t = new double[bez_num - 1];
	    for(k = 0; k <= bez_num - 2; k++) 
			if( fabs(curve.knot[order + k] - curve.knot[order -1]) > TOLER )
				break; 
	    int temp = 1; 
	    t[0] = curve.knot[order + k];
	    for(i = k + 1; i <= bez_num - 2; i++)
    	{
    		if( fabs(curve.knot[order + i] - t[temp-1]) > TOLER )
    		{
    			t[temp] =  curve.knot[order + i];	
	        	temp++;
	        }
	    }
	    bez_num = temp+1;
		CS_NurbsCurve *newcurve = new CS_NurbsCurve[bez_num];
        NurbscMultiSplit(curve, bez_num-1, t, newcurve);
	    // 填 bezier[i] 结构    
	    for(i = 0; i < bez_num ; i++)
	    {    
	    	bezier[i].n = order;
	    	bezier[i].ctrl = new Point3D[order];
	    	bezier[i].weight = new double[order];
	        for(j = 0; j < order; j++)            
	        {   
	            bezier[i].ctrl[j] = newcurve[i].ctrl[j];
	            bezier[i].weight[j] = newcurve[i].weight[j];
  	        }  
	    }

	    for(i = 0; i < bez_num ; i++)
	    {    
	    	delete [] newcurve[i].ctrl;
	    	delete [] newcurve[i].knot;
	    	delete [] newcurve[i].weight;
        }                            
        delete [] newcurve;
        return bez_num;
    }  
}

	// prototype revised by Yang Lei 060406
CS_Box SPL_GetCurvSurfBox3d(int number,Point3D points[])
{
	int i;
	CS_Box  box;	
    box.m_min.x = box.m_min.y = box.m_min.z = 1e+10; 
    box.m_max.x = box.m_max.y = box.m_max.z = - 1e+10; 
    for(i = 0; i < number; i++)
    {
    	if(points[i].x < box.m_min.x) 
    		box.m_min.x = points[i].x;
    	if(points[i].y < box.m_min.y) 
    		box.m_min.y = points[i].y;
    	if(points[i].z < box.m_min.z) 
    		box.m_min.z = points[i].z;
    	if(points[i].x > box.m_max.x) 
    		box.m_max.x = points[i].x;
    	if(points[i].y > box.m_max.y) 
    		box.m_max.y = points[i].y;
    	if(points[i].z > box.m_max.z) 
    		box.m_max.z = points[i].z;
    }
	return box;
}
CS_Box GetCurvSurfBox(int number, Point3D *point)
{
	return SPL_GetCurvSurfBox3d(number, point);
	/*
	int i;
	CS_Box  box;
    box.m_min.x = box.m_min.y = box.m_min.z = 1e+10; 
    box.m_max.x = box.m_max.y = box.m_max.z = - 1e+10; 
    for(i = 0; i < number; i++)
    {
    	if(point[i].x < box.m_min.x) 
    		box.m_min.x = point[i].x;
    	if(point[i].y < box.m_min.y) 
    		box.m_min.y = point[i].y;
    	if(point[i].z < box.m_min.z) 
    		box.m_min.z = point[i].z;
    	if(point[i].x > box.m_max.x) 
    		box.m_max.x = point[i].x;
    	if(point[i].y > box.m_max.y) 
    		box.m_max.y = point[i].y;
    	if(point[i].z > box.m_max.z) 
    		box.m_max.z = point[i].z;
    }
	return box;
	*/
}
int BoxXaxisInterTest(CS_Box box)
// Box 不相交, 返回1
{
    if (box.m_min.y < CS_EPS && box.m_max.y >  - CS_EPS &&
        box.m_min.z < CS_EPS && box.m_max.z >  - CS_EPS )
        return 1;
    return 0;
}

void SPL_RbezcSplit(CS_RBezierCurve curve, double value, CS_RBezierCurve &pc1, CS_RBezierCurve &pc2)
{  
    //为pc1,pc2申请空间
    int number = curve.n;
    pc1.n = number;
    pc1.ctrl = new Point3D[number]; 
    pc1.weight = new double[number]; 
    pc2.n = number;
    pc2.ctrl = new Point3D[number]; 
    pc2.weight = new double[number]; 

	int     i,j,k;
    double  *pp = new double[number];
    // 对控制顶点的三个分量及权因子用de casteljau算法的中间点得到新控制顶点
    pc1.ctrl[0] = curve.ctrl[0];
    pc2.ctrl[number-1] = curve.ctrl[number-1]; 
    pc1.weight[0] = curve.weight[0];
    pc2.weight[number-1] = curve.weight[number-1]; 
    for( k = 1 ; k <= 4; k++ )  
	{  
		for( j = 0 ; j < number ; j++ ) 
		{ 
			if( k == 1 )  
			pp[j] = curve.ctrl[j].x*curve.weight[j];
			else if (k == 2) 
			pp[j] = curve.ctrl[j].y*curve.weight[j];
			else if (k == 3) 
			pp[j] = curve.ctrl[j].z*curve.weight[j];
		    else if (k == 4) 
			pp[j] = curve.weight[j];
		    else;
		} 
        for( i = 1 ; i < number; i++ )
		{
			for( j = 0 ; j < number -i; j++ )
				pp[j] = (1-value)*pp[j] + value*pp[j+1];  
			if( k == 1 )  
			{
			    pc1.ctrl[i].x = pp[0];
			    pc2.ctrl[number-1-i].x = pp[number-1-i]; 
			}
			else if (k == 2)
			{
			    pc1.ctrl[i].y = pp[0];
			    pc2.ctrl[number-1-i].y = pp[number-1-i]; 
			}
	    	else if (k == 3)
			{
			    pc1.ctrl[i].z = pp[0];
			    pc2.ctrl[number-1-i].z = pp[number-1-i]; 
			}
	    	else if (k == 4)
			{
			    pc1.weight[i] = pp[0];
			    pc2.weight[number-1-i] = pp[number-1-i]; 
			}
		    else;
		}
	}       
	delete pp;
	// 控制顶点与权因子分离   
	for( j = 1 ; j < number ; j++ ) 
	{ 
	    pc1.ctrl[j].x = pc1.ctrl[j].x/pc1.weight[j];
	    pc1.ctrl[j].y = pc1.ctrl[j].y/pc1.weight[j];
	    pc1.ctrl[j].z = pc1.ctrl[j].z/pc1.weight[j]; 
    }                      
    for( j = 0 ; j < number -1; j++ ) 
	{ 
	    pc2.ctrl[j].x = pc2.ctrl[j].x/pc2.weight[j];
	    pc2.ctrl[j].y = pc2.ctrl[j].y/pc2.weight[j];
	    pc2.ctrl[j].z = pc2.ctrl[j].z/pc2.weight[j]; 
    }  
}

// 将一条曲线离散为四条                                   
void SPL_SubdRbezcToNCurve(CS_RBezierCurve curve, CS_RBezierCurve *rcurve)
{            
    CS_RBezierCurve  temp1,temp2;
	SPL_RbezcSplit(curve, 0.5, temp1, temp2);
	SPL_RbezcSplit(temp1, 0.5, rcurve[0], rcurve[1]);
    SPL_RbezcSplit(temp2, 0.5, rcurve[2], rcurve[3]);
    delete [] temp1.ctrl;
    delete [] temp1.weight;
    delete [] temp2.ctrl;
    delete [] temp2.weight;
}
// 将一条曲线离散为四条                                   
void SubdRbezcToNCurve(CS_RBezierCurve curve, CS_RBezierCurve *rcurve)
{            
    SPL_SubdRbezcToNCurve(curve, rcurve);
	/*
	CS_RBezierCurve  temp1,temp2;
	RbezcSplit(curve, 0.5, temp1, temp2);
	RbezcSplit(temp1, 0.5, rcurve[0], rcurve[1]);
    RbezcSplit(temp2, 0.5, rcurve[2], rcurve[3]);
    delete [] temp1.ctrl;
    delete [] temp1.weight;
    delete [] temp2.ctrl;
    delete [] temp2.weight;
	*/
}
int GetXaxisBezcNearInter(CS_RBezierCurve curve, CS_CurvCurvIntPoint *ip)
//  由控制多边形与控制多边形求交得到Bezier曲线与Bezier曲线的近似交点
{
    CS_Box   box;
    box = GetCurvSurfBox(curve.n, curve.ctrl);
    if(!BoxXaxisInterTest(box))
        return 0;

    int flag, k;
    CS_RBezierCurve  rcurv[4];
    SubdRbezcToNCurve(curve, rcurv);

    Point3D pp;
    int inter_num = 0;
    for(k = 0; k < 4; k++)
    {
        box = GetCurvSurfBox(rcurv[k].n, rcurv[k].ctrl);
        flag = BoxXaxisInterTest(box);
        if( flag == 1 )
        {
            pp.x = (box.m_min.x+box.m_max.x)/2;
            pp.y = 0;
            pp.z = 0;
            ip[inter_num].p = pp;
            ip[inter_num].t1 = (k+0.5)/4;
            inter_num++;
        }
    }
    for( k = 0; k < 4; k++ )
    {
        delete [] rcurv[k].ctrl;
        delete [] rcurv[k].weight;
    }
    return inter_num;
}
int CS_XaxisRbezcNearInter(CS_RBezierCurve curve, CS_CurvCurvIntPoint *ip)
// 离散法求Bezier曲线与X轴的近似交点
{
    CS_Box   box;
    box = GetCurvSurfBox(curve.n, curve.ctrl);
    if(!BoxXaxisInterTest(box))
        return 0;

    int i,k;
    CS_CurvCurvIntPoint rp[10];
    CS_RBezierCurve  rcurv[4];
    SubdRbezcToNCurve(curve, rcurv);

    int resu, int_num = 0;
    for(i = 0; i < 4; i++)
    {
        resu = GetXaxisBezcNearInter(rcurv[i],rp);
        if( resu > 0 )
        {
            for( k = 0; k < resu; k++ )
            {
                ip[int_num+k].p = rp[k].p;
                ip[int_num+k].t1 = (rp[k].t1 + i)/4;
            }
            int_num += resu;
        }
    }
    for( k = 0; k < 4; k++ )
    {
        delete [] rcurv[k].ctrl;
        delete [] rcurv[k].weight;
    }
    return int_num;
}
int CS_XaxisNurbscNearInterP(CS_NurbsCurve curve, CS_CurvCurvIntPoint *ip)
//  求近似交点： 返回 0 两曲线不相交; >0 两线段相交(交点个数); 交点存在interp中
{
    int k,l;

    // 先将曲线curve转化为Bezier曲线
    int cn,cord,bcnum, nn;
    cn = curve.n;
    cord = curve.order;
    bcnum = cn - cord + 1;       // 曲线curve最大段数
    double *nknot;
    nknot = new double [cord+cn];
    GetKnotNoRepeat(cord+cn, nn, curve.knot, nknot);

    //曲线curve转化为Bezier曲线
  	double *splitpara;
    CS_RBezierCurve  *rcurv = new CS_RBezierCurve[bcnum];
    bcnum = NurbscToRBezier(curve, rcurv, splitpara);
    // 直线与Bezier曲线两两求交
    int flag,inter_num=0;
    CS_CurvCurvIntPoint pp[32];
    for(k = 0; k < bcnum; k++)
    {
        flag = CS_XaxisRbezcNearInter(rcurv[k], pp);
        if( flag > 0 )
        {
            for( l = 0; l < flag; l++ )
            {
                ip[inter_num+l].p = pp[l].p;
                ip[inter_num+l].t1 = (nknot[k+1]-nknot[k])*pp[l].t1 + nknot[k];
            }
            inter_num += flag;
        }
    }
    for( k = 0; k < bcnum; k++ )
    {
        delete [] rcurv[k].ctrl;
        delete [] rcurv[k].weight;
    }
    delete [] rcurv;
    delete [] splitpara;
    delete [] nknot;

    // 交点处理:
    return inter_num;
}
// 计算Nurbs曲线上参数为Value处的值、一阶导数值
void NurbscComputAll(CS_NurbsCurve curve, double value, Point3D &position, Vector3D &cv1)
{
/*
	if(CS_NurbsSIsBspl(curve))
	{
		CS_BSplineCurve bcurv;
		PutNurbsCToBspl(curve, bcurv);
		return BscurvComputAll(bcurv, value, position, cv1);
	}
*/
	
    int number,order,i,j,k,l;
    number = curve.n;
    order = curve.order;   
    k = order-1;

    double    *alf, *W, wu, w1p, w2;
    Vector3D  *C, pu, p1p, v;
    // 导数 = (P'*W - P*W')/(W*W) = (tp_vector*w_value - p_vector*tw_value)/(w_value*w_value)
    C = new Vector3D[number];
    W = new double[number];
    alf = new double[number];
    i = NurbscIntervalIndex(curve,value);  
    // 计算 P(t) , W(t)
    for( j = i-k; j <= i; j++ ) 
    {    
        C[j] = (curve.ctrl[j] - ZeroP3D)*curve.weight[j];
        W[j] = curve.weight[j];
    }    
    for( l = 1; l <= k; l++)
    {
        for( j = i-k+l; j <= i ; j++ ) 
            alf[j] = (value - curve.knot[j])/(curve.knot[j+k+1-l]-curve.knot[j]);
        for( j = i ; j >= i-k+l ; j-- ) 
        {
            C[j] = C[j-1]*(1-alf[j])+C[j]*alf[j];
            W[j] = W[j-1]*(1-alf[j])+W[j]*alf[j];
        }    
    }     
    pu = C[i];  
    wu = W[i];

    // 计算 P'(t), W'(t) 
    for( j = i-k; j <= i; j++ ) 
    {    
        C[j] = (curve.ctrl[j] - ZeroP3D)*curve.weight[j];
        W[j] = curve.weight[j];
    }    
    for( j = i; j >= i-k+1; j-- )
    {
        C[j] = (C[j] - C[j-1])*k/(curve.knot[j+k] - curve.knot[j]);  
        W[j] = (W[j] - W[j-1])*k/(curve.knot[j+k] - curve.knot[j]);  
    }    
    k = k-1;
    for( l = 1; l <= k; l++)
    {
        for( j = i-k+l; j <= i ; j++ ) 
            alf[j] = (value - curve.knot[j])/(curve.knot[j+k+1-l]-curve.knot[j]);
        for( j = i ; j >= i-k+l ; j-- ) 
        {
            C[j] = C[j-1]*(1-alf[j])+C[j]*alf[j];
            W[j] = W[j-1]*(1-alf[j])+W[j]*alf[j];
        }    
    }     
    p1p = C[i];  
    w1p = W[i];  

    delete C;
    delete W;
    delete alf;
    w2 = wu*wu;
    position = ZeroP3D + pu/wu;
    cv1 = (p1p*wu - pu*w1p)/w2;
}
// 用控制多边形长度近似曲线弧长
double NurbscNetLengh(CS_NurbsCurve curv)
{
    double length = 0;
    for (int i=1; i<curv.n; i++)
        length += Distance(curv.ctrl[i-1], curv.ctrl[i]);
    return length;
}     
// 求曲线上一点的信息, 包括定义区间外; 用于MAF求交
void NurbscComputAllExtend(CS_NurbsCurve curve, double value, Point3D &position, Vector3D &cv)
{
    if(value > 0 && value < 1)
        NurbscComputAll(curve, value, position, cv);
    else
    {
        /*double scale; 
        if(value > 1)
        {
            if(value - 1 < 0.16)
         scale = 0.16;
                   else 
                scale = 0.24;
            NurbscExtend(curve, 1, scale);
            position = NurbscPosition(curve, value/(1+scale));
            cv = NurbscDerivative1(curve, value/(1+scale));
        }
        else
        {
            if(0 - value < 0.16)
                scale = 0.16;
            else 
                scale = 0.24;
            NurbscExtend(curve, 0, scale);
            position = NurbscPosition(curve, (scale+value)/(1+scale));
            cv = NurbscDerivative1(curve, (scale+value)/(1+scale));
        }*/
        Point3D p;
        Vector3D v;
        double len;
        len = NurbscNetLengh(curve);
        if (value > 1)
        {
            NurbscComputAll(curve, 1, p, v);
            cv = v;
            v.SetUnit();
            position = p + v*(len*(value-1));
        }
        else
        {
            NurbscComputAll(curve, 0, p, v);
            cv = v;
            v.SetUnit();
            position = p + v*(len*value);
        }
    }   
}
// 2. 平面:  非法 return 1                  
int IllegalPlane(CS_Plane  &plan)  
{
    if( Length(plan.vn) < TOLER )
        return 1;
    else 
        return 0;      
}
//直线与平面求交  (魏开敏：11)
int LinePlaneInters(CS_Line ln , CS_Plane pln , Point3D &pi) 
{
    if (IllegalLine(ln) || IllegalPlane(pln))
        return -1;   

    pln.vn.SetUnit();
    ln.vn.SetUnit();             
    if ( fabs (pln.vn * ln.vn) < TOLER ) 
        if ( fabs(pln.vn * (ln.pa - pln.pa)) < CS_EPS ) 
            return 2;   //重合
        else 
            return 0;   //平行但相离
    else//相交，求交点
    {   double t;               
        t = pln.vn * ( pln.pa - ln.pa );
        t /= pln.vn * ln.vn;
        pi = ln.pa + ln.vn * t;
        return 1;
    }
}
//===============================================================================
// 4. 已知直线上一点，确定参数t: pvar = psta + pvec*t
double GetLineParameter(Point3D pvar, Point3D psta, Vector3D pvec)
{
   if( (fabs(pvec.x) >= fabs(pvec.y)) &&  (fabs(pvec.x) >= fabs(pvec.z)) )
      return( (pvar.x - psta.x)/pvec.x );
   else
   {
      if(fabs(pvec.y) >= fabs(pvec.z))
         return( (pvar.y - psta.y)/pvec.y );
      else
         return( (pvar.z - psta.z)/pvec.z );
  }
}
void CurvCurvIntPointReduce(int &rn, CS_CurvCurvIntPoint rt[])
//   剔除重复的近似交点
{   
    if(rn > 1)
    {
        int flag,i,j,numb = 1;
        CS_CurvCurvIntPoint *srt  = new CS_CurvCurvIntPoint[rn];
        for( i = 0; i <= rn-1; i++ )
        {
            srt[i].p = rt[i].p;
            srt[i].t1 = rt[i].t1;
            srt[i].t2 = rt[i].t2;
        }
        for( i = 1; i <= rn-1; i++ )
        {        
            flag = 0;
            for( j = 0; j<= numb -1; j++ )
                if(Distance(srt[i].p, rt[j].p) < CS_EPS )
                {   flag = 1;   break; }
            if( flag == 0 )         
            {
                rt[numb].p = srt[i].p;
                rt[numb].t1 = srt[i].t1;
                rt[numb].t2 = srt[i].t2;
                numb++; 
            }
        }           
        rn = numb;
        delete [] srt;
    } 
}  
void FreeNurbsCurve(CS_NurbsCurve curve)
{
    delete [] curve.ctrl;
    delete [] curve.weight;
    delete [] curve.knot;
} 

// 求交点： 返回 0 两曲线不相交; >0 两线段相交(交点个数); 交点存在CS_CurvSurfIntPoint结构中
int CS_LineNurbscInter(CS_Line line, CS_NurbsCurve curve, CS_CurvCurvIntPoint *ip)
{
    CS_CurvCurvIntPoint interp[16];
    CS_NurbsCurve curv = NurbscCopy(curve);
    //把直线和曲线转化到以直线为x轴的坐标系中
    Point3D cp0(0,0,0), cp;
    Vector3D vx, vy, vz,vx0(1,0,0);
    NurbscLineConvert(curve, line, curv, cp, vx, vy, vz);
    int flag = CS_XaxisNurbscNearInterP(curv, interp);
    if(flag == 0)   // 得不到近似交点，应该无解
    {
        //FreeNurbsCurve(curv);
        return 0;
    }

    // 用MAF方法求精确交点

    int i,iter_num,inter_num;
    double t, dt1;
    Vector3D  cv1, cv12;
    Point3D   cp1, comp;
    CS_Line line1;
    CS_Plane pln;

    inter_num = 0;
    for(i = 0; i < flag; i++ )
    {
        t = interp[i].t1;
        iter_num = 0;
        while(iter_num < 99)
        {
            NurbscComputAllExtend(curv, t, cp1, cv1);
            if(cp1.y*cp1.y+cp1.z*cp1.z < CS_EPS*CS_EPS)
                break;
            line1.pa = cp1;     line1.vn = cv1;
            cv12 = cv1 ^ vx0;
            pln.pa = cp0;       pln.vn = cv12 ^ vx0;
            LinePlaneInters(line1 , pln , comp);
            dt1 = GetLineParameter(comp, cp1, cv1);
            t += dt1;
            if (t < -Extend_Length || t > 1.0+Extend_Length)
                iter_num = 98;
            iter_num++;
        }
        if(iter_num < 99 && t > -TOLER && t < 1.0+TOLER)
        {
            ip[inter_num].p = cp+vx*cp1.x;
            ip[inter_num].t1 = t;
            inter_num++;
        }
    }
    // 交点处理:
    if(inter_num != 0)
        CurvCurvIntPointReduce(inter_num, ip);
    FreeNurbsCurve(curv);
    return inter_num;
}



int LineNurbscInters(CS_Line line, CS_NurbsCurve curve,  CS_IntCurve *inp, int &numb)
{
    int  k, result;
    CS_CurvCurvIntPoint temp[16];
    result = CS_LineNurbscInter(line, curve, temp);
    for(k = 0; k < result; k++)
        inp[k/8].pt[k%8] = temp[k].p;
    for(k = 0; k < result/8; k++)
        inp[k].type = INTER_POINT8P;
    if(result%8 != 0)
    {
        inp[result/8].type = result%8 + INTER_POINT1P - 1;
        numb = result/8 + 1;
        return numb;
    }
    else
    {
        numb = result/8;
        return numb;
    }
}

int LineBezier4Inters(CS_Line line, CS_NurbsCurve curve,  CS_IntCurve *inp, int &numb)
{
    int  k, result;
    CS_CurvCurvIntPoint temp[16];

	if(curve.order != 4 || curve.numb != 4)
		return -1;
	if(fabs( curve.weight[0] - 1) > 0.001 || fabs( curve.weight[1] - 1) > 0.001 || 
		fabs( curve.weight[2] - 1) > 0.001 || fabs( curve.weight[3] - 1) > 0.001)
		return -1;

	//convert into implicit equation and get the value


    for(k = 0; k < result; k++)
        inp[k/8].pt[k%8] = temp[k].p;
    for(k = 0; k < result/8; k++)
        inp[k].type = INTER_POINT8P;
    if(result%8 != 0)
    {
        inp[result/8].type = result%8 + INTER_POINT1P - 1;
        numb = result/8 + 1;
        return numb;
    }
    else
    {
        numb = result/8;
        return numb;
    }
}

	// prototype revised by Yang Lei 060406
// 判断点在包围盒内
int SPL_PointInBox3d(Point3D point, CS_Box box)
{
	if(point.x > box.m_min.x - TOLER && point.y > box.m_min.y - TOLER && 
	   point.z > box.m_min.z - TOLER && point.x < box.m_max.x + TOLER && 
	   point.y < box.m_max.y + TOLER && point.z < box.m_max.z + TOLER)
		return 1;
	else 
		return 0;
}
// 求一点在有理Bezier曲线上的近似参数
int SPL_RbezcNearPointPara(CS_RBezierCurve curve, Point3D poin, double *para)
{
	CS_Box   box;
	box = SPL_GetCurvSurfBox3d(curve.n, curve.ctrl);
    if(!SPL_PointInBox3d(poin, box))  
    	return 0; 
    
    int j;
    CS_RBezierCurve  rcurv[4];
    SPL_SubdRbezcToNCurve(curve, rcurv);

    int int_num = 0;
	for(j = 0; j < 4; j++)            
	{
		box = SPL_GetCurvSurfBox3d(rcurv[j].n, rcurv[j].ctrl);
        if(!SPL_PointInBox3d(poin, box))
        	continue;
	    para[int_num] = (0.5 + j)/4;
       	int_num ++;
    }	
	for( j = 0; j < 4; j++ )
    {
        delete [] rcurv[j].ctrl;
        delete [] rcurv[j].weight;
    }
    return int_num; 
}
// 求一点在有理Bezier曲线上的近似参数
int RbezcNearPointPara(CS_RBezierCurve curve, Point3D poin, double *para)
{
	return SPL_RbezcNearPointPara(curve, poin, para);
	/*
	CS_Box   box;
	box = GetCurvSurfBox(curve.n, curve.ctrl);
    if(!PointInBox(poin, box))  
    	return 0; 
    
    int j;
    CS_RBezierCurve  rcurv[4];
    SubdRbezcToNCurve(curve, rcurv);

    int int_num = 0;
	for(j = 0; j < 4; j++)            
	{
		box = GetCurvSurfBox(rcurv[j].n, rcurv[j].ctrl);
        if(!PointInBox(poin, box))
        	continue;
	    para[int_num] = (0.5 + j)/4;
       	int_num ++;
    }	
	for( j = 0; j < 4; j++ )
    {
        delete [] rcurv[j].ctrl;
        delete [] rcurv[j].weight;
    }
    return int_num; 
	*/
}
Point3D PointProjectToLine(Point3D point, Point3D origin, Vector3D xaxis) 
{
    xaxis.SetUnit();
    return origin + xaxis*((point - origin)*xaxis);
}
void SPL_NumberReduce(int &rn, double rt[])
//   剔除重复的近似交点
{   
    if(rn > 1)
    {
	    int flag,i,j,numb = 1;
	    double *srt  = new double[rn];
	    for( i = 0; i <= rn-1; i++ )
	        srt[i] = rt[i];
	    for( i = 1; i <= rn-1; i++ )
	    {        
	        flag = 0;
	        for( j = 0; j <= numb -1; j++ )
	            if(fabs(srt[i] - rt[j]) < TOLER )
	            {   flag = 1;   break; }
	        if( flag == 0 )         
	        {
	            rt[numb] = srt[i];
	            numb++; 
	        }
	    }           
	    rn = numb;
        delete srt;
	} 
}
void NumberReduce(int &rn, double rt[])
//   剔除重复的近似交点
{   
    SPL_NumberReduce(rn, rt);
	/*
	if(rn > 1)
    {
	    int flag,i,j,numb = 1;
	    double *srt  = new double[rn];
	    for( i = 0; i <= rn-1; i++ )
	        srt[i] = rt[i];
	    for( i = 1; i <= rn-1; i++ )
	    {        
	        flag = 0;
	        for( j = 0; j <= numb -1; j++ )
	            if(fabs(srt[i] - rt[j]) < TOLER )
	            {   flag = 1;   break; }
	        if( flag == 0 )         
	        {
	            rt[numb] = srt[i];
	            numb++; 
	        }
	    }           
	    rn = numb;
        delete srt;
	} 
	*/
}
bool CS_NurbsCIsBspl(CS_NurbsCurve& curv)
{
	if(curv.type.isBSpline == CSNT_IsBSpline)
		return true;
	else if(curv.type.isBSpline == CSNT_IsNURBS)
		return false;
	else
	{
		int i;
		for(i = 0; i < curv.n; ++i)
		{
			if(curv.weight[i] > 1 + CS_EPS ||
			    curv.weight[i] < 1 - CS_EPS )
			    break;
		}
		if(i == curv.n)
		{
			curv.type.isBSpline = CSNT_IsBSpline;
			return true;
		} else
		{
			curv.type.isBSpline = CSNT_IsNURBS;
			return false;
		}
	}
}

// 取得NURBS曲面的BSpl曲面表示（不重新复制数据结构）用于临时计算
void PutNurbsCToBspl(CS_NurbsCurve& ncurv, CS_BSplineCurve& bcurv)
{
	bcurv.ctrl = ncurv.ctrl;
	bcurv.knot = ncurv.knot;
	bcurv.n = ncurv.n;
	bcurv.order = ncurv.order;
}

Point3D BscurvPosition(CS_BSplineCurve curve, double value)
// 只对4阶以下有效
{
	int			i, r, j, m, order = curve.order;
    double		fz1, fz2, fm;
    //-------------------------------------------------
	//Yupiqiang 2003/07/21,改为动态数组，防止越界
	Vector3D	*C;

    C=new Vector3D[order];
    //-------------------------------------------------
    j = BscurvIntervalIndex(curve, value);
	m = j - order + 1;

	for( i = m; i <= j; i++ )
    	C[i-m] = curve.ctrl[i] - ZeroP3D;

    for( r = 1; r <= order - 1; r++ )
    {
    	for( i = m; i <= j - r; i++ )
		{
			fz1 = curve.knot[i+order] - value;
			fz2 = value - curve.knot[i+r];
			fm  = curve.knot[i+order] - curve.knot[i+r];
    		C[i-m] = (C[i-m]*fz1 + C[i-m+1]*fz2)/fm;
		}
    }
    //----------------------------------------------------------
	Point3D pt=C[0];
	delete []C;
    return pt;
	//----------------------------------------------------------
/*
	int i,j,r,order,number;   
    Vector3D  *C;
    Point3D   position; 

	order =  curve.order;
	number = curve.n;
    j = BscurvIntervalIndex(curve, value);  
    
	C = new Vector3D[number];
    for( i = 0; i <= number - 1; i++ ) 
    	C[i] = curve.ctrl[i] - ZeroP3D;
    	
    for( r = 1; r <= order -1; r++)
    {
    	for( i = j - order + 1; i <= j - r; i++ )
    		C[i] = C[i]*(curve.knot[i+order] - value)/(curve.knot[i+order] - curve.knot[i+r]) + 
    		       C[i+1]*(value - curve.knot[i+r])/(curve.knot[i+order] - curve.knot[i+r]); 
    }     
    position = ZeroP3D + C[j-order+1];
	delete 	C;	 
	return  position;
*/
}

Point3D NurbscPosition(CS_NurbsCurve curve, double value)
{
	if(CS_NurbsCIsBspl(curve))
	{
		CS_BSplineCurve bcurv;
		PutNurbsCToBspl(curve, bcurv);
		return BscurvPosition(bcurv, value);
	}

	int			i, r, j, m, order = curve.order;
    //------------------------------------------------------------
	//Yupiqiang 2003/07/21
	//double		W[4], fz1, fz2, fm;
    //Vector3D	C[4];
	double		*W, fz1, fz2, fm;
    Vector3D	*C;
	W=new double[order];
    C=new Vector3D[order];
	//------------------------------------------------------------
	j = NurbscIntervalIndex(curve, value);
	m = j - order + 1;

	for( i = m; i <= j; i++ )
	{
    	W[i-m] = curve.weight[i];
    	C[i-m] = (curve.ctrl[i] - ZeroP3D)*curve.weight[i];
	}

    for( r = 1; r <= order - 1; r++ )
    {
    	for( i = m; i <= j - r; i++ )
		{
			fz1 = curve.knot[i+order] - value;
			fz2 = value - curve.knot[i+r];
			fm  = curve.knot[i+curve.order] - curve.knot[i+r];
    		W[i-m] = (W[i-m]*fz1 + W[i-m+1]*fz2)/fm;
    		C[i-m] = (C[i-m]*fz1 + C[i-m+1]*fz2)/fm;
		}
    }
	//----------------------------------------------------------------------
    //return ZeroP3D + C[0]/W[0];
	Vector3D vec=C[0]/W[0];
	delete []C;
    delete []W;
    return ZeroP3D + vec;
	//------------------------------------------------------------------------
/*
    int i,j,r,order,number;   
    Vector3D  *C;
    double    *W;
    Point3D   position; 

	order =  curve.order;
	number = curve.n;
    j = NurbscIntervalIndex(curve, value);  
    
	C = new Vector3D[number];
    for( i = 0; i <= number - 1; i++ ) 
    	C[i] = (curve.ctrl[i] - ZeroP3D)*curve.weight[i];
    	
    for( r = 1; r <= order -1; r++)
    {
    	for( i = j - order + 1; i <= j - r; i++ )
    		C[i] = C[i]*(curve.knot[i+order] - value)/(curve.knot[i+order] - curve.knot[i+r]) + 
    		       C[i+1]*(value - curve.knot[i+r])/(curve.knot[i+order] - curve.knot[i+r]); 
    }     

	W = new double[number];
    for( i = 0; i <= number - 1; i++ ) 
    	W[i] = curve.weight[i];
    	
    for( r = 1; r <= order -1; r++)
    {
    	for( i = j - order + 1; i <= j - r; i++ )
    		W[i] = W[i]*(curve.knot[i+order] - value)/(curve.knot[i+order] - curve.knot[i+r]) +  
    		       W[i+1]*(value - curve.knot[i+r])/(curve.knot[i+order] - curve.knot[i+r]);
    }     
    position = ZeroP3D + C[j-order+1]/W[j-order+1];
	delete 	C;	 
    delete  W;
	return  position;
*/
}
#define  ITERAT_NUM 124      // 几何迭代次数
// 反求曲线参数
double NurbscGetPointPara(CS_NurbsCurve curve, Point3D p)
{
    // 求初始近似参数
    int i,k,l;
    double  par[4], startpara[4];
    // 先将曲线curve转化为Bezier曲线, 
    int number,order,bcnumber;
    number = curve.n; 
    order = curve.order; 
    bcnumber = number - order +1; // 曲线最大段数

    //曲线curve转化为Bezier曲线
	double *splitpara;
    CS_RBezierCurve  *rcurv = new CS_RBezierCurve[bcnumber];
    bcnumber = NurbscToRBezier(curve, rcurv, splitpara);
    
    // 求点在曲线中的近似参数
    int     flag, sp_num = 0;
	for(i = 0; i < bcnumber; i++)             
	{
        flag = RbezcNearPointPara(rcurv[i], p, par);
        if( flag > 0 ) 
        {
        	for( l = 0; l < flag; l++ )
            {
//                startpara[sp_num+l] =  curve.knot[order-1+i] + 
//        	       par[l]*(curve.knot[order+i] - curve.knot[order-1+i]);
                if ( sp_num+l > 4 )
                    break;
        	    if(bcnumber == 1)
                    startpara[sp_num+l] = par[l];
                else if(i == 0)
                    startpara[sp_num+l] = par[l]*splitpara[0];
                else if(i == bcnumber-1)
                    startpara[sp_num+l] = splitpara[bcnumber-2] + par[l]*(1-splitpara[bcnumber-2]);
                else 
                    startpara[sp_num+l] = splitpara[i-1] + par[l]*(splitpara[i] - splitpara[i-1]);
            }
            sp_num += flag;
        }
	}                                       
    for(k = 0; k < bcnumber; k++)
    {
	    delete [] rcurv[k].ctrl;
        delete [] rcurv[k].weight;
    }
    delete [] rcurv;
    delete [] splitpara;
    
    // 迭代修正
    int      iter_num, rp_num = 0;
	double   para,paratemp[4];
    Point3D  projp, iterp;
    Vector3D axis;
    double   dt;
 
	for(k = 0; k < sp_num; k++)
	{          
		para = startpara[k];

        // 几何迭代
	    iter_num = 0;      
	    while(iter_num < ITERAT_NUM) 
	    {
			NurbscComputAll(curve,para,iterp,axis);
	        projp = PointProjectToLine(p, iterp, axis);
			
	        dt = ((projp - iterp)*axis)/(axis*axis);
//			if(dt == 0)  
//				dt = 1;
	    	para += dt;
	        if(para > curve.knot[number])      para = curve.knot[number];
	    	if(para < curve.knot[order-1])     para = curve.knot[order-1]; 
	    	if( fabs(dt) < TOLER ) 
		    	break;
		    iter_num++;
		}
        if(iter_num < ITERAT_NUM-1)
        {
        	paratemp[rp_num] = para; 
        	rp_num++;
        }	
    }             
    NumberReduce(rp_num, paratemp);
    if(rp_num  == 1) 
    	return paratemp[0];
    else
    {   
    	for(k = 1; k < rp_num; k++)
        	if(Distance(p,NurbscPosition(curve, paratemp[k])) < Distance(p,NurbscPosition(curve, paratemp[0])))
        		paratemp[0] = paratemp[k];
		return paratemp[0];
    }
}
