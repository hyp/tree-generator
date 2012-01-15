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

#include <iostream>
#include <vector>

#include "alg.h"
#include "math_.h"
#include "mc.h"

#ifndef VK_RETURN
#define VK_RETURN 0x0D
#endif

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
	vec3 dir = end;
	dir.sub(start);
	dir.normalize();
	
	float radius = 0.15f;
	float progression = 0.0f;
	vec3 offset(0.0f,0.0f,0.0f);
	
	while(progression < segmentLength){
		glPushMatrix();
		glTranslatef(start.x+offset.x, start.y+offset.y, start.z+offset.z);
		glutSolidSphere(radius, 8, 8);
		glPopMatrix();
		progression+=radius;
		offset = dir;
		offset.mul(progression);
	}
}

std::vector<vec3> triangles;

void triangulate(){
	triangles.clear();
	marchCubes(vec3(-7.0,0.0,-7.0),vec3(14.0,14.0,14.0),32,triangles,&segments);
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
	
	triangulate();
}

void print(float x, float y, char* text) {
	glRasterPos2f(x, y);
	while (*text) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *text);
		text++;
	}
}

void drawLine(vec3 a,vec3 b,vec3 color){
	glBegin(GL_LINES);
	glColor3f(color.x,color.y,color.z);
	glVertex3f(a.x,a.y,a.z);
	glVertex3f(b.x,b.y,b.z);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) 1280 / (GLfloat) 720, 1.0, 60.0);
	gluLookAt(-1.0, camera, -camera * 2, // eye
		0, 0, 0, // center
		0.0, 1.0, 0.0); // up

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	drawLine(vec3(0,0,0),vec3(20,0,0),vec3(1,0,0));
	drawLine(vec3(0,0,0),vec3(0,20,0),vec3(0,1,0));
	drawLine(vec3(0,0,0),vec3(0,0,20),vec3(0,0,1));
	

	glColor4f(1, 0, 0, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_TRIANGLES);
	for(int i=0;i<triangles.size();i++){
		if(triangles[i].y < 5.0) glColor4f(107.0/255.0,66.0/255.0,38.0/255.0, 1.0f);
		else glColor4f(0,0.8,0, 1.0f);
		glVertex3f(triangles[i].x,triangles[i].y,triangles[i].z);
	}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glDisable(GL_DEPTH_TEST);
	glColor4f(0, 0, 1, 1.0f);
	for (int i = 0; i < segments.size(); i++) {
		drawLine(segments[i].start,segments[i].end,vec3(0,0,1));
	}
	glEnable(GL_DEPTH_TEST);
	
	int pointsRemaining = 0;
	/*for (int i = 0; i < points.size(); i++) {
		if (points[i].used) {
			glPushMatrix();
			glTranslatef(points[i].position.x, points[i].position.y, points[i].position.z);
			glutSolidSphere(0.2f, 4, 4);
			glPopMatrix();
			pointsRemaining++;
		}
	}
	glEnable(GL_LIGHTING);
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };    
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };   
	GLfloat LightPosition[]= { -6.0f, 8.0f, 2.0f, 1.0f };  
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); 
	glEnable(GL_LIGHT1);
	glColor4f(107.0/255.0,66.0/255.0,38.0/255.0, 1.0f);
	for (int i = 0; i < segments.size(); i++) {
		segments[i].display();
	}
	glDisable(GL_LIGHTING);*/

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
	sprintf(text, "%d segments generated with %d points remaining", (unsigned int)(segments.size()),pointsRemaining);
	print(-1, 0.75, text);

	print(-1, -1, "Controls: 'z'/'x' change segment length, 'c'/'v' change influence radius, 'b'/'n' change kill distance, 'Enter' run one iteration");

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
		0, 2.0, 0, // center
		0.0, 1.0, 0.0); // up

}

void keyboard(unsigned char key, int x, int y) {

	if (key == VK_RETURN){
		iteration(segments, points, segmentLength, influenceRadius, killDistance);
		triangulate();
	}
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

