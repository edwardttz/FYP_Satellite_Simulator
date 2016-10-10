//
// cEcef.cpp
//
// 
//
#include "stdafx.h"

#include "globals.h"
#include "cEcef.h"
#include "coord.h"

namespace Zeptomoby
{
	namespace OrbitTools
	{

		//////////////////////////////////////////////////////////////////////
		// Class cEcef
		//////////////////////////////////////////////////////////////////////
		cEcef::cEcef(const cVector &pos, const cVector &vel)
			: m_Position(pos),
			m_Velocity(vel)
		{
		}

		// Creates a instance of the class from geodetic coordinates.
		//
		// Assumes the Earth is an oblate spheroid.
		// Reference: The 1992 Astronomical Almanac, page K11
		// Reference: www.celestrak.com (Dr. T.S. Kelso)
		cEcef::cEcef(const cGeo& geo, cJulian date)
		{
			double lat = geo.LatitudeRad();
			double lon = geo.LongitudeRad();
			double alt = geo.AltitudeKm();

			// Calculate Local Mean Sidereal Time (theta)
			double theta = date.ToLmst(lon);
			double c = 1.0 / sqrt(1.0 + F * (F - 2.0) * sqr(sin(lat)));
			double s = sqr(1.0 - F) * c;
			double achcp = (XKMPER_WGS84 * c + alt) * cos(lat);

			m_Position.m_x = achcp * cos(theta);         // km
			m_Position.m_y = achcp * sin(theta);         // km
			m_Position.m_z = (XKMPER_WGS84 * s + alt) * sin(lat);   // km
			m_Position.m_w = sqrt(sqr(m_Position.m_x) +  // range, km
				sqr(m_Position.m_y) +
				sqr(m_Position.m_z));

			// Determine velocity components due to earth's rotation
			double mfactor = TWOPI * (OMEGA_E / SEC_PER_DAY);

			m_Velocity.m_x = -mfactor * m_Position.m_y;  // km / sec
			m_Velocity.m_y = mfactor * m_Position.m_x;  // km / sec
			m_Velocity.m_z = 0.0;                        // km / sec
			m_Velocity.m_w = sqrt(sqr(m_Velocity.m_x) +  // range rate km/sec^2
				sqr(m_Velocity.m_y));
		}

		//////////////////////////////////////////////////////////////////////
		// Class cEcefTime
		//////////////////////////////////////////////////////////////////////

		cEcefTime::cEcefTime(const cEcef &ecef, cJulian date) :
			cEcef(ecef),
			m_Date(date)
		{
		}

		cEcefTime::cEcefTime(const cVector &pos, const cVector &vel, cJulian date) :
			cEcef(pos, vel),
			m_Date(date)
		{
		}

		cEcefTime::cEcefTime(const cGeo &geo, cJulian date)
			: cEcef(geo, date),
			m_Date(date)
		{
		}

		cEcefTime::cEcefTime(const cGeoTime &geo)
			: cEcef(geo, geo.Date()),
			m_Date(geo.Date())
		{
		}

	}
}