#ifndef MATERIALDATA_H
#define MATERIALDATA_H

#include "Common.h"
#include "glm\glm.hpp"
#include "Material.h"

class MaterialData
{

public:
	static MaterialData& getInstance()
	{
		static MaterialData instance;
		return instance;
	}
	void initialize();

	// Material
	MaterialColor mat1;
	MaterialColorPBR matPBR1;
	MaterialTexturePBR matTexPBR1;
	MaterialTexturePBR matRustedIron;
	MaterialTexturePBR matGold;

private:
	MaterialData() {}

public:
	MaterialData(MaterialData const&) = delete;
	void operator=(MaterialData const&) = delete;

};

#endif // MATERIALDATA_H