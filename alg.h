/* 
 * File:   alg.h
 * Author: alex
 *
 * Created on 04 August 2011, 11:53
 */

#ifndef _ALG_H
#define	_ALG_H

#include "math_.h"

/**
 * Attraction point
 */
struct Point {
    vec3 position;
    int used;

    Point() {
    }

    Point( vec3 pos) : position(pos), used(true) {
    }
};

/**
 * Tree Crown
 */
class Crown {
public:
    virtual void bounds(vec3* min, vec3* max) = 0;
    virtual bool contains(vec3* point) = 0;
    virtual void display() = 0;

    void generate(std::vector<Point>& points);
};


/**
 * Branch segment
 */
struct Segment {
    vec3 start, end;

    std::vector<int> children;
    std::vector<int> influencePoints;
    float startRadius,endRadius;
public:

    Segment() {
    }

    Segment(vec3 s, vec3 e) : start(s), end(e) {
    }

    void display();
};

/**
 * @param segments
 * @param points
 * @param segmentLength
 * @param influenceRadius
 * @param killDistance
 */
void iteration(std::vector<Segment>& segments,std::vector<Point>& points,float segmentLength,float influenceRadius,float killDistance);

#endif	/* _ALG_H */

