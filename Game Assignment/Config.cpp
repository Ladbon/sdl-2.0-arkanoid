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
	if(it == keys.end()) printf("Config::get() Key %s was not found\n", key.c_str());
	return (*it).second;
}

bool Config::defined(std::string key) {
	std::map<std::string, std::string>::iterator it = keys.find(key);
	return it != keys.end();
}