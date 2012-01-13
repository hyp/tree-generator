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

    void generateAttractionPoints(std::vector<Point>& points);
};

/**
 * Spherical tree crown
 */
//class SphereCrown : public Crown {
//    sphere volume;
//public:
//
//    SphereCrown(sphere vol) : volume(vol) {
//    }
//
//    void bounds(treegenerator::vec3* min, vec3* max);
//    bool contains(treegenerator::vec3* point);
//    void display();
//};

/**
 * Branch segment
 */
struct Segment {
    vec3 start, end;
    Segment* next,* child,* parent;
    std::vector<int> influencePoints;
public:

    Segment(): next(0),child(0),parent(0) {
    }

    Segment(vec3 s, vec3 e) : start(s), end(e),next(0),child(0),parent(0) {
    }

    Segment(vec3 s, vec3 e, Segment* prev) : start(s), end(e),next(0),child(0),parent(prev) {
        prev->next=this;
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

