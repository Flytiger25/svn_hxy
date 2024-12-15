#include "stdafx.h"
#include "math.h"
#include "disline.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
Point2D C_Curve2D::GetStartPoint()                { return Point2D(0,0); }
Point2D C_Curve2D::GetEndPoint()                  { return Point2D(0,0); }
Point2D C_Curve2D::GetMiddlePoint()               { return Point2D(0,0); }
Point2D C_Curve2D::GetParaPoint(double t)         { return Point2D(0,0); }
double C_Curve2D::GetPointPara(Point2D p)         { return 0; }
BOOL C_Curve2D::PointOn(Point2D p, double toler)  { return TRUE; }
C_Curve2D *C_Curve2D::Copy()                      { return this; }
C_Curve2D *C_Curve2D::Reverse()                   { return this; }

void C_Curve2D::SplitD(double t, C_Curve2D **pC1, C_Curve2D **pC2)  { }
void C_Curve2D::SplitP(Point2D &p, C_Curve2D **pC1, C_Curve2D **pC2) { }
void C_Curve2D::SplitN(int n, double t[], Point2D p[], C_Curve2D *newcurve[])    { }

Vector2D C_Curve2D::GetParaTangent(double t)  { return Vector2D (0, 0); }
void C_Curve2D::CalBox(double &umin, double &vmin, double &umax, double &vmax) { }

C_Point2D::C_Point2D(void)
{
}

C_Point2D::C_Point2D(Point2D point)
{
    m_point = point;
}

C_Point2D::C_Point2D(C_Point2D &point)
{
    m_point = point.GetPoint();
}

C_Point2D::C_Point2D(C_Point2D *point)
{
    m_point = point->GetPoint();
}

Point2D C_Point2D::GetPoint(void)
{
    return m_point;
}

void C_Point2D::SetPoint(Point2D point)
{
    m_point = point;
}

C_Point2D *C_Point2D::Copy(void)
{
    return new C_Point2D(m_point);
}


C_Curve3D::C_Curve3D(void)
{
}

C_Curve3D::~C_Curve3D()
{
}

int C_Curve3D::PrintInformation(void)
{
  return 0;
}

void C_Curve3D::Draw(CDC *pDC)
{
}

C_Disline2D::C_Disline2D(void)
{
    m_PointA.SetSize( 0, 1 );
}

C_Disline2D::C_Disline2D(int initnum)
{
    m_PointA.SetSize( initnum, 1 );
    //m_PointA.SetSize( 0, 1 );
}

C_Disline2D::~C_Disline2D(void)
{
    int n = m_PointA.GetSize();
   for (int i=0; i<n; i++)
        delete (C_Point2D *) m_PointA[i];
    m_PointA.RemoveAll();
}

int C_Disline2D::GetDispointNum()
{
    return m_PointA.GetSize();
}

C_Point2D* C_Disline2D::GetDispoint(int index)
{
    return (C_Point2D *) m_PointA[index];
}

void C_Disline2D::AddDispoint(C_Point2D *dispoint2d)
{
    m_PointA.Add( dispoint2d );
}

void C_Disline2D::AllocDispoint(int number)
{
    m_PointA.SetSize(number, 1);
}

void C_Disline2D::SetDispoint(int index, C_Point2D *dispoint2d)
{
    m_PointA.SetAt(index, dispoint2d);
}

/////////////////////////////////////////////////////
Point2D C_Disline2D::GetStartPoint()
{
    return ( (C_Point2D *) m_PointA[0] )->GetPoint();
}

Point2D C_Disline2D::GetEndPoint()
{
    int index = m_PointA.GetSize() - 1;
    return ( (C_Point2D *) m_PointA[index] )->GetPoint();
}

Point2D C_Disline2D::GetMiddlePoint()
{
    int maxi = m_PointA.GetSize() - 1;
    int midi = maxi / 2;
    if (maxi < 4)
    {
        if ( maxi % 2 == 0 )  // 取中间顶点
            return ( (C_Point2D *) m_PointA[midi] )->GetPoint();
        else                  // 取小段中点
        {
            Point2D p1 = ( (C_Point2D *) m_PointA[midi] )->GetPoint();
            Point2D p2 = ( (C_Point2D *) m_PointA[midi+1] )->GetPoint();
            return Point2D ( (p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5 );
        }
    }
    else    // 强制取中间顶点, 该点为曲线上的精确点
        return ( (C_Point2D *) m_PointA[midi] )->GetPoint();
}

Point2D C_Disline2D::GetParaPoint(double t)
{
    int maxi = m_PointA.GetSize() - 1;
    double pos = t * maxi;
    int i = (int) pos;
    if ( i == maxi )
        return ( (C_Point2D *) m_PointA[maxi] )->GetPoint();
    else   // 线性插值
    {
        Point2D p1 = ( (C_Point2D *) m_PointA[i] )->GetPoint();
        Point2D p2 = ( (C_Point2D *) m_PointA[i+1] )->GetPoint();
        return p1 + (p2 - p1) * (pos - i);
    }
}

double C_Disline2D::GetPointPara(Point2D p)
{
    Point2D stp, edp;
    Vector2D v1, v2;
    double h, t, minh, mint, Len;
    int i, n, mini;

	mini = 0;	//add by Peng Yu 2003-12-9 初始化mini

    stp = ( (C_Point2D *) m_PointA[0] )->GetPoint();
    if ( SamePoint(stp, p) )
        return 0.0;
    edp = ( (C_Point2D *) m_PointA[m_PointA.GetSize()-1] )->GetPoint();
    if ( SamePoint(edp, p) )
        return 1.0;

    minh = 1e+20;
    n = m_PointA.GetSize();
    stp = ( (C_Point2D *) m_PointA[0] )->GetPoint();
    for (i=1; i<n; i++)
    {
        h = Distance( p, stp );
        if ( h < minh )
        {
            minh = h;
            mini = i - 1;
            mint = 0.0;
            if ( minh < TOLER )
                break;
        }
        edp = ( (C_Point2D *) m_PointA[i] )->GetPoint();
        v1 = p - stp;
        v2 = edp - stp;
        Len = Length(v2);
        if ( Len > TOLER)
        {
            v2 /= Len;
            t = v1 * v2;
            if ( t >= 0.0 && t <= Len )
            {
                h = fabs( v1.x*v2.y - v1.y*v2.x );
                if ( h < minh )
                {
                    minh = h;
                    mini = i - 1;
                    mint = t / Len;
                    if ( minh < TOLER )
                        break;
                }
            }
        }

        stp = edp;
    }

    if ( Distance(p, edp) < minh )
        return (double)i / (n - 1);
    else
        return (mini + mint) / (n - 1);
}

BOOL C_Disline2D::PointOn(Point2D p, double toler)
{
    double toler2 = toler * toler;
    int n = m_PointA.GetSize();
    Point2D stp = ( (C_Point2D *) m_PointA[0] )->GetPoint();
    if (Distance(p, stp) < toler)
        return TRUE;
    Point2D edp;
    Vector2D dir, v;
    double Len, d2;
    for (int i=1; i<n; i++)
    {
        edp = ( (C_Point2D *) m_PointA[i] )->GetPoint();
        if (Distance(p, edp) < toler)
            return TRUE;
        dir = edp - stp;
        Len = Length( dir );
        if ( Len < TOLER )
        {
            stp = edp;
            continue;
        }
        dir /= Len;
        v = p - stp;
        double d = v * dir;
        if ( d < toler || d > Len-toler )
        {
            stp = edp;
            continue;
        }
        d2 = v*v - d*d;
        if (d2 < toler*toler)
            return TRUE;
        stp = edp;
    }
    return FALSE;
}

C_Curve2D *C_Disline2D::Copy()
{
    int n = m_PointA.GetSize();
    C_Disline2D *newdis = new C_Disline2D ( n );
    for (int i=0; i<n; i++)
        newdis->SetDispoint( i, ((C_Point2D *)m_PointA[i])->Copy() );
    return newdis;
}

C_Curve2D *C_Disline2D::Reverse()
{
    int maxi = m_PointA.GetSize() - 1;
    int n = (maxi + 1) / 2;
    for (int i=0; i<n; i++)
    {
        C_Point2D *tmp = (C_Point2D *) m_PointA[i];
        m_PointA[i] = m_PointA[maxi - i];
        m_PointA[maxi - i] = tmp;
    }
    return this;
}

void C_Disline2D::SplitD(double t, C_Curve2D **pC1, C_Curve2D **pC2)
{
    if ( fabs(t) < TOLER || fabs(t-1.0) < TOLER)
    {
        *pC1 = *pC2 = NULL;
        return ;
    }
    int n = m_PointA.GetSize();
    int maxi = n - 1;
    double pos = t * maxi + TOLER;
    int i = (int) pos;
    C_Disline2D *dis1, *dis2;
    if ( pos-i < TOLER ) // 在顶点处断开
    {
        dis1 = new C_Disline2D ( i + 1 );
        for (int j=0; j<=i; j++)
            dis1->SetDispoint( j, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
        dis2 = new C_Disline2D ( n - i );
        for (int j=i; j<n; j++)
            dis2->SetDispoint( j-i, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
    }
    else   // 线性插值
    {
        Point2D p1 = ( (C_Point2D *) m_PointA[i] )->GetPoint();
        Point2D p2 = ( (C_Point2D *) m_PointA[i+1] )->GetPoint();
        Point2D p = p1 + (p2 - p1) * (pos - i);
        dis1 = new C_Disline2D ( i + 2 );
        for (int j=0; j<=i; j++)
            dis1->SetDispoint( j, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
        dis1->SetDispoint( i+1, new C_Point2D (p) );
        dis2 = new C_Disline2D ( n - i );
        dis2->SetDispoint( 0, new C_Point2D (p) );
        for (int j=i+1; j<n; j++)
            dis2->SetDispoint( j-i, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
    }
    *pC1 = dis1;
    *pC2 = dis2;
}

void C_Disline2D::SplitP(Point2D &p, C_Curve2D **pC1, C_Curve2D **pC2)
{
    Point2D stp, edp;
    Vector2D v1, v2;
    double h, t, minh, mint, Len;
    int i, j, n, mini;
    C_Disline2D *dis1, *dis2;

    minh = 1e+20;
    n = m_PointA.GetSize();
    stp = ( (C_Point2D *) m_PointA[0] )->GetPoint();
    for (i=1; i<n; i++)
    {
        h = Distance( p, stp );
        if ( h < minh )
        {
            minh = h;
            mini = i - 1;
            mint = 0.0;
            if ( minh < TOLER )
                break;
        }
        edp = ( (C_Point2D *) m_PointA[i] )->GetPoint();
        v1 = p - stp;
        v2 = edp - stp;
        Len = Length(v2);
        if ( Len > TOLER)
        {
            v2 /= Len;
            t = v1 * v2;
            if ( t >= 0.0 && t <= Len )
            {
                h = fabs( v1.x*v2.y - v1.y*v2.x );
                if ( h < minh )
                {
                    minh = h;
                    mini = i - 1;
                    mint = t / Len;
                    if ( minh < TOLER )
                        break;
                }
            }
        }

        stp = edp;
    }

    if ( (i == n && Distance(p, edp) < minh) || (mini == 0 && mint < TOLER) )
    {
        *pC1 = *pC2 = NULL;
        return ;
    }

    if ( mint < TOLER )    // 在小线段起点处断开
    {
        dis1 = new C_Disline2D ( mini + 1 );
        for (j=0; j<mini; j++)
            dis1->SetDispoint( j, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
        dis1->SetDispoint( mini, new C_Point2D (p) );
        dis2 = new C_Disline2D ( n - mini );
        dis2->SetDispoint( 0, new C_Point2D (p) );
        for (j=mini+1; j<n; j++)
            dis2->SetDispoint( j-mini, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
    }
    else if ( fabs(mint-1.0) < TOLER )   // 在小线段终点处断开
    {
        dis1 = new C_Disline2D ( mini + 2 );
        for (j=0; j<=mini; j++)
            dis1->SetDispoint( j, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
        dis1->SetDispoint( mini+1, new C_Point2D (p) );
        dis2 = new C_Disline2D ( n - mini - 1 );
        dis2->SetDispoint( 0, new C_Point2D (p) );
        for (j=mini+2; j<n; j++)
            dis2->SetDispoint( j-mini-1, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
    }
    else   // 在小线段中间断开
    {
        dis1 = new C_Disline2D ( mini + 2 );
        for (j=0; j<=mini; j++)
            dis1->SetDispoint( j, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
        dis1->SetDispoint( mini+1, new C_Point2D (p) );
        dis2 = new C_Disline2D ( n - mini );
        dis2->SetDispoint( 0, new C_Point2D (p) );
        for (j=mini+1; j<n; j++)
            dis2->SetDispoint( j-mini, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
    }
    *pC1 = dis1;
    *pC2 = dis2;
}

void C_Disline2D::SplitN(int n, double t[], Point2D p[], C_Curve2D *newcurve[])
{
    if ( t[0] < TOLER )
        return ;
    else if ( t[n-1] > 1.0-TOLER )
        return ;
    for (int k=1; k<n; k++)
        if ( t[k] <= t[k-1] ) return ;

    int i, j;
    double pos, step;
    Point2D p1, p2;
    C_Disline2D *dis;

    int maxi = m_PointA.GetSize() - 1;
    int lasti = 0;
    Point2D lastp = ( (C_Point2D *) m_PointA[0] )->GetPoint();
    for (int k=0; k<n; k++)
    {
        pos = t[k] * maxi + TOLER;
        i = (int) pos;
        step = pos - i;
        if ( step < TOLER ) // 在顶点处断开
        {
            dis = new C_Disline2D ( i - lasti + 1 );
            dis->SetDispoint( 0, new C_Point2D(lastp) );
            for (j=lasti+1; j<i; j++)
                dis->SetDispoint( j-lasti, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
            lastp = p[k];
            dis->SetDispoint( i-lasti, new C_Point2D(lastp) );
        }
        else   // 在小线段中间断开
        {
            dis = new C_Disline2D ( i - lasti + 2 );
            dis->SetDispoint( 0, new C_Point2D(lastp) );
            for (j=lasti+1; j<=i; j++)
                dis->SetDispoint( j-lasti, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
            lastp = p[k];
            dis->SetDispoint( i-lasti+1, new C_Point2D(lastp) );
        }
        newcurve[k] = dis;
        lasti = i;
    }

    dis = new C_Disline2D ( maxi - lasti + 1 );
    dis->SetDispoint( 0, new C_Point2D(lastp) );
    for (j=lasti+1; j<=maxi; j++)
        dis->SetDispoint( j-lasti, new C_Point2D ( (C_Point2D *)m_PointA[j] ) );
    newcurve[n] = dis;
}

Vector2D C_Disline2D::GetParaTangent(double t)
{
	Point2D		pt1,pt2;
	Vector2D	vn;
	int			ndispoint;
	int			i1;

	ndispoint = GetDispointNum();

	if ( ndispoint < 1 )
		return Vector2D (0, 0);
	else
	{
		if ( fabs(t) < TOLER )
			i1 = 0;
		else if ( fabs(t - 1) < TOLER )
			i1 = ndispoint - 2;
		else
		{
			t = t * (ndispoint - 1);
			if ( fabs(t - int(t)) < TOLER )
				i1 = int(t) - 1;
			else
				i1 = int(t);
		}
		pt1 = GetDispoint(i1)->GetPoint();
		pt2 = GetDispoint(i1 + 1)->GetPoint();
		if ( !SamePoint(pt1,pt2) )
			vn = pt2 - pt1;
		else
		{
			if ( i1 + 2 < ndispoint )
				i1 ++;
			else
				i1 --;
			pt1 = GetDispoint(i1)->GetPoint();
			pt2 = GetDispoint(i1 + 1)->GetPoint();
			vn = pt2 - pt1;
		}
		vn.SetUnit();
		return vn;
	}
}

void C_Disline2D::CalBox(double &umin, double &vmin, double &umax, double &vmax)
{
    int n = m_PointA.GetSize();
    Point2D p = ( (C_Point2D *) m_PointA[0] )->GetPoint();
    umin = umax = p.x;
    vmin = vmax = p.y;
    for (int i=1; i<n; i++)
    {
        p = ( (C_Point2D *) m_PointA[i] )->GetPoint();
        if (p.x < umin)
            umin = p.x;
        else if (p.x > umax)
            umax = p.x;
        if (p.y < vmin)
            vmin = p.y;
        else if (p.y > vmax)
            vmax = p.y;
    }
}
//得到点到直线上的投影点在直线上的参数
double GetParamOfProjectionPoint(const CS_Line2d line, const Point2D &pnt, double tol)
{
	Vector2D vec = line.vn;
	if(Length(vec)<tol)//线段退化为点
		return 0;

	double t;
	Vector2D vec2=pnt-line.pa;
	t= (vec*vec2)/(vec*vec);
	return t;
}
// 点、点求交   // 0 无交,1有交

int PointPointInters2d(Point2D  pnt1,Point2D pnt2,double tol)
{
	tol = fabs(tol);
	Vector2D vec = pnt1 - pnt2;
	if(Length(vec)<tol)
		return 1;

    return 0;
}
// 点、线求交   // 0 无交,1有交

int PointLineSegInters2d(Point2D pnt2d,CS_LineSeg2d lineseg2d,double tol)
{
	tol = fabs(tol);
	CS_Line2d line;
	line.pa = lineseg2d.pa;
	line.vn = lineseg2d.pb - lineseg2d.pa;
	
	double t=GetParamOfProjectionPoint(line,pnt2d,tol);
	
	if(fabs(t)<tol)
	{
		return PointPointInters2d(pnt2d,lineseg2d.pa);	
	}

	double len = Length(line.vn);
	if((t*len<=-tol) || ((t-1)*len>=tol))
		return 0;

	Point2D projPnt = lineseg2d.pa + line.vn * t;
	return PointPointInters2d(pnt2d,projPnt);	
}
/*线段与线段求交，返回值：
0： 无交
1： 一个交点
2:  一条线段
*/
int LineSegLineSegInters2d(CS_LineSeg2d lineseg1,CS_LineSeg2d lineseg2,Point2D &pnt2d,CS_LineSeg2d  &lineseg2d ,double tol)
{
    Point2D  p1,p2,pm,p3,p4,p0,tmp; 
    Vector2D va,vb,vc,v1,v2,v3,vacb; 
    CS_LineSeg2d tl;
    double  d1,d2,dm,dmin,len1,len2,t2;//,d,dmax
    int     i;//,j,ii;   
        
    va=lineseg1.pb-lineseg1.pa;
    vb=lineseg2.pb-lineseg2.pa;       
    len1=Length(va);
    len2=Length(vb);

    // 两条线段均退化为点
    if (len1 < tol && len2 < tol)
    {
        if ( Distance(lineseg1.pa,lineseg2.pa)<=tol )
        { 
            pnt2d = lineseg1.pa;
            return 1;
        }
        else 
            return 0; 
    }
    // 第一条线段退化为点
    else if (len1 < tol)
    {
        pm = MidPoint(lineseg1.pa, lineseg1.pb);
        if (PointLineSegInters2d(pm, lineseg2)==1)
        {
            pnt2d = pm;
            return 1;
        }
        else
            return 0;
    }
    // 第二条线段退化为点
    else if (len2 < tol)
    {
        pm = MidPoint(lineseg2.pa, lineseg2.pb);
        if (PointLineSegInters2d(pm, lineseg1))
        {
            pnt2d = pm;
            return 1;
        }
        else
            return 0;
    }
    // 两条线段都不退化为点
    else
    {  
        p1=lineseg1.pa;
        p2=lineseg1.pb;                   
        pm=MidPoint(lineseg1.pa,lineseg1.pb);           
          
        p3=lineseg2.pa;
        p4=lineseg2.pb;    
        
        va.SetUnit();
        vb.SetUnit(); 
          
        // 判两条线段是否平行
        // 两条线段不平行
        if (fabs(fabs(va*vb)-1) >= tol )
        {            
            t2=p1[0]*va[1]-p1[1]*va[0]-p3[0]*va[1]+p3[1]*va[0];
            t2=t2/(vb[0]*va[1]-va[0]*vb[1]);
            p0=p3+vb*t2; 
            pnt2d = p0;
            
			// 判交点是否在线段2上
			if( (t2<-tol) || (t2>len2+tol) )
				return 0;
			
			// 判交点是否在线段1上
            if(fabs(va.x)>tol)
			{
				if(va.x>tol)
				{
					if((p0.x>=p1.x-len1*tol)&&(p0.x<=p2.x+len1*tol))
						return 1;
					else
						return 0;
				}
				else
				{
					if((p0.x>=p2.x-len1*tol)&&(p0.x<=p1.x+len1*tol))
						return 1;
					else
						return 0;
				}
			}
			else
			{
				if(va.y>tol)
				{
					if((p0.y>=p1.y-len1*tol)&&(p0.y<=p2.y+len1*tol))
						return 1;
					else
						return 0;
				}
				else
				{
					if((p0.y>=p2.y-len1*tol)&&(p0.y<=p1.y+len1*tol))
						return 1;
					else
						return 0;
				}
			}
        }  
  
        // 两条线段平行
        else         
        {   
            v1=p1-lineseg2.pa;
            d1=fabs(vb.x*v1.y - vb.y*v1.x);
			dmin = d1;
                  
            v2=p2-lineseg2.pa;
            d2=fabs(vb.x*v2.y - vb.y*v2.x);
			if(d2<dmin)
				dmin = d2;
                                   
            v3=pm-lineseg2.pa;
            dm=fabs(vb.x*v3.y - vb.y*v3.x);
			if(dm<dmin)
				dmin = dm;
                              
            // 两线段不重合
            if ( dmin > tol )
                return 0;
            // 两线段重合
            else 
            {
				
                if(fabs(va.x)>fabs(va.y))
					i=0;	
				else
					i=1;

				if (p2[i]<p1[i])
                {
                    tmp=p2;
                    p2=p1;
                    p1=tmp;
                }
                if (p4[i]<p3[i])
                {
                    tmp=p4;
                    p4=p3;
                    p3=tmp; 
                }
                            
                if ( Length(p2-p3) < tol )
                { 
                    pnt2d=p3; 
                    return 1;
                }

                else if ( Length(p1-p4) < tol)
                {
                    pnt2d=p4; 
                    return 1;
                }
                else if ( p2[i] < p3[i] )
                    return 0;
                else if ( p1[i] > p4[i] )
                    return 0; 
                else  
                {
                    if (p1[i]>p3[i])
                        lineseg2d.pa=p1;
                    else 
                        lineseg2d.pa=p3; 
                        
                    if (p2[i]<p4[i])
                        lineseg2d.pb=p2;
                    else 
                        lineseg2d.pb=p4; 
                    if (Distance(lineseg2d.pa,lineseg2d.pb)<tol)
                    {
                        pnt2d=lineseg2d.pb;
                        return 1;
                    }  
                    else  
                        return 2;
                } 
            }     
        }
    }    

}
int DislineDisline2DInters(C_Disline2D **disline,Point2D *pt)
{
	CS_LineSeg2d	lineseg[2];
	Point2D		start[2],end[2];
	int			segnum[2];
	int			i,intnum;
	int			internum;
	Point2D		interPt;
	CS_LineSeg2d	interlineseg;

	internum = 0;
	for ( segnum[0] = 0; segnum[0] < disline[0]->GetDispointNum()-1; segnum[0]++ )
	{
		for ( segnum[1] = 0; segnum[1] < disline[1]->GetDispointNum()-1; segnum[1]++ )
		{
			for ( i = 0; i < 2; i++ )
			{
				start[i] = disline[i]->GetDispoint(segnum[i])->GetPoint();
				end[i] = disline[i]->GetDispoint(segnum[i]+1)->GetPoint();
				if ( SamePoint(start[i],end[i]) )
					continue;
				lineseg[i].pa = Point2D(start[i].x,start[i].y);
				lineseg[i].pb = Point2D(end[i].x,end[i].y);
			}
			intnum = LineSegLineSegInters2d(lineseg[0],lineseg[1],interPt,interlineseg);
			if ( intnum > 0 )
			{
				ASSERT(intnum == 1);
				pt[internum] = Point2D(interPt.x,interPt.y);
				internum ++;
				ASSERT(internum <= 16);
			}
		}
	}
	return internum;
}



