#include "NoiseMap.h"
#include <iostream>

#include "WorldGen.h"

NoiseMap::NoiseMap()
{				
	map = new noise::module::Perlin();
	map->SetOctaveCount(6);
	map->SetFrequency(1.0);
	map->SetPersistence(0.5);
}

NoiseMap::~NoiseMap()
{
	map = nullptr;
	delete map;
}

double NoiseMap::getValue(double x, double z)
{			
	if(map != nullptr)
		return map->GetValue(x / WorldGen::SMOOTHNESS, 0.0, z / WorldGen::SMOOTHNESS);
	return 0;
}

