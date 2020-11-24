#pragma once

#include <json.hpp>
#include <fstream>

enum model
{
	crate,
	chinese_box
};

struct ModelManager
{
	std::vector<xplor::Model> models;
	std::shared_ptr<std::vector<std::string>> model_names = std::make_shared<std::vector<std::string>>();

	void loadJsonModels()
	{
		std::ifstream file("res/models/models.json");

		std::string content((std::istreambuf_iterator<char>(file)),
			(std::istreambuf_iterator<char>()));

		json::JSON json = json::JSON::Load(content);

		for (auto& o : json["models"].ArrayRange())
		{
			models.push_back(xplor::Model(o.ToString()));
			model_names->push_back(o.ToString());
		}
	}

	void createJsonModels()
	{
		json::JSON json;

		json["models"] = json::Array("crate", "chinese_box");

		std::ofstream ofile("res/models/models.json");

		ofile << json;
		ofile.flush();
	}

	void loadModels()
	{
		loadJsonModels();
	}
};