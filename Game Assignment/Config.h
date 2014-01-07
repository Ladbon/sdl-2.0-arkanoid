#pragma once

#include <string>
#include <map>
#include <vector>

class Config {
	Config();

public:
	static bool parseFile(std::string _config_file);
	static std::string get(std::string key);
	static int getInt(std::string key, int defaultVal);
	static int getHex(std::string key, int defaultVal);
	static bool getBool(std::string key, bool defaultVal);
	static float getFloat(std::string key, float defaultVal);

private:
	static std::string config_file;
	static std::map<std::string, std::string> keys;
};