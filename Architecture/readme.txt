
Properties > C/C++ > Additional Dependencies
$(SolutionDir)\Sun;$(SolutionDir)\GyroNoiseModel\GyroNoiseModel;$(SolutionDir)..\Magnetometer\Magnetometer;$(SolutionDir)..\orbitTools\orbit;$(SolutionDir)..\orbitTools\core;$(SolutionDir)\FYP_v1\FYP_v1;%(AdditionalIncludeDirectories)

Properties > Linker > Input > Additional Dependencies
$(SolutionDir)\Win32\Debug\orbitTools.core.lib;$(SolutionDir)\Win32\Debug\orbitTools.orbit.lib;$(SolutionDir)\Debug\FYP_v1.lib;$(SolutionDir)\Debug\GyroNoiseModel.lib;$(SolutionDir)\Debug\Magnetometer.lib;$(SolutionDir)\Debug\Sun.lib;%(AdditionalDependencies)
