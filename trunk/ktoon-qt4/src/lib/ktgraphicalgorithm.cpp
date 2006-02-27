/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
 
/*
	An Algorithm for Automatically Fitting Digitized Curves
	by Philip J. Schneider
	from "Graphics Gems", Academic Press, 1990

	http://www.acm.org/pubs/tog/GraphicsGems/gems/FitCurves.c
	http://www.acm.org/pubs/tog/GraphicsGems/gems/README
*/


#include "ktgraphicalgorithm.h"
#include <cmath>


#include "ktdebug.h"


#define MAXPOINTS	1000		/* The most points you can have */


class FitVector
{
	public:
		FitVector(QPointF &p)
		{
			m_X=p.x();
			m_Y=p.y();
		}
		
		FitVector()
		{
			m_X=0;
			m_Y=0;
		}
	
		FitVector(QPointF &a, QPointF &b)
		{
			m_X=a.x()-b.x();
			m_Y=a.y()-b.y();
		}
	
		void normalize()
		{
			double len=length();
			if(len==0.0f)
				return;
			m_X/=len; m_Y/=len;
		}
	
		void negate()
		{
			m_X = -m_X;
			m_Y = -m_Y;
		}
	
		void scale(double s)
		{
			double len = length();
			if(len==0.0f)
				return;
			m_X *= s/len;
			m_Y *= s/len;
		}
	
		double dot(FitVector &v)
		{
			return ((m_X*v.m_X)+(m_Y*v.m_Y));
		}
	
		double length()
		{
			return (double) sqrt(m_X*m_X+m_Y*m_Y); 
		}
	
		QPointF operator+(QPointF &p)
		{
			QPointF b(p.x()+m_X,p.y()+m_Y);
			return b;
		}

	public:
		double m_X,m_Y;
};

double distance(const QPointF& p1, const QPointF&  p2)
{
	double dx = (p1.x()-p2.x());
	double dy = (p1.y()-p2.y());
	return sqrt( dx*dx + dy*dy );
}


FitVector computeLeftTangent(QPolygonF &points, int end)
{
	FitVector tHat1((points[end+1]), (points[end]));
	tHat1.normalize();
	return tHat1;
}

FitVector computeRightTangent(QPolygonF &points,int end)
{
	FitVector tHat1(points[end-1],points[end]);
	tHat1.normalize();
	return tHat1;
}

/*
 *  chordLengthParameterize :
 *	Assign parameter values to digitized points 
 *	using relative distances between points.
 */
double *chordLengthParameterize(QPolygonF points,int first,int last)
{
	int i;	
	double	*u; /*  Parameterization */

	u = new double[(last-first+1)];

	u[0] = 0.0;
	for (i = first+1; i <= last; i++) 
	{
		u[i-first] = u[i-first-1] + distance(points[i], points[i-1]);
	}

	for (i = first + 1; i <= last; i++)
	{
		int denominator = u[last-first];
		
		SHOW_VAR(denominator);
		if ( denominator != 0.0f)
		{
			u[i-first] = u[i-first] / denominator;
		}
	}

	return(u);
}

FitVector vectorAdd(FitVector a,FitVector b)
{
    FitVector	c;
    c.m_X = a.m_X + b.m_X;  c.m_Y = a.m_Y + b.m_Y;
    return (c);
}

FitVector vectorScale(FitVector v,double s)
{
    FitVector result;
    result.m_X = v.m_X * s; result.m_Y = v.m_Y * s;
    return (result);
}

FitVector vectorSub(FitVector a,FitVector b)
{
    FitVector	c;
    c.m_X = a.m_X - b.m_X; c.m_Y = a.m_Y - b.m_Y;
    return (c);
}

FitVector computeCenterTangent(QPolygonF points,int center)
{
    FitVector V1, V2, tHatCenter;
    
    FitVector cpointb = points[center-1];
    FitVector cpoint = points[center];
    FitVector cpointa = points[center+1];

    V1 = vectorSub(cpointb,cpoint);
    V2 = vectorSub(cpoint,cpointa);
    tHatCenter.m_X= ((V1.m_X + V2.m_X)/2.0);
    tHatCenter.m_Y= ((V1.m_Y + V2.m_Y)/2.0);
    tHatCenter.normalize();
    return tHatCenter;
}

/**
 *  b0, b1, b2, b3 :
 *	Bezier multipliers
 */
double b0(double u)
{
    double tmp = 1.0 - u;
    return (tmp * tmp * tmp);
}


double b1(double u)
{
    double tmp = 1.0 - u;
    return (3 * u * (tmp * tmp));
}

double b2(double u)
{
    double tmp = 1.0 - u;
    return (3 * u * u * tmp);
}

double b3(double u)
{
    return (u * u * u);
}

/**
 *  generateBezier :
 *  Use least-squares method to find Bezier control points for region.
 *
 */
QPointF* generateBezier(QPolygonF &points, int first, int last, double *uPrime,FitVector tHat1,FitVector tHat2)
{
	
	int i;
	FitVector A[MAXPOINTS][2];	/* Precomputed rhs for eqn	*/
	int nPts;			/* Number of pts in sub-curve */
	double C[2][2];			/* Matrix C		*/
	double X[2];			/* Matrix X			*/
	double det_C0_C1,		/* Determinants of matrices	*/
	det_C0_X,
	det_X_C1;
	double alpha_l,		/* Alpha values, left and right	*/
	alpha_r;
	FitVector tmp;			/* Utility variable		*/
	QPointF *curve;
	
	curve = new QPointF[4];
	nPts = last - first + 1;

 
	/* Compute the A's	*/
	for (i = 0; i < nPts; i++) 
	{
		FitVector v1, v2;
		v1 = tHat1;
		v2 = tHat2;
		v1.scale(b1(uPrime[i]));
		v2.scale(b2(uPrime[i]));
		A[i][0] = v1;
		A[i][1] = v2;
	}

	/* Create the C and X matrices	*/
	C[0][0] = 0.0;
	C[0][1] = 0.0;
	C[1][0] = 0.0;
	C[1][1] = 0.0;
	X[0]    = 0.0;
	X[1]    = 0.0;

	for (i = 0; i < nPts; i++) {
		C[0][0] += (A[i][0]).dot(A[i][0]);
		C[0][1] += A[i][0].dot(A[i][1]);
		/* C[1][0] += V2Dot(&A[i][0], &A[i][1]);*/	
		C[1][0] = C[0][1];
		C[1][1] += A[i][1].dot(A[i][1]);

		FitVector vfirstp1(points[first+i]);
		FitVector vfirst(points[first]);
		FitVector vlast(points[last]);

		tmp = vectorSub(vfirstp1,
				vectorAdd( vectorScale(vfirst, b0(uPrime[i])),
				vectorAdd( vectorScale(vfirst, b1(uPrime[i])),
				vectorAdd( vectorScale(vlast, b2(uPrime[i])),
				vectorScale(vlast, b3(uPrime[i])) ))));
	
		X[0] += A[i][0].dot(tmp);
		X[1] += A[i][1].dot(tmp);
	}

	/* Compute the determinants of C and X	*/
	det_C0_C1 = C[0][0] * C[1][1] - C[1][0] * C[0][1];
	det_C0_X  = C[0][0] * X[1]    - C[0][1] * X[0];
	det_X_C1  = X[0]    * C[1][1] - X[1]    * C[0][1];

	/* Finally, derive alpha values	*/
	if (det_C0_C1 == 0.0)
	{
		det_C0_C1 = (C[0][0] * C[1][1]) * 10e-12;
	}
	
	SHOW_VAR(det_C0_C1);
	if ( det_C0_C1 != 0.0f)
	{
		alpha_l = det_X_C1 / det_C0_C1;
		alpha_r = det_C0_X / det_C0_C1;
	}


	/*  If alpha negative, use the Wu/Barsky heuristic (see text) */
	/* (if alpha is 0, you get coincident control points that lead to
	* divide by zero in any subsequent newtonRaphsonRootFind() call. */
	if (alpha_l < 1.0e-6 || alpha_r < 1.0e-6)
	{
		double	dist = distance(points[last], points[first]) / 3.0;

		curve[0] = points[first];
		curve[3] = points[last];

		tHat1.scale(dist);
		tHat2.scale(dist);

		curve[1] = tHat1 + curve[0];
		curve[2] = tHat2 + curve[3];
		return curve;
	}

	/*  First and last control points of the Bezier curve are */
	/*  positioned exactly at the first and last data points */
	/*  Control points 1 and 2 are positioned an alpha distance out */
	/*  on the tangent vectors, left and right, respectively */
	curve[0] = points[first];
	curve[3] = points[last];

	tHat1.scale(alpha_l);
	tHat2.scale(alpha_r);

	curve[1] = tHat1 + curve[0];
	curve[2] = tHat2 + curve[3];
    
	return (curve);
}

/*
 *  Bezier :
 *  	Evaluate a Bezier curve at a particular parameter value
 * 
 */
QPointF bezierII(int degree,QPointF *V, double t)
{
	int i, j;
	QPointF Q;	        /* Point on curve at parameter t	*/
	QPointF *Vtemp;		/* Local copy of control points		*/

	Vtemp = new QPointF[degree+1];
    
	for (i = 0; i <= degree; i++) 
	{
		Vtemp[i] = V[i];
	}

	/* Triangle computation	*/
	for (i = 1; i <= degree; i++) 
	{
		for (j = 0; j <= degree-i; j++) 
		{
			Vtemp[j].setX((1.0 - t) * Vtemp[j].x() + t * Vtemp[j+1].x());
			Vtemp[j].setY((1.0 - t) * Vtemp[j].y() + t * Vtemp[j+1].y());
		}
	}

	Q = Vtemp[0];
	delete[] Vtemp;
	return Q;
}

/*
 *  computeMaxError :
 *	Find the maximum squared distance of digitized points
 *	to fitted curve.
*/
double computeMaxError(QPolygonF points,int first,int last,QPointF *curve,double *u,int *splitPoint)
{
	int i;
	double maxDist; /*  Maximum error		*/
	double dist; /*  Current error		*/
	QPointF P; /*  Point on curve		*/
	FitVector v; /*  Vector from point to curve	*/
	
	*splitPoint = (last - first + 1)/2;
	maxDist = 0.0;
	for (i = first + 1; i < last; i++)
	{
		P = bezierII(3, curve, u[i-first]);
		v = vectorSub(P, points[i]);
		dist = v.length();
		if (dist >= maxDist) 
		{
			maxDist = dist;
			*splitPoint = i;
		}
	}
	return (maxDist);
}


/**
 *  newtonRaphsonRootFind :
 *	Use Newton-Raphson iteration to find better root.
 */
double newtonRaphsonRootFind(QPointF *Q,QPointF P,double u)
{
	double  numerator, denominator;
	QPointF Q1[3], Q2[2];	/*  Q' and Q''			*/
	QPointF Q_u, Q1_u, Q2_u; /*u evaluated at Q, Q', & Q''	*/
	double  uPrime;		/*  Improved u			*/
	int i;
    
	/* Compute Q(u)	*/
	Q_u = bezierII(3,Q, u);
    
	/* Generate control vertices for Q'	*/
	for (i = 0; i <= 2; i++)
	{
		Q1[i].setX((Q[i+1].x() - Q[i].x()) * 3.0);
		Q1[i].setY((Q[i+1].y() - Q[i].y()) * 3.0);
	}
    
	/* Generate control vertices for Q'' */
	for (i = 0; i <= 1; i++)
	{
		Q2[i].setX((Q1[i+1].x() - Q1[i].x()) * 2.0);
		Q2[i].setY((Q1[i+1].y() - Q1[i].y()) * 2.0);
	}
 
	/* Compute Q'(u) and Q''(u)	*/
	Q1_u = bezierII(2, Q1, u);
	Q2_u = bezierII(1, Q2, u);

	/* Compute f(u)/f'(u) */
	numerator = (Q_u.x() - P.x()) * (Q1_u.x()) + (Q_u.y() - P.y()) * (Q1_u.y());
	denominator = (Q1_u.x()) * (Q1_u.x()) + (Q1_u.y()) * (Q1_u.y()) +
			(Q_u.x() - P.x()) * (Q2_u.x()) + (Q_u.y() - P.y()) * (Q2_u.y());

	/* u = u - f(u)/f'(u) */
	if ( denominator != 0.0f )
	{
		ktDebug() << "OTHER: " << denominator;
		uPrime = u - (numerator/denominator);
	}

	return (uPrime);
}

/*
 *  reparameterize:
 *	Given set of points and their parameterization, try to find
 *   a better parameterization.
 *
 */
double *reparameterize(QPolygonF points,int first,int last,double *u,QPointF *curve)
{
	
	int nPts = last-first+1;
	int i;
	double *uPrime;/*  New parameter values	*/

	uPrime = new double[nPts];
	for (i = first; i <= last; i++)
	{
		uPrime[i-first] = newtonRaphsonRootFind(curve, points[i], u[i- first]);
	}
	return (uPrime);
}

QPointF *fitCubic(QPolygonF &points,int first,int last,FitVector tHat1,FitVector tHat2,float error,int &width)
{
	double *u;
	double *uPrime;
	double maxError;
	int splitPoint;
	int nPts;
	double iterationError;
	int maxIterations=4;
	FitVector tHatCenter;
	QPointF *curve;
	int i;
	width=0;
	iterationError=error*error;
	nPts = last-first+1;
	if(nPts == 2)
	{
		double dist = distance(points[last], points[first]) / 3.0;

		curve = new QPointF[4];
		
		curve[0] = points[first];
		curve[3] = points[last];

		tHat1.scale(dist);
		tHat2.scale(dist);
		curve[1] = tHat1 + curve[0];
		curve[2] = tHat2 + curve[3];
		width=4;	
		return curve;
	}
    
	/*  Parameterize points, and attempt to fit curve */
	u = chordLengthParameterize(points, first, last);
	curve = generateBezier(points, first, last, u, tHat1, tHat2);


	/*  Find max deviation of points to fitted curve */
	maxError = computeMaxError(points, first, last, curve, u, &splitPoint);
	if (maxError < error)
	{
		delete[] u;
		width=4;	
		return curve;
	}


	/*  If error not too large, try some reparameterization  */
	/*  and iteration */
	if (maxError < iterationError) 
	{
		for (i = 0; i < maxIterations; i++) 
		{
			uPrime = reparameterize(points, first, last, u, curve);
			curve = generateBezier(points, first, last, uPrime, tHat1, tHat2);
			maxError = computeMaxError(points, first, last,
					curve, uPrime, &splitPoint);
			if (maxError < error) {
				delete[] u;
				width=4;	
				return curve;
			}
			delete[] u;
			u = uPrime;
		}
	}

	/* Fitting failed -- split at max error point and fit recursively */
	delete[] u;
	delete[] curve;
	tHatCenter = computeCenterTangent(points, splitPoint);

	int w1,w2;
	QPointF *cu1=NULL, *cu2=NULL;
	cu1 = fitCubic(points, first, splitPoint, tHat1, tHatCenter, error,w1);

	tHatCenter.negate();
	cu2 = fitCubic(points, splitPoint, last, tHatCenter, tHat2, error,w2);

	QPointF *newcurve = new QPointF[w1+w2];
	for(int i=0;i<w1;i++)
	{
		newcurve[i]=cu1[i];
	}
	for(int i=0;i<w2;i++)
	{
		newcurve[i+w1]=cu2[i];
	}
	
	delete[] cu1;
	delete[] cu2;
	width=w1+w2;
	return newcurve;
}


QPainterPath KTGraphicalAlgorithm::bezierFit( QPolygonF &points,float error)
{
	FitVector tHat1, tHat2;

	tHat1 = computeLeftTangent(points,0);
	tHat2 = computeRightTangent(points, points.count()-1);
	
	int width=0;
	QPointF *curve;
	 
	curve = fitCubic(points,0,points.count()-1,tHat1,tHat2,error,width);
	
	QPainterPath path;
	
	path = QPainterPath();
	
	if(width>3)
	{
		path.moveTo(curve[0]);
		path.cubicTo(curve[1], curve[2],curve[3] );
		for(int i=4;i<width;i+=4)
		{
			path.cubicTo(curve[i+1],curve[i+2],curve[i+3]);	
		}
	}
	
	delete[] curve;
	return path;
}

