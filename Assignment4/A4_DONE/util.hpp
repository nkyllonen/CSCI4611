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
	/*-------------------------------------------------
		scaleT : scales given t, assuming
					t exists in [t_i, t_ii]
	-------------------------------------------------*/
	float scaleT(float t_i, float t_ii, float t){
		float timeInterval = t_ii - t_i;
		return ((1.0 / timeInterval)*t - (t_i / timeInterval));
	}
	/*-------------------------------------------------
		h00 : Hermite basis function - first coefficient
	-------------------------------------------------*/
	float h00(float t) {
		return (2*pow(t,3) - 3*pow(t,2) + 1);
	}
	/*-------------------------------------------------
		h10 : Hermite basis function - second coefficient
	-------------------------------------------------*/
	float h10(float t) {
		return (pow(t,3) - 2*pow(t,2) + t);
	}
	/*-------------------------------------------------
		h01 : Hermite basis function - third coefficient
	-------------------------------------------------*/
	float h01(float t) {
		return (-2*pow(t,3) + 3*pow(t,2));
	}
	/*-------------------------------------------------
		h11 : Hermite basis function - fourth coefficient
	-------------------------------------------------*/
	float h11(float t) {
		return (pow(t,3) - pow(t,2));
	}
	/*-------------------------------------------------
		dh00 : derivative of Hermite basis function
				- first coefficient
	-------------------------------------------------*/
	float dh00(float t) {
		return (6 * pow(t, 2) - 6 * t);
	}
	/*-------------------------------------------------
		dh10 : derivative of Hermite basis function
				- second coefficient
	-------------------------------------------------*/
	float dh10(float t) {
		return (3*pow(t, 2) - 4 * t + 1);
	}
	/*-------------------------------------------------
		dh01 : derivative of Hermite basis function
				- third coefficient
	-------------------------------------------------*/
	float dh01(float t) {
		return (-6 * pow(t, 2) + 6 * t);
	}
	/*-------------------------------------------------
		dh11 : derivative of Hermite basis function
				- fourth coefficient
	-------------------------------------------------*/
	float dh11(float t) {
		return (3*pow(t, 2) - 2*t);
	}
}

#endif