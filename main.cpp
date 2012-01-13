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
#include <vector>

#include "alg.h"
#include "math_.h"

int numAttractionPoints = 1000;
float segmentLength = 1.0f;
float influenceRadius = 4.0f;
float killDistance = 2.0f;

float camera = 10.0f;

Crown* crown = 0;
std::vector<Point> points;
std::vector<Segment> segments;

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

void Segment::display(){
	glBegin(GL_LINES);
	glVertex3f(start.x, start.y, start.z);
	glVertex3f(end.x, end.y, end.z);
	glEnd();
	return;
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
	if (crown) delete crown;
	crown = new SphereCrown(sphere(vec3(0, 8, 0), 4));
	points.resize(numAttractionPoints);
	crown->generate(points);
	int numSegs = 12;
	float segL = 0.5f;
	segments.resize(numSegs);
	for (int i = 0; i < numSegs; i++) {
		if (i) segments[i] = Segment(vec3(0, segL * i, 0), vec3(0, segL * i + segL, 0), &segments[i - 1]);
		else segments[i] = Segment(vec3(0, segL*i, 0), vec3(0, segL * i + segL, 0));
	}
}

void print(float x, float y, char* text) {
	glRasterPos2f(x, y);
	while (*text) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *text);
		text++;
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) 1280 / (GLfloat) 720, 1.0, 60.0);
	gluLookAt(-camera * 2, camera, 0, // eye
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

	//crown->display();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 0, 1280, 720, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor4f(0, 0, 0, 1.0f);
	glEnable(GL_BLEND);

	char text[512];
	sprintf(text, "Attraction points = %d", numAttractionPoints);
	print(-1, 0.95, text);
	sprintf(text, "Influence radius = %f", influenceRadius);
	print(-1, 0.9, text);
	sprintf(text, "Kill distance = %f", killDistance);
	print(-1, 0.85, text);
	sprintf(text, "Seg length = %f", segmentLength);
	print(-1, 0.8, text);

	print(-1, -1, "Controls: 'z'/'x' change segment length, 'c'/'v' change influence radius, 'b'/'n' change kill distance");

	glDisable(GL_BLEND);
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

}

void keyboard(unsigned char key, int x, int y) {

	if (key == VK_RETURN) iteration(segments, points, segmentLength, influenceRadius, killDistance);
	switch (key) {
		case 'z':segmentLength -= 0.05f;
			init();
			break;
		case 'x':segmentLength += 0.05f;
			init();
			break;
		case 'c':influenceRadius -= 0.1f;
			init();
			break;
		case 'v':influenceRadius += 0.1f;
			init();
			break;
		case 'b':killDistance -= 0.1f;
			init();
			break;
		case 'n':killDistance += 0.1f;
			init();
			break;
	}
	display();
}

void special(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: camera -= 0.1;
			break;
		case GLUT_KEY_DOWN: camera += 0.1;
			break;
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

	if (crown) delete crown;
	return (EXIT_SUCCESS);
}

