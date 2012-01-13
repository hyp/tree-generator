/* 
 * File:   math.h
 * Author: alex
 *
 * Created on 03 August 2011, 16:37
 */

#ifndef _MATH_H
#define	_MATH_H

const double PI = 3.141592653589793238462643;
const float PIf = 3.1415926536f;
const double PI_2 = 6.283185307179586476925287;
const float PI_2f = 6.2831853072f;
const float RAD_to_DEGf = 57.295779513f;
const float DEG_to_RADf = 0.0174532925f;
const double E = 2.718281828458563411277850;
const float Ef = 2.7182818285f;
const float Zerof = 0.00001000000f;
const float mZerof = -0.00001000000f;

/**
 * 
 * @param min
 * @param max
 * @return 
 */
float random(float min, float max);

struct vec3 {
    float x, y, z;

    vec3() {
    }

    vec3(const vec3 & p) : x(p.x), y(p.y), z(p.z) {
    }

    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {
    }

    float distance(vec3 * other);
    float magnitude();
    void normalize();

    void add(vec3 * other);
    void sub(vec3 * other);
    float dot(vec3 * other);

    void mul(float scale);
    void div(float scale);
};

struct sphere {
    vec3 position;
    float radius;

    sphere() {
    }

    sphere(const sphere & other) : position(other.position), radius(other.radius) {
    }

    sphere(vec3 pos, float r) : position(pos), radius(r) {
    }

    bool contains(vec3 * point);
};

#endif	/* _MATH_H */

