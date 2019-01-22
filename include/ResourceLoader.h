#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include <string>
#include <nlohmann/json.hpp>

#include <glm/vec3.hpp>

using json = nlohmann::json;

class ResourceLoader
{
private:
	ResourceLoader();
	~ResourceLoader();

public:

	static ResourceLoader& Instance()
	{
		static ResourceLoader refInstance;
		return refInstance;
	}

	void load();

	ResourceLoader(ResourceLoader const&) = delete;
	void operator=(ResourceLoader const&) = delete;

private:

	const std::string m_config = "config.json";
};

#endif // RESOURCELOADER_H