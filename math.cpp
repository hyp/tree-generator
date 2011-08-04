/* 
 * File:   math.cpp
 * Author: alex
 * 
 * Created on 03 August 2011, 16:37
 */

#include <stdlib.h>
#include <math.h>
#include "math.h"

float random(float min, float max) {
	float r = (float) rand() / (float) RAND_MAX; //random number between 0 and 1
	return r * (max - min) + min;
}

float vec3::distance(vec3* other) {
	return sqrtf((other->x - x)*(other->x - x)+(other->y - y)*(other->y - y)+(other->z - z)*(other->z - z));
}

float vec3::magnitude() {
	return sqrtf(x * x + y * y + z * z);
}

void vec3::normalize() {
	float m = sqrtf(x * x + y * y + z * z);
	x /= m;
	y /= m;
	z /= m;
}

void vec3::add(vec3* other) {
	x += other->x;
	y += other->y;
	z += other->z;
}

void vec3::sub(vec3* other) {
	x -= other->x;
	y -= other->y;
	z -= other->z;
}

float vec3::dot(vec3* other) {
	return x * other->x + y * other->y + z * other->z;
}

void vec3::mul(float scale) {
	x *= scale;
	y *= scale;
	z *= scale;
}

void vec3::div(float scale) {
	x /= scale;
	y /= scale;
	z /= scale;
}

bool sphere::contains(vec3* point) {
	return position.distance(point) < radius;
}
