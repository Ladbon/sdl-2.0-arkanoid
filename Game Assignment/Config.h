#pragma once

#include <string>
#include <map>
#include <vector>

class Config {
	Config();

public:
	static bool parseFile(std::string _config_file);
	static std::string get(std::string key);
	static bool defined(std::string key);

private:
	static std::string config_file;
	static std::map<std::string, std::string> keys;
};