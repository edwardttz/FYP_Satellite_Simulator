#include "coreLib.h"
#include "orbitLib.h"

class sgp4Data : public cSatellite
{
public:
	sgp4Data();
	cTle getTLE();
	void setTLE();
	void setSatSGP4();
	cSatellite getSatSGP4();
	vector<cEci> getVecPos();
	vector<cGeo> getGeoPos();
	vector<cEcef> getEcefPos();

private:
	vector<cEci> vecPos;
	vector<cGeo> geoPos;
	vector<cEcef> ecefPos;
	cSatellite satSGP4;
	cTle tleSGP4;
};

