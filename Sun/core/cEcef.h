//
// cEcef.h
//
//
#pragma once

#include "globals.h"
#include "cVector.h"
#include "cJulian.h"

namespace Zeptomoby
{
	namespace OrbitTools
	{

		class cGeo;
		class cGeoTime;

		//////////////////////////////////////////////////////////////////////
		// class cEcef
		//////////////////////////////////////////////////////////////////////
		class cEcef
		{
		public:
			cEcef(const cVector &pos, const cVector &vel);
			cEcef(const cGeo &geo, cJulian date);

			virtual ~cEcef() {};

			const cVector& Position() const { return m_Position; }
			const cVector& Velocity() const { return m_Velocity; }

			void ScalePosVector(double factor) { m_Position.Mul(factor); }
			void ScaleVelVector(double factor) { m_Velocity.Mul(factor); }

		protected:
			cVector  m_Position;
			cVector  m_Velocity;
		};

		//////////////////////////////////////////////////////////////////////
		// class cEcefTime
		//////////////////////////////////////////////////////////////////////
		class cEcefTime : public cEcef
		{
		public:
			cEcefTime(const cEcef &ecef, cJulian date);
			cEcefTime(const cVector &pos, const cVector &vel, cJulian date);
			cEcefTime(const cGeo &geo, cJulian date);
			cEcefTime(const cGeoTime &geo);

			virtual ~cEcefTime() {};

			cJulian Date() const { return m_Date; }

		protected:
			cJulian m_Date;
		};

	}
}
