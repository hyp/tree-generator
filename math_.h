/* 
 * File:   math.h
 * Author: alex
 *
 * Created on 03 August 2011, 16:37
 */

#ifndef _MATH__H
#define	_MATH__H

#include <math.h>

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

    vec3& operator /=(float t)
    {
        float f = 1.0f / t;
        x *= f;
        y *= f;
        z *= f;
        return (*this);
    }

    vec3 add(const vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return (*this);
    }

    vec3 mul(float t)
    {
        x *= t;
        y *= t;
        z *= t;
        return (*this);
    }

     vec3 sub(const vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return (*this);
    }

     vec3 normalize()
     {
         return (*this /= sqrtf(x * x + y * y + z * z));
     }

    float distance(vec3 * other);
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

