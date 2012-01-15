/**
 * The marching cubes algorithm implementation
 */
#ifndef MC_H
#define	MC_H

#include "math_.h"
#include "alg.h"

void marchCubes(vec3 position,vec3 size,int numCubes,std::vector<vec3>& vertices,std::vector<Segment>* segments);

#endif	/* MC_H */

