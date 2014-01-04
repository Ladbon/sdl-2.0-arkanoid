#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <chrono>
#include <string>

struct Circle {
	float x;
	float y;
	float radius;
};

struct RGB {
	int r;
	int g;
	int b;
	int a;
};

namespace Utils {

	/* Generates truly random numbers */
	namespace Random {
		int random(int min, int max);
		float frandom(float min, float max);
		void seed();
	};

	/* Get current time in milliseconds */
	uint64_t GetTimeByMilliseconds();

	/* Converts an integer to a string */
	std::string int_to_string(int v);

	/* Converts a string to a float */
	float string_to_float(std::string s);

	/* Splits a string by a delimiter and returns a vector with std::string */
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

	/* Trims a string from start */
	std::string &ltrim(std::string &s);

	/* Trims a string from end */
	std::string &rtrim(std::string &s);

	/* Trims a string from both ends */
	std::string &trim(std::string &s);

	/* Convert string to int */
	int string_to_int(std::string v);

	/* Rounds a number the the nearest number n */
	float roundToNearestN(float x, float n);

	/* Compare float numbers */
	bool AreSame(float a, float b);
	bool AreSame(double a, double b);

	/* Converts a hexadecimal color to a RGB struct */
	RGB HexToRGB(int hex);

	/* Converts all uppercase letters to lowercase */
	std::string toLower(std::string string);
}

#endif