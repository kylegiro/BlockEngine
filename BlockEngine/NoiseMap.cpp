#include "NoiseMap.h"
#include <iostream>

NoiseMap::NoiseMap()
{				
	map = new noise::module::Perlin();
	map->SetOctaveCount(6);
	map->SetFrequency(1.0);
	map->SetPersistence(1.0);
}

NoiseMap::~NoiseMap()
{
}

double NoiseMap::getValue(double x, double z)
{			
	if(map != nullptr)
		return map->GetValue(x / 1000.0, 1.5, z / 1000.0);
	return 0;
}

