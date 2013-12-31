#include <random>
#include <chrono>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>

#include "Utils.h"

namespace Utils {
	namespace Random {
		std::mt19937 engine;
	
		int Utils::Random::random(int min, int max) {
			std::uniform_int_distribution<int> distance(min, max);
			return distance(engine);
		}

		float Utils::Random::frandom(float min, float max) {
			std::uniform_real_distribution<float> distance(min, max);
			return distance(engine);
		}

		void Utils::Random::seed() {
			engine.seed(time(nullptr));
		}
	}

	uint64_t Utils::GetTimeByMilliseconds() {
		return std::chrono::duration_cast<std::chrono::milliseconds>  (std::chrono::system_clock::now().time_since_epoch()).count();
	}

	std::string Utils::int_to_string(int v) {
		std::ostringstream ostr;
		ostr << v;
		return ostr.str();
	}

	std::vector<std::string> &Utils::split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(Utils::trim(item));
		}
		return elems;
	}

	float Utils::roundToNearestN(float x, float n)  {
		float fmod = fmodf(x, n);
		if(fmod < n*.5) { return ceil(x + fmod); }
		return ceil(x - fmod);
	}

	std::string &Utils::ltrim(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
			return s;
	}

	std::string &Utils::rtrim(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
			return s;
	}

	std::string &Utils::trim(std::string &s) {
			return ltrim(rtrim(s));
	}

	int Utils::string_to_int(std::string v) {
		return std::stoi(v);
	}
}