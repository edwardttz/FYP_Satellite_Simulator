#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//libraries
#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <cstdlib>
#include <condition_variable>
#include <time.h>
#include <vector>
#include <iomanip>
#include <map>
#include <random>
#include <exception>
#include <ctime>
#include <tchar.h>
#include <algorithm>
#include <assert.h>
#include <math.h>
#include <cmath>
#include <tchar.h>

//headers for ground truth
#include "SpacecraftDynamics.h"
#include "Storage.h"
#include "coreLib.h"
#include "orbitLib.h"
#include "sgp4Data.h"

//headers for noise models
#include "CircularEngine.hpp"
#include "Constants.hpp"
#include "Geocentric.hpp"
#include "MagneticCircle.hpp"
#include "MagneticModel.hpp"
#include "Math.hpp"
#include "SphericalEngine.hpp"
#include "SphericalHarmonic.hpp"
#include "Utility.hpp"
#include "Magnetometer.h"
#include "NoiseModel.h"

#include "Gyroscope Noise Model.h"

//#include "globals.h"
//#include "EciSun.h"
//#include "cVector.h"
//#include "SunSensorModel.h"

using namespace std;
using namespace GeographicLib;