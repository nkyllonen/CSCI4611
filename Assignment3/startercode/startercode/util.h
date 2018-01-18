#ifndef UTIL_HPP
#define UTIL_HPP

namespace Util {
	/*-------------------------------------------------
		lerp: returns linear interp between x and y
				by an amount a
	-------------------------------------------------*/
	float lerp(float x, float y, float a) {
		return (x + a*(y - x));
	}
	/*-------------------------------------------------
		getV3Magnitude: returns magnitude of vec3
	-------------------------------------------------*/
	float getV3Magnitude(vec3 v) {
		return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
	}

}

#endif