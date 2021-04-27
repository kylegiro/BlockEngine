#pragma once

#include <noise/noise.h>

class NoiseMap
{
public:
	NoiseMap();
	~NoiseMap();
	double getValue(double x, double z);

private:
	noise::module::Perlin* map;
};

