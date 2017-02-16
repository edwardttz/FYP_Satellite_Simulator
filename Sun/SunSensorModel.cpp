#pragma once

#include "stdafx.h"

using namespace std;

SunSensorModel::SunSensorModel()
{	
}

SunSensorModel::SunSensorModel(const SunSensorModel &s)
{
	x = s.x;
	y = s.y;
	z = s.z;
	d = s.d;
}