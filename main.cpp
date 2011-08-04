/* 
 * File:   main.cpp
 * Author: alex
 *
 * Created on 03 August 2011, 11:20
 */

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


#include "math.h"
#include "alg.h"

int numAttractionPoints = 1000;
float segmentLength=1.0f;
float influenceRadius = 4.0f;
float killDistance = 2.0f;

float camera=10.0f;

Crown* crown;
std::vector<Point> points;
std::vector<Segment> segments;

void SphereCrown::display() {
	glPushMatrix();
	glTranslatef(volume.position.x, volume.position.y, volume.position.z);
	glColor4f(0, 0, 0, 1.0f);
	glutWireSphere(volume.radius, 12, 12);
	glPopMatrix();
}

void Segment::display() {
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

void init() {
	crown = new SphereCrown(sphere(vec3(0, 8, 0), 4));
	points.resize(numAttractionPoints);
	crown->generateAttractionPoints(points);
	segments.resize(6);
	segments[0] = Segment(vec3(0, 0, 0), vec3(0, 1, 0));
	segments[1] = Segment(vec3(0, 1, 0), vec3(0, 2, 0), &segments[0]);
	segments[2] = Segment(vec3(0, 2, 0), vec3(0, 3, 0), &segments[1]);
	segments[3] = Segment(vec3(0, 3, 0), vec3(0, 4, 0), &segments[2]);
	segments[4] = Segment(vec3(0, 4, 0), vec3(0, 5, 0), &segments[3]);
	segments[5] = Segment(vec3(0, 5, 0), vec3(0, 6, 0), &segments[4]);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) 1280 / (GLfloat) 720, 1.0, 60.0);
	gluLookAt(-camera*2, camera, 0, // eye
		0, 0, 0, // center
		0.0, 1.0, 0.0); // up

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(1, 0, 0, 1.0f);
	for (int i = 0; i < points.size(); i++) {
		if (points[i].used) {
			glPushMatrix();
			glTranslatef(points[i].position.x, points[i].position.y, points[i].position.z);
			glutSolidSphere(0.2f, 4, 4);
			glPopMatrix();
		}
	}

	glColor4f(0, 0, 1, 1.0f);
	for (int i = 0; i < segments.size(); i++) {
		segments[i].display();
	}

	crown->display();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

}

void keyboard(unsigned char key,int x,int y){

	if(key==VK_RETURN) iteration(segments,points,segmentLength,influenceRadius,killDistance);
	display();
}

void special(int key,int x,int y){
	switch(key){
		case GLUT_KEY_UP: camera-=0.1;break;
		case GLUT_KEY_DOWN: camera+=0.1; break;
	}
	display();
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
	glutKeyboardFunc(& keyboard);
	glutSpecialFunc(& special);

	glClearColor(1, 1, 1, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	init();

	glutMainLoop();
	return (EXIT_SUCCESS);
}

