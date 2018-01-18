#ifndef SPLINE_HPP
#define SPLINE_HPP

#include <glm/glm.hpp>
#include <vector>
#include "util.hpp"
using glm::vec3;

class SplinePoint3 {
public:
    float t;    // time
    vec3 p, dp; // value and derivative
    SplinePoint3(float t, vec3 p, vec3 dp): t(t), p(p), dp(dp) {}
};

class Spline3 {
public:
    // list of spline control points. assumed to be in increasing order of t
    std::vector<SplinePoint3> points;

    // value of t for first and last control points
    float minTime() {return points.front().t;}
    float maxTime() {return points.back().t;}

    // returns i such that t lies between points[i].t and points[i+1].t.
    // also modifies t to lie in [minTime(), maxTime()] if necessary.
    int findSegment(float &t);

    // returns value of spline function at time t
    vec3 getValue(float t);

    // returns d/dt of spline function at time t
    vec3 getDerivative(float t);
};
// TODO: If t is outside the range [minTime(), maxTime()], replace
// it with the closest time in that range. Then, find the segment
// that contains t, so that you can perform cubic Hermite
// interpolation within it.
inline int Spline3::findSegment(float &t) {
	//1. check for out-of-bounds
	if (t > maxTime()) { t = maxTime(); }
	else if (t < minTime()) { t = minTime(); }
	
	//2. t is in-bounds
	for (int i = 1; i < points.size(); i++) {
		if (points.at(i).t > t) {
			return i-1;
		}
		else if (points.at(i).t == t) {
			return i;
		}
	}

    return 0;

}
// TODO: Find the segment that contains t, and use the cubic
// Hermite interpolation formula to find the interpolated value
// within it. Note that the formula discussed in class is only
// valid for t0 = 0, t1 = 1, so you will have to use a modified
// formula.
inline vec3 Spline3::getValue(float t) {
	int i = findSegment(t);

	//check if at the end - wrap if necessary
	if (i >= (points.size() - 1)) {
		i = 0;
	}

	float timeInterval = points.at(i + 1).t - points.at(i).t;
	vec3 p_i = points.at(i).p;
	vec3 dp_i = points.at(i).dp*timeInterval; //scaled deriv
	vec3 p_ii = points.at(i+1).p;
	vec3 dp_ii = points.at(i+1).dp*timeInterval; //scaled deriv

	float t_sc = Util::scaleT(points.at(i).t, points.at(i + 1).t, t);

	//hermite cooefficients
	float h00 = Util::h00(t_sc);
	float h10 = Util::h10(t_sc);
	float h01 = Util::h01(t_sc);
	float h11 = Util::h11(t_sc);

	vec3 interpVec = h00*p_i + h10*dp_i + h01*p_ii + h11*dp_ii;

    return interpVec;

}
// TODO: Find the segment that contains t. Differentiate the cubic
// Hermite interpolation formula to find the derivative of the
// spline function. Be careful about how rescaling affects
// derivatives.
inline vec3 Spline3::getDerivative(float t) {
	int i = findSegment(t);

	//check if at the end - wrap if necessary
	if (i >= (points.size() - 1)) {
		i = 0;
	}

	float timeInterval = points.at(i + 1).t - points.at(i).t;
	vec3 p_i = points.at(i).p;
	vec3 dp_i = points.at(i).dp*timeInterval;
	vec3 p_ii = points.at(i + 1).p;
	vec3 dp_ii = points.at(i + 1).dp*timeInterval;

	float t_sc = Util::scaleT(points.at(i).t, points.at(i + 1).t, t);

	//derivatives of hermite cooefficients
	float dh00 = Util::dh00(t_sc);
	float dh10 = Util::dh10(t_sc);
	float dh01 = Util::dh01(t_sc);
	float dh11 = Util::dh11(t_sc);

	vec3 interpVec = dh00*p_i + dh10*dp_i + dh01*p_ii + dh11*dp_ii;

    return interpVec / timeInterval;

}

#endif
