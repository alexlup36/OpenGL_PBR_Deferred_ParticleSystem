#include "ResourceLoader.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <list>

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::~ResourceLoader()
{
}

void ResourceLoader::load()
{
	std::ifstream inputStream("../" + m_config);
	if (inputStream.is_open())
	{
		// Parse json
		json j;
		inputStream >> j;

		auto& object = j["object"];
		auto active = object["active"].get<bool>();
		auto name = object["name"].get<std::string>();
		auto model = object["model"].get<std::string>();
		auto vertexType = object["vertextype"].get<std::string>();
		auto displacementMapScale = object["displacementmapscale"].get<double>();
		auto textureOffset = object["textureoffset"].get<std::vector<double>>();
		auto textureTiling = object["texturetile"].get<std::vector<double>>();
		// Shader
		auto& shader = object["shader"];
		auto vertexShader = shader["vertex"].get<std::string>();
		auto fragmentShader = shader["fragment"].get<std::string>();
		// Transform
		auto& transform = object["transform"];
		auto position = transform["position"].get<std::vector<double>>();
		auto rotation = transform["rotation"].get<std::vector<double>>();
		auto scale = transform["scale"].get<std::vector<double>>();
		// Material
		auto& material = object["material"];
		auto albedo = material["albedo"].get<std::string>();
		auto normal = material["normal"].get<std::string>();
		auto roughness = material["roughness"].get<std::string>();
		auto metallic = material["metallic"].get<std::string>();
		auto displacement = material["displacement"].get<std::string>();
		auto ambientocclusion = material["ambientocclusion"].get<std::string>();

		inputStream.close();
	}
	else
	{
		std::cout << "Failed to load configuration file: " << m_config << "\n";
		return;
	}
}