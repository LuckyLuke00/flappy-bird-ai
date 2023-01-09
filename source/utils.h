#ifndef UTILS_H
#define UTILS_H

#include <random>

namespace utils
{
	// Initialize the random number generator
	static std::mt19937 mt{ std::random_device{}() };

	inline int RandomInt(int min, int max)
	{
		std::uniform_int_distribution dist{ min, max };
		return dist(mt);
	}

	inline float RandomFloat(float min, float max)
	{
		std::uniform_real_distribution dist{ min, max };
		return dist(mt);
	}
}

#endif
