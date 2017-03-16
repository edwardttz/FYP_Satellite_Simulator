//
// globals.h
//
// Copyright (c) 2003-2013 Michael F. Henry
//
#pragma once

#include "math.h"

namespace Zeptomoby 
{
namespace OrbitTools
{
const double sunDistInKm = 149597870700;
const double PI           = 3.141592653589793;
const double TWOPI        = 2.0 * PI;
const double RADS_PER_DEG = PI / 180.0;

const double GM           = 398600.5;   // Earth gravitational constant, km^3/sec^2
const double GEOSYNC_ALT  = 42241.892;  // km
const double EARTH_DIA    = 12800.0;    // km
const double DAY_SIDERAL  = (23 * 3600) + (56 * 60) + 4.09;  // sec
const double DAY_24HR     = (24 * 3600);   // sec

const double AE           = 1.0;
const double AU           = 149597870.0;  // Astronomical unit (km) (IAU 76)
const double SR           = 696000.0;     // Solar radius (km)      (IAU 76)
const double XKMPER_WGS84 = 6378.137;     // Earth equatorial radius - km (WGS '84)
const double F            = 1.0 / 298.257223563; // Earth flattening (WGS '84)
const double GE           = 398600.5;     // Earth gravitational constant (WGS '84)
const double J2           = 1.08262998905E-3; // J2 harmonic (WGS '84)
const double J3           = -2.53215306E-6;  // J3 harmonic (WGS '84)
const double J4           = -1.61098761E-6;  // J4 harmonic (WGS '84)
const double CK2          = J2 / 2.0;
const double CK4          = -3.0 * J4 / 8.0;
const double XJ3          = J3;
const double QO           = AE + 120.0 / XKMPER_WGS84;
const double S            = AE + 78.0  / XKMPER_WGS84;
const double HR_PER_DAY   = 24.0;          // Hours per day   (solar)
const double MIN_PER_DAY  = 1440.0;        // Minutes per day (solar)
const double SEC_PER_DAY  = 86400.0;       // Seconds per day (solar)
const double OMEGA_E      = 1.00273790934; // earth rotation per sideral day
const double XKE          = sqrt(3600.0 * GE /           //sqrt(ge) ER^3/min^2
                                (XKMPER_WGS84 * XKMPER_WGS84 * XKMPER_WGS84)); 
const double QOMS2T       = pow((QO - S), 4);            //(QO - S)^4 ER^4
const double WE				  = 7.292115854788046E-5; //Angular velocity of Earth

// Utility functions
double sqr   (const double x);
double Fmod2p(const double arg);
double AcTan (const double sinx, const double cosx);

double rad2deg(const double);
double deg2rad(const double);
}
}