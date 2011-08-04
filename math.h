/* 
 * File:   math.h
 * Author: alex
 *
 * Created on 03 August 2011, 16:37
 */

#ifndef _MATH_H
#define	_MATH_H

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

