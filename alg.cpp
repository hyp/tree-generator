/* 
 * File:   alg.cpp
 * Author: alex
 * 
 * Created on 04 August 2011, 11:53
 */

#include <iostream>
#include <vector>

#include "math.h"
#include "alg.h"

/**
 * Generate random attraction points
 */
void Crown::generate(std::vector<Point> &points) {
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
 * Space colonization algorithm iteration
 */
void iteration(std::vector<Segment>& segments, std::vector<Point>& points,float segmentLength,float influenceRadius,float killDistance) {

	std::cout<<"== Iteration =="<<std::endl;

	//Foreach target point find closest segment and record in segment's influence set it if it's in influence area
	for (int i = 0; i < points.size(); i++) {
		if(points[i].used){
		  int mindist = -1;
		  int minidx = -1;
			for (int j = 0; j < segments.size(); j++) {
				float dist = segments[j].end.distance(&points[i].position);
				if (dist <= influenceRadius) {
				  if ( dist < mindist || minidx == -1 )
				    {
				      mindist = dist;
				      minidx = j;
				    }

				}
			}
			if ( mindist != -1 )
			  {
			    segments[minidx].influencePoints.push_back(i);
			  }
		}
	}

	//Foreach non empty segment's influence set find average direction toward its influence points and create a child segment in this direction
	int segs=segments.size(); // segments are created in loop, so save this to avoid looping over added segments
	for (int i = 0; i < segs; i++) {
		if(!segments[i].influencePoints.empty()){
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
			//branch.parent=&segments[i];
			segments.push_back(branch);
			segments[i].children.push_back(segments.size()-1);
			segments[i].influencePoints.clear();
		}
	}

	//Remove points which are within killing distance
	for (int i = 0; i < points.size(); i++) {
		if(points[i].used){
			for (int j = 0; j < segments.size(); j++) {
				if (segments[j].end.distance(&points[i].position) < killDistance) {
					std::cout << " point " << i << " removed" << std::endl;
					points[i].used=false;
					break;
				}
			}
		}
	}


}

