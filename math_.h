/* 
 * File:   math.h
 * Author: alex
 *
 * Created on 03 August 2011, 16:37
 */

#ifndef _MATH__H
#define	_MATH__H

#include <math.h>

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

