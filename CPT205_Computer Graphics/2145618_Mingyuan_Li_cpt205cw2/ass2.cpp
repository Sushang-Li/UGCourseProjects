//﻿ Name: Mingyuan Li
// ID: 2145618
//
//----------------------------------------------Description-----------------------------------------------//
// This is an OpenGL program only leveraging freeglut to draw a corner of the XJTLU campus(Central Building) 
// with 3D geometry creating, viewing, constructive solid geometry, transformation, hierarchical modeling, 
// lightening and materials effects, texture mapping, animation, and so on.
//--------------------------------------------------------------------------------------------------------//
// 
//----------------------------------------------Instructions----------------------------------------------//
//							 
//		Press 'a' to move the camera position to the left.
//		Press 'd' to move the camera position to the right.
//		Press 'w' to move the camera position to the up.
//		Press 's' to move the camera position to the down.
// 
//		Press 'i' to change the camera orientation up.
//		Press 'k' to change the camera orientation down.
//		Press 'j' to change the camera orientation left.
//		Press 'l' to change the camera orientation right.
// 
//		Press 'z' to move the camera position closer.
//		Press 'x' to move the camera position farther.
//		Press 'o' to zoom in the scene.
//		Press 'p' to zoom out the scene.
//		Press '0' to exit the program.
//		Press 'q' to control all vehicles to pause or continue movement.
// 
//		Pressing the left mouse button and dragging the mouse also change the camera orientation.
//		Sliding the mouse wheel to zoom in and out the scene as well.
//																 		
//--------------------------------------------------------------------------------------------------------//

#define FREEGLUT_STATIC 
#include <GL/freeglut.h> 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
using namespace std;

#define PI 3.1415927

int intWinWidth = 800;				//Default window size
int intWinHeight = 600;				//Default window size

// camera setting
double camera_speed = 10;
float cameraZoom = 1.05;			
float cameraX = -20.0;				
float cameraY = 30.0;
float cameraZ = 70.0;
float targetPointX = 0.0;			
float targetPointY = 0.0;
float targetPointZ = 0.0;
float upVectorX = 0.0;				
float upVectorY = 1.0;
float upVectorZ = 0.0;
int zoomming = 0;
int dragging = 0;
GLfloat mouseX;
GLfloat mouseY;
GLfloat mouseZ;

//lighting setting
GLfloat ambient0[] = { 0.14f, 0.19f, 0.19f, 1.0f };
GLfloat diffuse0[] = { 0.05f, 0.3f, 0.05f, 1.0f };
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPos0[] = { 200.0f, 100.0f, 50.0f, 1.0f };
GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat ambient1[] = { 0.4, 0.28, 0.25, 1.0 };
GLfloat diffuse1[] = { 0.10, 0.00, 0.30, 1.0 };
GLfloat specular1[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightPos1[] = { 3.0, 30.0, -30 };

GLfloat ambient2[] = { 0.3f, 0.3f, 0.3f, 0.3f };
GLfloat diffuse2[] = { 0.1f, 0.1f, 0.1f, 0.1f };
GLfloat specular2[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat lightPos2[] = { 28.0, 30, 10.0 };
GLfloat shine[] = { 25.0 };

// Material parameters
GLfloat specular_white[] = { 1.00, 1.00, 1.00, 1.0 };
GLfloat ambient_tree_1[] = { 0.02 , 0.91 , 0.02 , 1.0 };
GLfloat diffues_tree_1[] = { 0.12 , 0.95, 0.14, 1.0 };
GLfloat ambient_tree[] = { 0.62 , 0.21 , 0.02 , 1.0 };
GLfloat diffues_tree[] = { 0.62 , 0.35, 0.14, 1.0 };
GLfloat ambient_road[] = { 0.2 , 0.2 , 0.2 , 1.0 };
GLfloat diffues_road[] = { .1 , .1, .1, 1.0 };
GLfloat waterColor[] = { 0.0, 0.0, 1.0, 0.5 };
GLfloat groundColor[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat cbColor[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat ambient_gary[] = { 0.1, 0.1, 0.1, 1.0 };  
GLfloat diffuse_gary[] = { 0.2, 0.2, 0.2, 1.0 };  
GLfloat ambient_silver[] = { 0.75, 0.75, 0.75, 1.0 };  
GLfloat diffuse_silver[] = { 0.85, 0.85, 0.85, 1.0 };  
GLfloat ambient_yellow[] = { 1.0, 1.0, 0.0, 1.0 };  
GLfloat diffuse_yellow[] = { 1.0, 1.0, 0.0, 1.0 };  

// Defining the image, vertex, and quad structure 
struct image {
	GLint imageWidth;
	GLint imageHeight;
	GLint pixelLength;
};
struct Vertex {
	GLfloat x, y, z;
};
struct Quad {
	int a, b, c, d;
};

// Defining arrays of vertices
Vertex vertex_1[8] = {
	{ -1.0,-1.0,1.0 },
	{ -1.0,1.0,1.0 },
	{ 1.0,1.0,1.0 },
	{ 1.0,-1.0,1.0 },
	{ -1.0,-1.0,-1.0 },
	{ -1.0,1.0,-1.0 },
	{ 1.0,1.0,-1.0 },
	{ 1.0,-1.0,-1.0 }
};
Vertex vertex_2[8] = {
	{ -1.0,-1.0,1.0 },
	{ -0.6,1.0,1.0 },
	{ 0.6,1.0,1.0 },
	{ 1.0,-1.0,1.0 },
	{ -1.0,-1.0,-1.0 },
	{ -0.6,1.0,-1.0 },
	{ 0.6,1.0,-1.0 },
	{ 1.0,-1.0,-1.0 }
};

// Texture variable
image imageTexture[20];
vector<GLubyte*>p;
GLuint textures[25];

// Trees and cars variables
float distanceBetweenTrees = 7.0;  
float carMoveX = 0.0;  
float carMoveY = 0.0;  
float carMoveZ = 0.0;  
bool isMoving = true;

// Reading in images
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength) {
	GLubyte* pixeldata;
	FILE* pfile;
	fopen_s(&pfile, path, "rb");
	if (pfile == 0) exit(0);
	fseek(pfile, 0x0012, SEEK_SET);
	fread(&imagewidth, sizeof(imagewidth), 1, pfile);
	fread(&imageheight, sizeof(imageheight), 1, pfile);
	pixellength = imagewidth * 3;
	while (pixellength % 4 != 0)pixellength++;
	pixellength *= imageheight;
	pixeldata = (GLubyte*)malloc(pixellength);
	if (pixeldata == 0) exit(0);
	fseek(pfile, 54, SEEK_SET);
	fread(pixeldata, pixellength, 1, pfile);
	p.push_back(pixeldata);
	fclose(pfile);
}

// Setting texture and related OpenGL parameters
void setTexture() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_FLAT);
	glEnable(GL_TEXTURE_2D);
	const char* pictures[4] = {
		"Facade005_1K-JPG_Color_clip.bmp",
		"cobblestone_03_diff_1k_clip.bmp",
		"Road003_1K-JPG_Color_clip.bmp",
		"rocky_trail_02_diff_1k_clip.bmp",
	};

	for (int i = 0; i < 4; i++) {
		// Read the texture image and store it in the corresponding imageTexture struct
		ReadImage(pictures[i], imageTexture[i].imageWidth, imageTexture[i].imageHeight, imageTexture[i].pixelLength);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Textures are created and bound to unique texture ids
		glGenTextures(1, &textures[i]);

		// Set the format and data type
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imageTexture[i].imageWidth, imageTexture[i].imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[i]);

		// Set how textures should be wrapped and filtered
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
}

// Normalize the 3D vectors
void normalizeVector(float vec[3]) {
	float vecSize = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	vec[0] = vec[0] / vecSize;
	vec[1] = vec[1] / vecSize;
	vec[2] = vec[2] / vecSize;
}

// Renders textured quadrilaterals
// --const Quad& q: Representing a quadrangle containing the indices of the four vertices
void renderTexturedQuad(const Quad& q, GLfloat size, int e) {
	// Calculate the coordinates of the three vertices of the quadrilateral in 3D space
	GLfloat p[3][3] = {
		{ vertex_2[q.a].x * size, vertex_2[q.a].y * size, vertex_2[q.a].z * size },
		{ vertex_2[q.b].x * size, vertex_2[q.b].y * size, vertex_2[q.b].z * size },
		{ vertex_2[q.c].x * size, vertex_2[q.c].y * size, vertex_2[q.c].z * size }
	};

	//Calculate the cross product of the two vectors, which is used to calculate the normal vector
	GLfloat vec1[3] = { p[1][0] - p[0][0], p[1][1] - p[0][1], p[1][2] - p[0][2] };
	GLfloat vec2[3] = { p[2][0] - p[0][0], p[2][1] - p[0][1], p[2][2] - p[0][2] };
	GLfloat out[3] = { vec1[1] * vec2[2] - vec1[2] * vec2[1], vec1[2] * vec2[0] - vec1[0] * vec2[2], vec1[0] * vec2[1] - vec1[1] * vec2[0] };

	normalizeVector(out);

	glBindTexture(GL_TEXTURE_2D, textures[e]);

	glBegin(GL_QUADS);
	glNormal3f(out[0], out[1], out[2]);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(vertex_1[q.a].x * size, vertex_1[q.a].y * size, vertex_1[q.a].z * size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(vertex_1[q.b].x * size, vertex_1[q.b].y * size, vertex_1[q.b].z * size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(vertex_1[q.c].x * size, vertex_1[q.c].y * size, vertex_1[q.c].z * size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(vertex_1[q.d].x * size, vertex_1[q.d].y * size, vertex_1[q.d].z * size);
	glEnd();
}

// Similar to renderTexturedQuad. Different quads.
void renderTexturedQuad2(const Quad& q, GLfloat size, int e) {
	GLfloat p[3][3] = {
		{ vertex_1[q.a].x * size, vertex_1[q.a].y * size, vertex_1[q.a].z * size },
		{ vertex_1[q.b].x * size, vertex_1[q.b].y * size, vertex_1[q.b].z * size },
		{ vertex_1[q.c].x * size, vertex_1[q.c].y * size, vertex_1[q.c].z * size }
	};

	GLfloat vec1[3] = { p[1][0] - p[0][0], p[1][1] - p[0][1], p[1][2] - p[0][2] };
	GLfloat vec2[3] = { p[2][0] - p[0][0], p[2][1] - p[0][1], p[2][2] - p[0][2] };
	GLfloat out[3] = { vec1[1] * vec2[2] - vec1[2] * vec2[1], vec1[2] * vec2[0] - vec1[0] * vec2[2], vec1[0] * vec2[1] - vec1[1] * vec2[0] };

	normalizeVector(out);

	glBindTexture(GL_TEXTURE_2D, textures[e]);

	glBegin(GL_QUADS);
	glNormal3f(out[0], out[1], out[2]);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(vertex_2[q.a].x * size, vertex_2[q.a].y * size, vertex_2[q.a].z * size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(vertex_2[q.b].x * size, vertex_2[q.b].y * size, vertex_2[q.b].z * size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(vertex_2[q.c].x * size, vertex_2[q.c].y * size, vertex_2[q.c].z * size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(vertex_2[q.d].x * size, vertex_2[q.d].y * size, vertex_2[q.d].z * size);
	glEnd();
}

// Draws a textured cube(normal)
void drawCube(double x, double y, GLfloat size, int texe, int texf)
{
	glPushMatrix();

	// Define the six faces of the cube
	glScalef((GLfloat)x, (GLfloat)y, 1.0f);
	Quad front = { 0, 3, 2, 1 };
	Quad left = { 4, 0, 1, 5 };
	Quad back = { 7, 4, 5, 6 };
	Quad right = { 7, 6, 2, 3 };
	Quad top = { 1, 2, 6, 5 };
	Quad bottom = { 0, 4, 7, 3 };

	renderTexturedQuad(front, size, texf);
	renderTexturedQuad(left, size, texf);
	renderTexturedQuad(back, size, texf);
	renderTexturedQuad(right, size, texe);
	renderTexturedQuad(top, size, texe);
	renderTexturedQuad(bottom, size, texe);

	glPopMatrix();
}

// Draws a textured cube for the ground. For specific texture design purpose.
void drawCubePlate(double x, double y, GLfloat size, int texe, int texf)
{
	glPushMatrix();
	glScalef((GLfloat)x, (GLfloat)y, 1.0f);
	Quad front = { 0, 3, 2, 1 };
	Quad left = { 4, 0, 1, 5 };
	Quad back = { 7, 4, 5, 6 };
	Quad right = { 7, 6, 2, 3 };
	Quad top = { 1, 2, 6, 5 };
	Quad bottom = { 0, 4, 7, 3 };

	renderTexturedQuad(front, size, texf);
	renderTexturedQuad(left, size, texe);
	renderTexturedQuad(back, size, texf);
	renderTexturedQuad(right, size, texe);
	renderTexturedQuad(top, size, texe);
	renderTexturedQuad(bottom, size, texe);
	glPopMatrix();
}

// Draws a textured cube for the CB. For specific texture design purpose.
void drawCubeCB(double x, double y, GLfloat size, int texe, int texf)
{
	glPushMatrix();
	glScalef((GLfloat)x, (GLfloat)y, 1.0f);
	Quad front = { 0, 3, 2, 1 };
	Quad left = { 4, 0, 1, 5 };
	Quad back = { 7, 4, 5, 6 };
	Quad right = { 7, 6, 2, 3 };
	Quad top = { 1, 2, 6, 5 };
	Quad bottom = { 0, 4, 7, 3 };

	renderTexturedQuad(bottom, size, texf);
	renderTexturedQuad(left, size, texe);
	renderTexturedQuad(front, size, texe);
	renderTexturedQuad(right, size, texe);
	renderTexturedQuad(top, size, texf);
	renderTexturedQuad(back, size, texe);
	glPopMatrix();
}

// // Draws a textured cube for the CB's building partition. For specific texture design purpose.
void drawPartition(double x, double y, GLfloat size, int texe, int texf) {
	glPushMatrix();
	glScalef((GLfloat)x, (GLfloat)y, 1.0f);
	Quad front = { 0, 3, 2, 1 };
	Quad left = { 4, 0, 1, 5 };
	Quad back = { 7, 4, 5, 6 };
	Quad right = { 7, 6, 2, 3 };
	Quad top = { 1, 2, 6, 5 };
	Quad bottom = { 0, 4, 7, 3 };

	renderTexturedQuad(front, size, texe);
	renderTexturedQuad(left, size, texe);
	renderTexturedQuad(back, size, texe);
	renderTexturedQuad(right, size, texf);
	renderTexturedQuad(top, size, texe);
	renderTexturedQuad(bottom, size, texe);

	glPopMatrix();
}

// Draw a cylinder with two domes
void drawFullCylinder(GLfloat radius, GLfloat height)
{
	GLfloat x = 0.0; GLfloat y = 0.0;
	GLfloat angle = 0.0; GLfloat angle_stepsize = (GLfloat)0.1;

	// Draw the lateral surface of the cylinder using a quad strip
	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		glVertex3f(x, y, 0.0);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glVertex3f(radius, 0.0, 0.0);
	glEnd();

	// Draw the top circular cap using a polygon
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glEnd();

	// Draw the bottom circular cap using a polygon
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(x, y, 0.0);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, 0.0);
	glEnd();
}

// Draw a textured cube for trapezoid. For specific texture design purpose.
void drawTrapezoid(double x, double y, GLfloat size, int texe)
{
	glPushMatrix();
	glScalef((GLfloat)x, (GLfloat)y, 1.0f);

	Quad front = { 0, 3, 2, 1 };
	Quad left = { 4, 0, 1, 5 };
	Quad back = { 7, 4, 5, 6 };
	Quad right = { 7, 6, 2, 3 };
	Quad top = { 1, 2, 6, 5 };
	Quad bottom = { 0, 4, 7, 3 };

	renderTexturedQuad2(front, size, texe);
	renderTexturedQuad2(left, size, texe);
	renderTexturedQuad2(back, size, texe);
	renderTexturedQuad2(right, size, texe);
	renderTexturedQuad2(top, size, texe);
	renderTexturedQuad2(bottom, size, texe);
	glPopMatrix();
}

// Draw a textured cube for triangular. For specific texture design purpose.
void drawTriangular(double x, double y, GLfloat size, int texe)
{
	glPushMatrix();
	glScalef((GLfloat)x, (GLfloat)y, 1.0f);

	Quad front = { 0, 3, 3, 1 };
	Quad left = { 4, 0, 1, 5 };
	Quad back = { 7, 4, 5, 5 };
	Quad right1 = { 7, 0, 1, 3 };
	Quad right2 = { 0, 6, 2, 1 };
	Quad bottom = { 0, 4, 7, 3 };
	
	renderTexturedQuad2(front, size, texe);
	renderTexturedQuad2(left, size, texe);
	renderTexturedQuad2(back, size, texe);
	renderTexturedQuad2(right1, size, texe);
	renderTexturedQuad2(bottom, size, texe);
	glPopMatrix();
}

// Draw a Tesla cybertruck
void cybertruck(void) {
	// Body of cybertruck
	glPushMatrix();
	glScalef((GLfloat)1, (GLfloat)1, 2);
	drawCube(3.0, 1.0, 0.5, 9, 9);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, (GLfloat)0.95, 0.0);
	glScalef((GLfloat)1, (GLfloat)1, 2);
	drawTriangular(3, 1.0, (GLfloat)0.45, 4);
	glPopMatrix();

	// Wheels of cybertruck
	glPushMatrix();
	glTranslatef((GLfloat)-0.8, (GLfloat)-0.8, (GLfloat)0.8);
	drawFullCylinder((GLfloat)0.3, (GLfloat)0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef((GLfloat)0.8, (GLfloat)-0.8, (GLfloat)0.8);
	drawFullCylinder((GLfloat)0.3, (GLfloat)0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef((GLfloat)-0.8, (GLfloat)-0.8, (GLfloat)-1);
	drawFullCylinder((GLfloat)0.3, (GLfloat)0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef((GLfloat)0.8, (GLfloat)-0.8, (GLfloat)-1);
	drawFullCylinder((GLfloat)0.3, (GLfloat)0.3);
	glPopMatrix();
}

// Draw a car
void car()
{
	// Body of car
	glPushMatrix();
	glScalef((GLfloat)1, (GLfloat)1, 2);
	drawCube(3.0, 1.0, 0.5, 9, 9);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, (GLfloat)0.95, 0.0);
	drawTrapezoid(1.9, 1.0, (GLfloat)0.45, 4);
	glPopMatrix();

	// Wheels of car
	glPushMatrix();
	glTranslatef((GLfloat)-0.8, (GLfloat)-0.8, (GLfloat)0.8);
	drawFullCylinder((GLfloat)0.3, (GLfloat)0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef((GLfloat)0.8, (GLfloat)-0.8, (GLfloat)0.8);
	drawFullCylinder((GLfloat)0.3, (GLfloat)0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef((GLfloat)-0.8, (GLfloat)-0.8, (GLfloat)-1);
	drawFullCylinder((GLfloat)0.3, (GLfloat)0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef((GLfloat)0.8, (GLfloat)-0.8, (GLfloat)-1);
	drawFullCylinder((GLfloat)0.3, (GLfloat)0.3);
	glPopMatrix();
}

// The leaf part of a tree
void tree_part() {
	glPushMatrix();
	glTranslatef((GLfloat)-0.6, (GLfloat)2.3, (GLfloat)0.5);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	drawFullCylinder((GLfloat)0.7, (GLfloat)0.7);
	glPopMatrix();

	glPushMatrix();
	glTranslatef((GLfloat)0.6, (GLfloat)2.3, (GLfloat)0.5);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	drawFullCylinder((GLfloat)0.7, (GLfloat)0.7);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, (GLfloat)1.8, (GLfloat)0.5);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	drawFullCylinder((GLfloat)0.7, (GLfloat)0.7);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, (GLfloat)3, (GLfloat)0.5);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	drawFullCylinder((GLfloat)0.7, (GLfloat)0.7);
	glPopMatrix();
}

// Draw a tree
void tree()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_tree_1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffues_tree_1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);

	// The leaf part of a tree consisting of two sets of cylinders with opposite symmetry
	tree_part();

	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0);
	tree_part();
	glPopMatrix();

	// The trunk
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_tree);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffues_tree);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	glPushMatrix();
	glScalef((GLfloat)0.7, (GLfloat)1.7, 1);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	drawCube(0.1, 0.1, 1, 9, 9);
	glPopMatrix();
}

// Draw the base part of the CB using a rectangle
void drawCBbase(void) {
	glPushMatrix();
	glTranslatef(10, (GLfloat)15, 0.0);
	glRotatef(90.0, 0, 1.0, 0.0);
	glScalef((GLfloat)7, (GLfloat)7, (GLfloat)11);
	drawTrapezoid(4, 1, (GLfloat)1, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, (GLfloat)15, 10);
	glRotatef(270.0, 1, 0.0, 0.0);
	glScalef((GLfloat)7, (GLfloat)7, (GLfloat)7);
	drawTrapezoid(4, 1, (GLfloat)1, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, (GLfloat)15, -10);
	glRotatef(90.0, 1, 0.0, 0.0);
	glScalef((GLfloat)7, (GLfloat)7, (GLfloat)7);
	drawTrapezoid(4, 1, (GLfloat)1, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(28, (GLfloat)15, 22);
	glScalef((GLfloat)7, (GLfloat)7, (GLfloat)7);
	drawCube(1.4, 1, 1, 9, 9);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-8, (GLfloat)15, 22);
	glScalef((GLfloat)7, (GLfloat)7, (GLfloat)7);
	drawCube(1.4, 1, 1, 9, 9);
	glPopMatrix();
}

// Draw the CB
void drawCB(float cbmovey) {
	// CB main part
	glPushMatrix();
	glTranslatef(0.0, (cbmovey+9.65), 0.0);
	glBegin(GL_QUADS);
	drawCubeCB(30.0, 30.0, 0.5, 0, 9);
	glEnd;
	glPopMatrix();

	// Base part
	glPushMatrix();
	glTranslatef(0.0, (3 + cbmovey), 0.0);
	drawCBbase();
	glPopMatrix();

	// External wall partition
	glPushMatrix();
	glTranslatef(0.0,  cbmovey, 0.0);
	glPushMatrix();
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(11.3, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10.0, 30.7, 0.0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(11.3, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.5, 33.4, 0.0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(9, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.3, 36.1, 0.0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(6, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 38.8, 0.0);
	glRotatef(90.0, 90.0, 1.0, 1.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(3.5, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.5, 41.5, -6);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(10.3, 1.8, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 41.5, 0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(4, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 44.2, 0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(4, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 44.2, 0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(4, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.2, 46.9, 0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(4.3, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 46.9, 0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(4, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 49.6, 0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(4.6, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 49.6, 0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(4, 11.3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 52.3, 7.5);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(4.6, 6, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 52.3, -13);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(4.6, 3, 0.5, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 52.3, 0);
	glRotatef(90.0, 90.0, 1.0, 0.0);
	glScalef((GLfloat)3, (GLfloat)3, (GLfloat)1);
	drawPartition(4, 11.3, 0.5, 4, 4);
	glPopMatrix();
	glPopMatrix();
}

// Draw a road
void road(void) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_road);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffues_road);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);

	glPushMatrix();
	glTranslatef(1.5, 3, -49);
	glScalef(53.5, 1, 6);
	drawCube(1, 1, 1, 2, 9);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 0, 1.0, 0.0);
	glTranslatef(0, 4, -35);
	glScalef(67.5, 1, 6);
	drawCube(1, 1, 1, 2, 9);
	glPopMatrix();

	// Sidewalk in the campus
	for (int i = 0; i < 3; ++i) {
		glPushMatrix();
		glTranslatef((-17 + i * 28), 3, 45);  
		glScalef(14, 0.6, 14);
		drawCube(1, 1, 1, 1, 9);  
		glPopMatrix();
	}
}

// Draws a river using blending for transparency and alpha testing for fragment discarding.
void drawRiver() {
	// Disable lighting for the river
	// Enable depth testing and blending
	// Set the blending function for transparency
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Configure color mask, depth mask, and alpha testing
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);  
	glDepthMask(GL_FALSE);  
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);  

	// Draw the solid base of the river using a white cube
	glPushMatrix();
	glTranslatef(0.0, 1, -35);
	glScalef(54, 3, 5);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  
	drawCube(1, 1, 1, 9, 9);  
	glPopMatrix();

	// Restore rendering state
	glDisable(GL_ALPHA_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);

	// Draw the transparent water layer of the river
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, waterColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, waterColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	glTranslatef(0, 0.09, -35.9);
	glScalef(53, 3, 6);
	drawCube(1, 1, 1, 9, 9);  
	glPopMatrix();
}

// Create a tree with a straight line orientation
void drawTreesInRow(int numTrees) {
	for (int i = 0; i < numTrees; ++i) {
		glPushMatrix();
		glTranslatef(i * distanceBetweenTrees, 0.0, 0.0);  
		tree();  
		glPopMatrix();
	}
}

// display function
void display(void) 
{
	// Set the projection matrix to perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0.1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the modelview matrix using gluLookAt to position the camera at a setting point
	gluLookAt(	cameraX * cameraZoom, cameraY * cameraZoom, cameraZ * cameraZoom, 
				targetPointX, targetPointY, targetPointZ, 
				upVectorX, upVectorY, upVectorZ);
	glEnable(GL_DEPTH_TEST);										// Enable depth testing		
	glClearColor(1, 1, 1, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw ground
	glMaterialfv(GL_FRONT, GL_AMBIENT, groundColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, groundColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);  
	glPushMatrix();
	glTranslatef(0, -1, 0.0);
	glScalef((GLfloat)1.5, (GLfloat)8, (GLfloat)1.5);
	glRotatef(90.0, 1, 0.0, 0.0);
	drawCubePlate(70.0, 90.0, 0.5, 3, 9);
	glPopMatrix();

	// Draw CB
	glMaterialfv(GL_FRONT, GL_AMBIENT, cbColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cbColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	glColor3f(0.9, 0.4, 0.6);
	glPushMatrix();
	glTranslatef(10.0, 40.0, 0.0);
	glScalef((GLfloat)30, (GLfloat)30, (GLfloat)30);
	drawCB(-10);
	glPopMatrix();

	//Draw trees
	glPushMatrix();
	glTranslatef(-45.5, 3.0, 60.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawTreesInRow(12);  
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-25.5, 3.0, 26.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	drawTreesInRow(8);  
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-25, 3.0, -60.0);
	drawTreesInRow(11);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-25, 3.0, 63.0);
	drawTreesInRow(11);
	glPopMatrix();

	// Draw roads
	road();

	//draw cars
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_gary);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_gary);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	glPushMatrix();
	glTranslatef(-38, 6.0, (-60.0 + carMoveZ));
	glRotatef(90.0, 0.0, 1.0, 0.0);
	cybertruck();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_silver);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_silver);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	glPushMatrix();
	glTranslatef(-32.5, 6.0, (66.0 - carMoveZ));
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	car();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_yellow);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_yellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	glPushMatrix();
	glTranslatef((32.5 - carMoveX), 6.0, -50.0);
	car();
	glPopMatrix();

	// Draw river
	drawRiver();

	glutSwapBuffers();
	glLoadIdentity();
	glFlush();
}

// define keyboard function forkeyboard action
void keyboard_input(unsigned char key, int x, int y) {
	switch (key) {
		case 'a':
			cameraX -= camera_speed;
			break;
		case 'd':
			cameraX += camera_speed;
			break;
		case 'w':
			cameraY += camera_speed;
			break;
		case 's':
			cameraY -= camera_speed;
			break;
		case 'z':
			cameraZ += camera_speed;
			break;
		case 'x':
			cameraZ -= camera_speed;
			break;
		case 'o':
			cameraZoom -= 0.005;
			break;
		case 'p':
			cameraZoom += 0.005;
			break;
		case 'k':
			targetPointY += camera_speed;
			break;
		case 'i':
			targetPointY -= camera_speed;
			break;
		case 'j':
			targetPointX -= camera_speed;
			break;
		case 'l':
			targetPointX += camera_speed;
			break;
		case 'q':
			isMoving = !isMoving;
			break;
		case '0':
			exit(0);
	}
	glutPostRedisplay();
}

// define mouseclick function for mouseclick action
void mouseclick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		dragging = 1; 
		mouseX = (GLfloat)x; 
		mouseY = (GLfloat)y;
	}
	else
		dragging = 0;
}

// define mousemove function for mousemove action
void mousemove(int x, int y)
{
	if (dragging)
	{
		targetPointX += (x - mouseX) ;
		targetPointY -= (y - mouseY) ;
	}

	mouseX = (GLfloat)x;
	mouseY = (GLfloat)y;
	glutPostRedisplay();
}

// define mousewheel function for mousewheel action
void mousewheel(int wheel, int direction, int x, int y)
{
	if (direction > 0)
		cameraZoom -= 0.005f;
	else
		cameraZoom += 0.005f;

	if (cameraZoom > 2) 
		cameraZoom = 2;
	else if (cameraZoom < 0.5) 
		cameraZoom = 0.5;

	glutPostRedisplay();
}

// This will be called when the viewport is changed
void myReshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w, 1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5 * (GLfloat)w / (GLfloat)h, 1.5 * (GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Initialize lighting and related settings
void Initlight()
{
	glClearColor(1, 1, 1, 1);
	// Depth Buffer Setup
	glClearDepth(1.0f);
	// Enable face culling with counter-clockwise winding order
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_NORMALIZE);
}

void init()
{
	Initlight();

	// Set up textures and blending
	setTexture();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_BLEND);
	// Enable 2D textures and set the texture environment mode to modulate
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

// Update the car's position
void update(int value) {
	if (isMoving == true ) {
		carMoveX += 0.1;
		carMoveZ += 0.1;

		if (carMoveX > 58.0) {
			carMoveX = 58;
		}
		if (carMoveZ > 126.0) {
			carMoveZ = 0;
		}
	}
	glutPostRedisplay();  
	glutTimerFunc(16, update, 0);  
}

// main
int main(int argc, char* argv[]) 
{
	glutInit(&argc, (char**)argv); 
	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(600, 300); 
	glutInitWindowSize(intWinWidth, intWinHeight);
	glutCreateWindow("Ass2_Mingyuan.Li");

	glutKeyboardFunc(keyboard_input);
	glutMouseFunc(mouseclick);
	glutMotionFunc(mousemove);
	glutMouseWheelFunc(mousewheel);
	init();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display); 
	glutTimerFunc(25, update, 0);
	glutMainLoop(); 
	return 0; 
}
