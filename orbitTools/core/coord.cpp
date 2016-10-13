//
// coord.cpp
//
// Copyright (c) 2003-2013 Michael F. Henry
// Version 01/2013
//
#include "stdafx.h"

#include "coord.h"
#include "cEci.h"
#include "cEcef.h"

namespace Zeptomoby 
{
namespace OrbitTools 
{

//////////////////////////////////////////////////////////////////////
// cGeo Class
//////////////////////////////////////////////////////////////////////

cGeo::cGeo(double latRad, double lonRad, double altKm)
   : m_Lat(latRad), 
     m_Lon(lonRad), 
     m_Alt(altKm)
{
}

cGeo::cGeo(const cEci& eci, cJulian date)
{
   Construct(eci.Position(),
             fmod((AcTan(eci.Position().m_y, eci.Position().m_x) - date.ToGmst()), TWOPI));
}

cGeo::cGeo(const cEcef& ecef, cJulian date)
{
	ConstructEcef(ecef.Position());
}

//
// By D. Rose - November, 2014
// http://danceswithcode.net/engineeringnotes/geodetic_to_ecef/geodetic_to_ecef.html
//
void cGeo::ConstructEcef(const cVector &posEcf)
{
	double x = posEcf.m_x, y = posEcf.m_y, z = posEcf.m_z;
	double a = XKMPER_WGS84;
	double e2 = F * (2.0 - F);
	double a1 = a*e2, a2 = a1*a1, a3 = a1*e2 / 2.0,
		   a4 = 2.5*a2, a5 = a1 + a3, a6 = 1 - e2;
	double zp, w2, w, r2, r, s2, c2, s, c, ss;
	double g, rg, rf, u, v, m, f, p;
	double lat;

	zp = abs(z);
	w2 = x*x + y*y;
	w = sqrt(w2);
	r2 = w2 + z*z;
	r = sqrt(r2);
	s2 = z*z / r2;
	c2 = w2 / r2;
	u = a2 / r;
	v = a3 - a4 / r;

	if (c2 > 0.3) {
		s = (zp / r)*(1.0 + c2*(a1 + u + s2*v) / r);
		lat = asin(s);      //Lat
		ss = s*s;
		c = sqrt(1.0 - ss);
	}
	else {
		c = (w / r)*(1.0 - s2*(a5 - u - c2*v) / r);
		lat = acos(c);      //Lat
		ss = 1.0 - c*c;
		s = sqrt(ss);
	}
	g = 1.0 - e2*ss;
	rg = a/sqrt(g);
	rf = a6*rg;
	u = w - rg*c;
	v = zp - rf*s;
	f = c*u + s*v;
	m = c*v - s*u;
	p = m / (rf / g + f);
	lat += p;
	if (z < 0.0) {
		lat *= -1.0;
	}
	m_Lat = lat;
	m_Lon = AcTan(y,x);		//Lon
	m_Alt = f + m*p / 2.0;
}

void cGeo::Construct(const cVector &posEcf, double theta)
{
   theta = fmod(theta, TWOPI);
   
   if (theta  < 0.0) 
   {
      theta += TWOPI;  // "wrap" negative modulo
   }

   double kmSemiMaj = XKMPER_WGS84;

   double r   = sqrt(sqr(posEcf.m_x) + sqr(posEcf.m_y));
   double e2  = F * (2.0 - F);
   double lat = AcTan(posEcf.m_z, r);

   const double delta = 1.0e-07;
   double phi;
   double c;

   do   
   {
      phi = lat;
      c   = 1.0 / sqrt(1.0 - e2 * sqr(sin(phi)));
      lat = AcTan(posEcf.m_z + kmSemiMaj * c * e2 * sin(phi), r);
   }
   while (fabs(lat - phi) > delta);
   
   m_Lat = lat;
   m_Lon = theta;
   m_Alt = r / cos(lat) - kmSemiMaj * c;
}

// Converts to a string representation of the form "38.0N 045.0W 500m".
string cGeo::ToString() const
{
   const int BUF_SIZE = 128;
   char sz[BUF_SIZE];

   bool isNorth = (LatitudeRad()  >= 0.0);
   bool isEast  = (LongitudeRad() >= 0.0);

   _snprintf_s(sz, BUF_SIZE, 
               "%04.3f%c %05.3f%c %.1fm", 
               fabs(LatitudeDeg()),  (isNorth ? 'N' : 'S'),
               fabs(LongitudeDeg()), (isEast  ? 'E' : 'W'),
               AltitudeKm() * 1000.0);

   string strLoc = sz;

   return strLoc;
}

//////////////////////////////////////////////////////////////////////
// cGeoTime Class
//////////////////////////////////////////////////////////////////////
cGeoTime::cGeoTime(const cGeo &geo, cJulian date)
   : cGeo(geo),
     m_Date(date)
{
}

cGeoTime::cGeoTime(double latRad, double lonRad, double altKm, cJulian date)
   : cGeo(latRad, lonRad, altKm),
     m_Date(date)
{
}

cGeoTime::cGeoTime(const cEci &eci, cJulian date)
   : cGeo(eci, date),
     m_Date(date)
{
}

cGeoTime::cGeoTime(const cEciTime &eci)
   : cGeo(eci, eci.Date()),
     m_Date(eci.Date())
{
}

cGeoTime::cGeoTime(const cEcef &ecef, cJulian date)
	: cGeo(ecef, date),
	m_Date(date)
{
}

cGeoTime::cGeoTime(const cEcefTime &ecef)
	: cGeo(ecef, ecef.Date()),
	m_Date(ecef.Date())
{
}
//////////////////////////////////////////////////////////////////////
// cTopo Class
//////////////////////////////////////////////////////////////////////

cTopo::cTopo(double azRad, double elRad, double rangeKm, double rateKmSec) 
   : m_Az(azRad), 
     m_El(elRad),
     m_Range(rangeKm), 
     m_RangeRate(rateKmSec)
{
}

//////////////////////////////////////////////////////////////////////
// cTopoTime Class
//////////////////////////////////////////////////////////////////////

cTopoTime::cTopoTime(const cTopo& topo, cJulian date)
   : cTopo(topo),
     m_Date(date)
{
}

cTopoTime::cTopoTime(double azRad, double elRad, double range, double rangeRate, cJulian date)
   : cTopo(azRad, elRad, range, rangeRate),
     m_Date(date)
{
}

}
}