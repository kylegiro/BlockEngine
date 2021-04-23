#include "NoiseMap.h"
#include <iostream>

NoiseMap::NoiseMap()
{				

}

double NoiseMap::getValue(double x, double z)
{			
	noise::module::Perlin map;
	map.SetOctaveCount(6);
	map.SetFrequency(1.0);
	map.SetPersistence(1.0);	
	return map.GetValue(x / 1000.0, 1.5, z / 1000.0);
}

