// File ID: cw1.cpp
// Title: 2D Assessment 1
// Author: Mingyuan Li

#define FREEGLUT_STATIC
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include <iostream>
using namespace std;

// Width and Height of Window
int windowWidth = 1000;
int windowHeight = 600;

// The Clouds
// x-axis position of clouds
float cloudX1 = 100.0;
float cloudX2 = 700.0;
float speed = 1.0;                      // Basic movement speed of clouds

// The hat
float rectColor[3] = { 1.0, 0.0, 0.0 }; // Tassel color, initially red
int tasselColorIndex = 0;               // Tassel color change index
GLfloat hatY = 0.0f;                    // y-axis position of clouds
bool hatMoved = false;                  // Hat movement change index
float rotateAngle = 0.0f;               // Tassel rotate angle

//The sun
float rotationSunAngle = 0.0f;          // Sun rotate angle

// The balloon
// x-axis position of balloons 
float positionX1 = 50;
float positionX2 = 240;
float positionX3 = 350;
float positionX4 = 650;
float positionX5 = 850;
float positionX6 = 430;
float positionX7 = 950;
float positionY6 = 200;
float positionY7 = 200;
float moveSpeed = 1;                    // Basic movement speed of balloons
int direction = 1;                      // Basic movement direction of balloons
bool isRising = false;                  // Ballons rising change index

// Draw the titel text function 
void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

// Mouse interaction function. Related to hat rise
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!hatMoved) {
            hatY += 50.0f;
            hatMoved = true;
        }
    }
}
// Keyboard event handler. 
void keyboard(unsigned char key, int x, int y) {
    //Toggle tassel color to next color (black, blue, red)
    if (key == 'e' || key == 'E') {
        tasselColorIndex = (tasselColorIndex + 1) % 3;
        switch (tasselColorIndex) {
        case 0:
            rectColor[0] = 0.0;
            rectColor[1] = 0.0;
            rectColor[2] = 0.0;
            break;
        case 1:
            rectColor[0] = 0.0;
            rectColor[1] = 0.0;
            rectColor[2] = 1.0;
            break;
        case 2:
            rectColor[0] = 1.0;
            rectColor[1] = 0.0;
            rectColor[2] = 0.0;
            break;
        }
        glutPostRedisplay();
    }

    // Tassel position rotation, 30 degrees per rotation
    if (key == 'r' || key == 'R') {
        rotateAngle += 30.0f;
        if (rotateAngle > 180.0f) {
            rotateAngle = 180.0f;
        }
        glutPostRedisplay();
    }

    // Balloons rising
    if (key == 'f' || key == 'F') {
        isRising = true; // 按下F键时开始上升
    }
}

// Draw clouds function
void drawCloud(float x, float y, float a, float b, float rotationAngle) {
    int segments = 100; // The number of segments, the larger it is the more computationally intensive and the finer the ellipse is.
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0); 
    //draw an ellipse. rotationAngle is for angle of ellipse
    for (int i = 0; i < segments; i++) {
        float angle = 2.0 * 3.1415926 * i / segments;
        float rx = a * cos(angle);
        float ry = b * sin(angle);

        float rotatedX = x + rx * cos(rotationAngle) - ry * sin(rotationAngle);
        float rotatedY = y + rx * sin(rotationAngle) + ry * cos(rotationAngle);

        glVertex2f(rotatedX, rotatedY);
    }
    glEnd();
}

// Draw balloons function
void drawBalloon(float positionX, float positionY, float wide, float r, float g, float b) {
    glColor3f(0, 0, 0); 
    glBegin(GL_LINES);
    glVertex2f(positionX, positionY+40);
    glVertex2f(positionX, positionY-50);
    glEnd();
    glColor3f(r, g, b);                                 // Color of balloons
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(positionX, positionY);                   // Central of circle
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159265359 / 180.0; 
        float x = positionX + wide * cos(angle); 
        float y = positionY+70 + (wide+5) * sin(angle); //The center of the circle is outside the circle so as to form a sector, \
                                                          and the y-axis radius is +5 to produce the convexity of the balloon.
        glVertex2f(x, y);
    }
    glEnd();
}

//Display function
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    //Set the OpenGL projection matrix to define the extent of the visible area on the screen
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 600);

    // The split height that divide the sky and land
    int splitHeight = windowHeight / 4;

    // The top part (sky) is set to blue
    glClearColor(0.529, 0.808, 0.980, 1.0);
    glScissor(0, splitHeight, windowWidth, windowHeight);
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    // The bottom part (green field) is set to light green
    glClearColor(0.565, 0.933, 0.565, 1.0);
    glScissor(0, 0, windowWidth, splitHeight);
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    // Horizontal dividing line (pure white)
    glColor3f(1, 1, 1);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f(0, splitHeight);
    glVertex2f(windowWidth, splitHeight);
    glEnd();

    // Display the title text
    glColor3f(1.0f, 1.0f, 1.0f); 
    drawText("2024 XJTLU Graduation Ceremony", 10, 10); 

    // Draw the outline of CB
    // Below part
    glColor3f(1.0, 1.0, 1.0); 
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(700, 150);// Bottom left corner
    glVertex2f(900, 150);// Bottom right corner
    glVertex2f(900, 210);// Top right corner
    glVertex2f(850, 210);// Upper depression point
    glVertex2f(700, 180);// Top left corner
    glEnd();
    // Connecting line
    glColor3f(1.0, 1.0, 1.0); 
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f(885, 210);
    glVertex2f(885, 230);
    glVertex2f(715, 183);
    glVertex2f(715, 230);
    glEnd();
    // Top part
    glColor3f(1.0, 1.0, 1.0); 
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(900, 230);
    glVertex2f(900, 350);
    glVertex2f(830, 350);
    glVertex2f(830, 330);
    glVertex2f(815, 330);
    glVertex2f(790, 350);
    glVertex2f(700, 350);
    glVertex2f(700, 230);
    glVertex2f(815, 230);
    glVertex2f(845, 275);
    glVertex2f(870, 275);
    glVertex2f(870, 230);
    glEnd();

    // XJTLU Logo
    // Outline of X
    glColor3ub(2, 30, 114); // Color changed
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(20, 150);
    glVertex2f(35, 150);
    glVertex2f(45, 215);
    glVertex2f(55, 150);
    glVertex2f(70, 150);
    glVertex2f(55, 225);
    glVertex2f(70, 300);
    glVertex2f(55, 300);
    glVertex2f(45, 235);
    glVertex2f(35, 300);
    glVertex2f(20, 300);
    glVertex2f(35, 225);
    glEnd();
    // Outline of Y
    glLineWidth(3.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 120; i++) {
        float angle = i * 3.1415926 / 180.0;
        float x = 105 + 30 * cos(angle); // Adjustment of radius and center point position
        float y = 218 - 50 * sin(angle); 
        glVertex2f(x, y);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 120; i++) {
        float angle = i * 3.1415926 / 180.0;
        float x = 105 + 45 * cos(angle); 
        float y = 200 - 50 * sin(angle); 
        glVertex2f(x, y);
    }
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(135, 215);
    glVertex2f(135, 300);
    glVertex2f(150, 200);
    glVertex2f(150, 300);
    glVertex2f(83, 156);
    glVertex2f(90, 175);
    glVertex2f(135, 300);
    glVertex2f(150, 300);
    glEnd();
    // Outline of T
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(160, 300);
    glVertex2f(160, 285);
    glVertex2f(190, 285);
    glVertex2f(190, 150);
    glVertex2f(205, 150);
    glVertex2f(205, 285);
    glVertex2f(235, 285);
    glVertex2f(235, 300);
    glEnd();
    //Outline of L
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(240, 150);
    glVertex2f(290, 150);
    glVertex2f(290, 165);
    glVertex2f(255, 165);
    glVertex2f(255, 300);
    glVertex2f(240, 300);
    glEnd();
    //Outline of U
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 180; i++) {
        float angle = i * 3.1415926 / 180.0;
        float x = 350 + 30 * cos(angle); 
        float y = 218 - 50 * sin(angle); 
        glVertex2f(x, y);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 180; i++) {
        float angle = i * 3.1415926 / 180.0;
        float x = 350 + 45 * cos(angle); 
        float y = 200 - 50 * sin(angle); 
        glVertex2f(x, y);
    }
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(380, 218);
    glVertex2f(380, 300);
    glVertex2f(396, 200);
    glVertex2f(396, 300);
    glVertex2f(380, 300);
    glVertex2f(396, 300);
    glVertex2f(320, 218);
    glVertex2f(320, 300);
    glVertex2f(305, 200);
    glVertex2f(305, 300);
    glVertex2f(305, 300);
    glVertex2f(320, 300);
    glEnd();
    glLineWidth(2.0f);    // Reset the line width

    // Clouds
    // Update the X-axie location of the cloud
    cloudX1 += speed;
    cloudX2 -= speed-0.4;               // Different speed      
    // When the cloud moves off the screen, move it to the left(right) of the screen
    if (cloudX1 > windowWidth + 100)
        cloudX1 = -100.0;
    if (cloudX2 < -100)
        cloudX2 = windowWidth + 100;
    // The first cloud
    drawCloud(cloudX1, 500, 50, 25, 0.0);
    drawCloud(cloudX1, 500, 40, 25, 15);
    drawCloud(cloudX1, 500, 40, 25, 95.0);
    // The second cloud
    glPushMatrix();
    glTranslatef(cloudX2, 550, 0.0f);
    glScalef(1.6, 1.6, 1.0f);               // Different size
    glTranslatef(-cloudX2, -550, 0.0f);
    drawCloud(cloudX2, 550, 50, 25, 0.0);
    drawCloud(cloudX2, 550, 40, 25, 15);
    drawCloud(cloudX2, 550, 40, 25, 95.0);
    glPopMatrix();

    // The hat
    glPushMatrix();
    glTranslatef(0, hatY, 0);
    // Set Smooth Shading Mode (GL_SMOOTH)
    glShadeModel(GL_SMOOTH);
    // Draw rectangle
    glBegin(GL_POLYGON);
    glColor3f(0.2, 0.2, 0.2);   // The upper left corner is light black
    glVertex2f(500, 150);
    glVertex2f(500, 220);
    glVertex2f(600, 220);
    glColor3f(0, 0, 0);         // The bottom right corner is dark black
    glVertex2f(600, 150);
    glEnd();
    // Draw parallelogram
    glBegin(GL_POLYGON);
    glColor3f(0.2, 0.2, 0.2); 
    glVertex2f(450, 225);
    glVertex2f(550, 270);
    glColor3f(0, 0, 0); 
    glVertex2f(650, 225);
    glVertex2f(550, 180);
    glEnd();
    // Draw tassel
    glTranslatef(550, 225, 0);
    glRotatef(rotateAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-550, -225, 0);
    glBegin(GL_LINES);
    glColor3f(0 ,0 ,0); 
    glVertex2f(550, 225);
    glVertex2f(650, 225);
    glEnd();
    // Calculate the rotation Angle in the opposite direction. 
    // In rotation, the tassel is always vertical in accordance with gravity logic
    GLfloat reverseRotateAngle = -rotateAngle;
    // Apply a rotation in the opposite direction
    glTranslatef(650, 225, 0);
    glRotatef(reverseRotateAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-650, -225, 0);
    glBegin(GL_LINES);
    glColor3f(0, 0, 0); 
    glVertex2f(650, 225);
    glVertex2f(650, 190);
    glEnd();
    glBegin(GL_QUADS);
    glColor3fv(rectColor); 
    glVertex2f(648, 150);
    glVertex2f(652, 150);
    glVertex2f(652, 190);
    glVertex2f(648, 190);
    glEnd();
    glPopMatrix();

    // The sun
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 0.53f, 0.0f);                   // Yellow
    glVertex2f(140.0f, 500.0f); 
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159265359 / 180.0; 
        float x = 140 + 35.0f * cos(angle); 
        float y = 500 + 35.0f * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    glPushMatrix(); 
    glTranslatef(140.0f, 500.0f, 0.0f); 
    glRotatef(rotationSunAngle, 0.0f, 0.0f, 1.0f);  // Rotate around the z-axis
    glTranslatef(-140.0f, -500.0f, 0.0f); 
    // Sunlight
    glBegin(GL_LINE_LOOP);
    for (float i = 0; i <= 360; i += 8.3) {
        float x = 140 + 50 * cos(i);
        float y = 500 + 50 * sin(i);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();

    //Display the balloons
    glPushMatrix();
    drawBalloon(positionX1, 370, 20, 1.0f, 0.53f, 0.5f);
    drawBalloon(positionX2, 370, 21, 0.8f, 0.7f, 0.1f);
    drawBalloon(positionX3, 390, 23, 0.2f, 0.9f, 0.8f);
    drawBalloon(positionX4, 400, 25, 1.0f, 0.1f, 0.8f);
    drawBalloon(positionX5, 430, 29, 1.0f, 0.5f, 0.0f);
    glPopMatrix();
    glPushMatrix();
    // The ground balloons
    drawBalloon(positionX6, positionY6, 24, 0.4f, 0.53f, 0.5f); 
    drawBalloon(positionX7, positionY7, 30, 0.8f, 0.2f, 0.1f);
    glPopMatrix();
    
    glFlush();
    glutSwapBuffers();
}

// Timer of Sun
void updateSun(int value) {
    rotationSunAngle += 1.0f;           // Increase the rotation Angle
    if (rotationSunAngle > 360.0f) {
        rotationSunAngle -= 360.0f;     // The restricted Angle is between 0 and 360 degrees
    }

    glutPostRedisplay();                // The notification needs to be redrawn

    glutTimerFunc(40, updateSun, 0);    // Set a timer to update every 16 milliseconds
}

// Timer of Balloons(x-axie)
void updateBallonX(int value) {
    positionX1 += moveSpeed * direction;
    positionX2 += moveSpeed * direction;
    positionX3 += moveSpeed * direction;
    positionX4 += moveSpeed * direction;
    positionX5 += moveSpeed * direction;
    if (positionY6 == 400) {
        positionX6 += moveSpeed * direction;        //6 and 7 only start to move from side to side after they rise
    }
    if (positionY7 == 430) {
        positionX7 += moveSpeed * direction;
    }
    if (positionX1 >= 1.0f || positionX1 <= -1.0f) {
        direction *= -1;                            // Switching directions
    }
    glutPostRedisplay();
    glutTimerFunc(120, updateBallonX, 0);  
}

// Timer of Balloons(y-axie)
void updateBallonY(int value) {
    if (isRising && positionY6 != 400) {
        positionY6 += 1.0f; 
    }
    if (isRising && positionY7 != 430) {
        positionY7 += 1.0f; 
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateBallonY, 0);  
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(400, 200);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("2024 XJTLU Graduation Card");

    glutDisplayFunc(display);
    glutMouseFunc(mouse);                   // Register mouse event handlers
    glutKeyboardFunc(keyboard);             // Register keyboard event handlers
    
    glutTimerFunc(0, updateSun, 0);         // Start the updateSun timer
    glutTimerFunc(0, updateBallonX, 0);     // Start the updateBallonX timer
    glutTimerFunc(0, updateBallonY, 0);     // Start the updateBallonY timer

    glClearColor(1.0, 1.0, 1.0, 1.0);       // Set the background color to white

    glutMainLoop();
}
