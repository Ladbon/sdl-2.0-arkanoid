#include <fstream>
#include <map>
#include <vector>

#include "Utils.h"
#include "Config.h"

std::map<std::string, std::string> Config::keys;

Config::Config() {

}

bool Config::parseFile(std::string _config_file) {
	std::ifstream file(_config_file);
	std::string single_row;
	std::vector<std::string> configs;
	configs.clear();

	if(!file.is_open()) return false;

	while(!file.eof()) {
		std::getline(file, single_row, '\n');
		if(single_row.size() == 0) continue;
		Utils::split(single_row, '=', configs);
		std::map<std::string, std::string>::iterator it = keys.find(configs.front());
		if(it != keys.end()) 
			printf("Config::parseFile() Key already exists: %s\n", configs.front().c_str());
		else
			keys.insert(std::pair<std::string, std::string>(configs.front(), configs.back()));
		configs.clear();
	}

	return true;
}

std::string Config::get(std::string key) {
	std::map<std::string, std::string>::iterator it = keys.find(key);
	if(it == keys.end()) {
		printf("Config::get() Key %s was not found\n", key.c_str());
		return "";
	}
	return (*it).second;
}

int Config::getInt(std::string key, int defaultVal) {
    std::string value = Config::get(key);
    if (value.empty())
		return defaultVal;

	return Utils::string_to_int(value);
}

int Config::getHex(std::string key, int defaultVal) {
	std::string value = Config::get(key);
    if (value.empty())
		return defaultVal;

	return Utils::string_to_hex(value);
}

bool Config::getBool(std::string key, bool defaultVal) {
	if(key.empty()) return defaultVal;
	std::string val = Config::get(key);
	if(val.empty()) return defaultVal;
	if(val == "true" || val == "yes" || val == "1")
		return true;
	return false;
}

float Config::getFloat(std::string key, float defaultVal) {
	if(key.empty()) return defaultVal;
	std::string val = Config::get(key);
	if(val.empty()) return defaultVal;
	return Utils::string_to_float(val);
}