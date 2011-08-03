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

bool sphere::contains(vec3* point) {
	return position.distance(point) < radius;
}
