/*************************************************************************
Copyright (c) Sergey Bochkanov (ALGLIB project).

>>> SOURCE LICENSE >>>
This program is a trial version of the ALGLIB package  licensed
to shandong university (Licensee, You).

Only Licensee can use  it  according   to   the  ALGLIB   Trial
License Agreement between Licensor (Sole  Proprietor  Bochkanov
Sergey Anatolyevich) and Licensee.

=============== ALGLIB TRIAL LICENSE AGREEMENT ================

1. Only owners of e-mails below can use this trial version:
* yangyijuny@gmail.com

2. This trial version can be used only for 30 days  (expiration
date is 09 Nov, 2013).

It can be used only for evaluation  purposes. You can  not  use
it to perform some "real" work.

After this trial period is over, you MUST delete all  copies of
software (including ones made for backup  purposes)  -  or  buy
commercial license for ALGLIB.

3. Following restrictions are applied:
A. Trial version of ALGLIB can not be used by other individuals
   or legal entities. It can not  be  distributed  in  any  way
   (including rent, lease, other means of sharing software).
B. Any application/product  developed  with  trial  version  of
   ALGLIB may only be used for  evaluation  purposes  and  only
   during evaluation period.
   You may not  distribute  applications/products  linked  with
   trial version of ALGLIB.
   After evaluation period  is  over, you should  re-link  such
   applications/products  with  Free  Edition  or  one  of  the
   Commercial editions of ALGLIB - or delete them.
C. You can not remove any  copyright  notice  from  the  Source
   Codes/Binary Files
   
WARRANTIES:

This trial version  is  provided  AS IS,  with  no  warranties,
express or implied.  Licensor  does  NOT  provide  support  and
maintenance for this trial version. If you need warranties  and
support, you should buy commercial version of ALGLIB.

COPYRIGHT:

Title to the ALGLIB and all copies thereof remain with Licensor
The ALGLIB is copyrighted and is protected by Russian copyright
laws and international treaty provisions.  You  will not remove
any copyright notice  from  the  ALGLIB  files.  You  agree  to
prevent any unauthorized  copying  of  the  ALGLIB.  Except  as
expressly provided herein, Licensor does not grant any  express
or implied right to you  under  Licensor  patents,  copyrights,
trademarks, or trade secret information.
>>> END OF LICENSE >>>
*************************************************************************/
#ifndef _diffequations_pkg_h
#define _diffequations_pkg_h
#include "ap.h"
#include "alglibinternal.h"

/////////////////////////////////////////////////////////////////////////
//
// THIS SECTION CONTAINS COMPUTATIONAL CORE DECLARATIONS (DATATYPES)
//
/////////////////////////////////////////////////////////////////////////
namespace alglib_impl
{
typedef struct
{
    ae_int_t n;
    ae_int_t m;
    double xscale;
    double h;
    double eps;
    ae_bool fraceps;
    ae_vector yc;
    ae_vector escale;
    ae_vector xg;
    ae_int_t solvertype;
    ae_bool needdy;
    double x;
    ae_vector y;
    ae_vector dy;
    ae_matrix ytbl;
    ae_int_t repterminationtype;
    ae_int_t repnfev;
    ae_vector yn;
    ae_vector yns;
    ae_vector rka;
    ae_vector rkc;
    ae_vector rkcs;
    ae_matrix rkb;
    ae_matrix rkk;
    rcommstate rstate;
} odesolverstate;
typedef struct
{
    ae_int_t nfev;
    ae_int_t terminationtype;
} odesolverreport;

}

/////////////////////////////////////////////////////////////////////////
//
// THIS SECTION CONTAINS C++ INTERFACE
//
/////////////////////////////////////////////////////////////////////////
namespace alglib
{

/*************************************************************************

*************************************************************************/
class _odesolverstate_owner
{
public:
    _odesolverstate_owner();
    _odesolverstate_owner(const _odesolverstate_owner &rhs);
    _odesolverstate_owner& operator=(const _odesolverstate_owner &rhs);
    virtual ~_odesolverstate_owner();
    alglib_impl::odesolverstate* c_ptr();
    alglib_impl::odesolverstate* c_ptr() const;
protected:
    alglib_impl::odesolverstate *p_struct;
};
class odesolverstate : public _odesolverstate_owner
{
public:
    odesolverstate();
    odesolverstate(const odesolverstate &rhs);
    odesolverstate& operator=(const odesolverstate &rhs);
    virtual ~odesolverstate();
    ae_bool &needdy;
    real_1d_array y;
    real_1d_array dy;
    double &x;

};


/*************************************************************************

*************************************************************************/
class _odesolverreport_owner
{
public:
    _odesolverreport_owner();
    _odesolverreport_owner(const _odesolverreport_owner &rhs);
    _odesolverreport_owner& operator=(const _odesolverreport_owner &rhs);
    virtual ~_odesolverreport_owner();
    alglib_impl::odesolverreport* c_ptr();
    alglib_impl::odesolverreport* c_ptr() const;
protected:
    alglib_impl::odesolverreport *p_struct;
};
class odesolverreport : public _odesolverreport_owner
{
public:
    odesolverreport();
    odesolverreport(const odesolverreport &rhs);
    odesolverreport& operator=(const odesolverreport &rhs);
    virtual ~odesolverreport();
    ae_int_t &nfev;
    ae_int_t &terminationtype;

};

/*************************************************************************
Cash-Karp adaptive ODE solver.

This subroutine solves ODE  Y'=f(Y,x)  with  initial  conditions  Y(xs)=Ys
(here Y may be single variable or vector of N variables).

INPUT PARAMETERS:
    Y       -   initial conditions, array[0..N-1].
                contains values of Y[] at X[0]
    N       -   system size
    X       -   points at which Y should be tabulated, array[0..M-1]
                integrations starts at X[0], ends at X[M-1],  intermediate
                values at X[i] are returned too.
                SHOULD BE ORDERED BY ASCENDING OR BY DESCENDING!!!!
    M       -   number of intermediate points + first point + last point:
                * M>2 means that you need both Y(X[M-1]) and M-2 values at
                  intermediate points
                * M=2 means that you want just to integrate from  X[0]  to
                  X[1] and don't interested in intermediate values.
                * M=1 means that you don't want to integrate :)
                  it is degenerate case, but it will be handled correctly.
                * M<1 means error
    Eps     -   tolerance (absolute/relative error on each  step  will  be
                less than Eps). When passing:
                * Eps>0, it means desired ABSOLUTE error
                * Eps<0, it means desired RELATIVE error.  Relative errors
                  are calculated with respect to maximum values of  Y seen
                  so far. Be careful to use this criterion  when  starting
                  from Y[] that are close to zero.
    H       -   initial  step  lenth,  it  will  be adjusted automatically
                after the first  step.  If  H=0,  step  will  be  selected
                automatically  (usualy  it  will  be  equal  to  0.001  of
                min(x[i]-x[j])).

OUTPUT PARAMETERS
    State   -   structure which stores algorithm state between  subsequent
                calls of OdeSolverIteration. Used for reverse communication.
                This structure should be passed  to the OdeSolverIteration
                subroutine.

SEE ALSO
    AutoGKSmoothW, AutoGKSingular, AutoGKIteration, AutoGKResults.


  -- ALGLIB --
     Copyright 01.09.2009 by Bochkanov Sergey
*************************************************************************/
void odesolverrkck(const real_1d_array &y, const ae_int_t n, const real_1d_array &x, const ae_int_t m, const double eps, const double h, odesolverstate &state);
void odesolverrkck(const real_1d_array &y, const real_1d_array &x, const double eps, const double h, odesolverstate &state);


/*************************************************************************
This function provides reverse communication interface
Reverse communication interface is not documented or recommended to use.
See below for functions which provide better documented API
*************************************************************************/
bool odesolveriteration(const odesolverstate &state);


/*************************************************************************
This function is used to launcn iterations of ODE solver

It accepts following parameters:
    diff    -   callback which calculates dy/dx for given y and x
    ptr     -   optional pointer which is passed to diff; can be NULL


  -- ALGLIB --
     Copyright 01.09.2009 by Bochkanov Sergey

*************************************************************************/
void odesolversolve(odesolverstate &state,
    void (*diff)(const real_1d_array &y, double x, real_1d_array &dy, void *ptr),
    void *ptr = NULL);


/*************************************************************************
ODE solver results

Called after OdeSolverIteration returned False.

INPUT PARAMETERS:
    State   -   algorithm state (used by OdeSolverIteration).

OUTPUT PARAMETERS:
    M       -   number of tabulated values, M>=1
    XTbl    -   array[0..M-1], values of X
    YTbl    -   array[0..M-1,0..N-1], values of Y in X[i]
    Rep     -   solver report:
                * Rep.TerminationType completetion code:
                    * -2    X is not ordered  by  ascending/descending  or
                            there are non-distinct X[],  i.e.  X[i]=X[i+1]
                    * -1    incorrect parameters were specified
                    *  1    task has been solved
                * Rep.NFEV contains number of function calculations

  -- ALGLIB --
     Copyright 01.09.2009 by Bochkanov Sergey
*************************************************************************/
void odesolverresults(const odesolverstate &state, ae_int_t &m, real_1d_array &xtbl, real_2d_array &ytbl, odesolverreport &rep);
}

/////////////////////////////////////////////////////////////////////////
//
// THIS SECTION CONTAINS COMPUTATIONAL CORE DECLARATIONS (FUNCTIONS)
//
/////////////////////////////////////////////////////////////////////////
namespace alglib_impl
{
void odesolverrkck(/* Real    */ ae_vector* y,
     ae_int_t n,
     /* Real    */ ae_vector* x,
     ae_int_t m,
     double eps,
     double h,
     odesolverstate* state,
     ae_state *_state);
ae_bool odesolveriteration(odesolverstate* state, ae_state *_state);
void odesolverresults(odesolverstate* state,
     ae_int_t* m,
     /* Real    */ ae_vector* xtbl,
     /* Real    */ ae_matrix* ytbl,
     odesolverreport* rep,
     ae_state *_state);
ae_bool _odesolverstate_init(void* _p, ae_state *_state, ae_bool make_automatic);
ae_bool _odesolverstate_init_copy(void* _dst, void* _src, ae_state *_state, ae_bool make_automatic);
void _odesolverstate_clear(void* _p);
void _odesolverstate_destroy(void* _p);
ae_bool _odesolverreport_init(void* _p, ae_state *_state, ae_bool make_automatic);
ae_bool _odesolverreport_init_copy(void* _dst, void* _src, ae_state *_state, ae_bool make_automatic);
void _odesolverreport_clear(void* _p);
void _odesolverreport_destroy(void* _p);

}
#endif

