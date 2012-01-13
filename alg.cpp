/* 
 * File:   alg.cpp
 * Author: alex
 * 
 * Created on 04 August 2011, 11:53
 */

#include <iostream>

#include <vector>
#include <stdio.h>

#include "math.h"
#include "alg.h"

class SphereCrown;

/**
 *Generate random attraction points
 */
void Crown::generateAttractionPoints(std::vector<Point> &points) {
	vec3 min, max;
	bounds(&min, &max);
	for (int i = 0; i < points.size(); i++) {
		vec3 r;
		while (true) {
			r = vec3(random(min.x, max.x), random(min.y, max.y), random(min.z, max.z));
			if (contains(&r)) break;
		}
		points[i] = Point(r);
	}
}

/**
 * Algorithm iteration
 */
void iteration(std::vector<Segment>& segments, std::vector<Point>& points,float segmentLength,float influenceRadius,float killDistance) {

	printf("== Iteration ==\n");

	//Foreach target point find closest segment and record in segment's influence set it if it's in influence area
	for (int i = 0; i < points.size(); i++) {
		if(points[i].used){
			//float closestDistance = 10000000;
			for (int j = 0; j < segments.size(); j++) {
				float dist = segments[j].end.distance(&points[i].position);
				if (dist <= influenceRadius /*&& closestDistance > dist*/) {
					segments[j].influencePoints.push_back(i);
					//closestDistance = dist;
				}
			}
		}
	}

	//Foreach non empty segment's influence set find average direction toward its influence points and create a child segment in this direction
	int segs=segments.size(); // segments are created in loop, so save this to avoid looping over added segments
	for (int i = 0; i < segs; i++) {
		if(!segments[i].influencePoints.empty()){
			//printf(" seg %d is influenced by %d points\n",i,segments[i].influencePoints.size());
                        std::cout << " seg " << i << " is influenced by " << segments[i].influencePoints.size() << " points" << std::endl;
			vec3 averageDirection=vec3(0,0,0);
			for(int j=0;j<segments[i].influencePoints.size();j++){
				vec3 dir=points[segments[i].influencePoints[j]].position;
				dir.sub(segments[i].end);
				dir.normalize();
				averageDirection.add(dir);
			}
			averageDirection.normalize();
			averageDirection.mul(segmentLength);
			averageDirection.add(segments[i].end);//now averageDirection is the end point of new segment

			Segment branch=Segment(segments[i].end,averageDirection);
			branch.parent=&segments[i];
			segments.push_back(branch);
			segments[i].child=&segments[segments.size()-1];
			segments[i].influencePoints.clear();
		}
	}

	//Remove points which are within killing distance
	for (int i = 0; i < points.size(); i++) {
		if(points[i].used){
			for (int j = 0; j < segments.size(); j++) {
				if (segments[j].end.distance(&points[i].position) < killDistance) {
					printf(" point %d removed\n",i);
					points[i].used=false;
					break;
				}
			}
		}
	}


}

