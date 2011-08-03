/* 
 * File:   main.cpp
 * Author: alex
 *
 * Created on 03 August 2011, 11:20
 */

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "math.h"

void init() {
	glClearColor(1, 1, 1, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/**
 * The tree crown
 */
class Crown {
public:
	virtual void bounds(vec3* min, vec3* max) = 0;
	virtual bool contains(vec3* point) = 0;
	virtual void display() = 0;

	void generate(vec3* points, int numPoints);
};

/**
 *Generate target points
 */
void Crown::generate(vec3* points, int numPoints) {
	vec3 min, max;
	bounds(&min, &max);
	for (int i = 0; i < numPoints; i++) {
		vec3 r;
		while (true) {
			r = vec3(random(min.x, max.x), random(min.y, max.y), random(min.z, max.z));
			if (contains(&r)) break;
		}
		points[i] = r;
	}
}

/**
 * Spherical tree crown
 */
class SphereCrown : public Crown {
	sphere volume;
public:

	SphereCrown(sphere vol) : volume(vol) {
	}

	void bounds(vec3* min, vec3* max);
	bool contains(vec3* point);
	void display();
};

void SphereCrown::bounds(vec3* min, vec3* max) {
	*min = vec3(volume.position.x - volume.radius, volume.position.y - volume.radius, volume.position.z - volume.radius);
	*max = vec3(volume.position.x + volume.radius, volume.position.y + volume.radius, volume.position.z + volume.radius);
}

bool SphereCrown::contains(vec3* point) {
	return volume.contains(point);
}

void SphereCrown::display() {
	glPushMatrix();
	glTranslatef(volume.position.x, volume.position.y, volume.position.z);
	glColor4f(0, 0, 0, 1.0f);
	glutWireSphere(volume.radius, 12, 12);
	glPopMatrix();
}

/**
 *Tree segment
 */
struct Segment {
	vec3 start;
	vec3 end;
public:

	Segment() {
	}

	Segment(vec3 s, vec3 e) : start(s), end(e) {
	}

	float length() {
		return start.distance(&end);
	}

	void display() {
		glColor4f(0, 0, 1, 1.0f);
		glBegin(GL_LINES);
		glVertex3f(start.x, start.y, start.z);
		glVertex3f(end.x, end.y, end.z);
		glEnd();
		glPushMatrix();
		glTranslatef(start.x, start.y, start.z);
		glutWireSphere(0.2, 8, 8);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(end.x, end.y, end.z);
		glutWireSphere(0.2, 8, 8);
		glPopMatrix();
	}
};

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	//our crown
	Crown* crown = new SphereCrown(sphere(vec3(0, 8, 0), 4));
	//our points
	int numPoints = 50;
	vec3* points = new vec3[numPoints];
	crown->generate(points, numPoints);

	//draw points+crown
	glColor4f(1, 0, 0, 1.0f);
	for (int i = 0; i < numPoints; i++) {
		glPushMatrix();
		glTranslatef(points[i].x, points[i].y, points[i].z);
		glutSolidSphere(0.2f, 4, 4);
		glPopMatrix();
	}
	crown->display();

	//Seg
	Segment seg = Segment(vec3(0, 0, 0), vec3(0, 1, 0));
	seg.display();


	//
	//glutSolidSphere(0.5f,4,4);
	//glPushMatrix   ();

	//glPopMatrix     ();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	// factor all camera ops into projection matrix
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 60.0);
	gluLookAt(-20, 10, 0, // eye
		0, 0, 0, // center
		0.0, 1.0, 0.0); // up

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 * 
 */
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("tree generation using space-colonization algorithm");
	glutDisplayFunc(& display);
	glutReshapeFunc(& reshape);
	init();
	glutMainLoop();
	return (EXIT_SUCCESS);
}

