// HoleInTheWall.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>
#include <Ole2.h>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include <iostream>

#include "Drawing.h"

#define width 640
#define height 480

#define sceletonCorrX 90
#define sceletonCorrY 80
#define sceletonCorrZ 0

#define sceletonScaleX 1.6
#define sceletonScaleY 1.3
#define sceletonScaleZ 0


enum
{
	FULL_WINDOW, // aspekt obrazu - całe okno
	ASPECT_1_1, // aspekt obrazu 1:1
	EXIT // wyjście
};

int Aspect = FULL_WINDOW;


// OpenGL Variables

Wall* testWall;

GLuint textureId;              // ID of the texture to contain Kinect RGB Data

GLuint cameraPosZ, cameraPosX, cameraPosY;	//Camera position

// Kinect variables

HANDLE rgbStream;              // The identifier of the Kinect's RGB Camera

INuiSensor* sensor;            // The kinect sensor

int iterator = 0;

struct jointScreenCo{
	float positionX;
	float positionY;
};

jointScreenCo   m_Points[NUI_SKELETON_POSITION_COUNT];

void output(int x, int y, float r, float g, float b, int font, char *string)
{
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
}

bool initKinect() {
	// Get a working kinect sensor
	int numSensors;
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
	if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

	// Initialize sensor
	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_SKELETON);
/*	sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_COLOR,            // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,    // Image resolution
		0,      // Image stream flags, e.g. near mode
		2,      // Number of frames to buffer
		NULL,   // Event handle
		&rgbStream);*/

	return sensor;
}



void DrawBone(const NUI_SKELETON_DATA & skel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1)
{
	NUI_SKELETON_POSITION_TRACKING_STATE joint0State = skel.eSkeletonPositionTrackingState[joint0];
	NUI_SKELETON_POSITION_TRACKING_STATE joint1State = skel.eSkeletonPositionTrackingState[joint1];

	std::cout << "Jestem w drawBonr!!";

	// If we can't find either of these joints, exit
	if (joint0State == NUI_SKELETON_POSITION_NOT_TRACKED || joint1State == NUI_SKELETON_POSITION_NOT_TRACKED)
	{
		return;
	}

	// Don't draw if both points are inferred
	if (joint0State == NUI_SKELETON_POSITION_INFERRED && joint1State == NUI_SKELETON_POSITION_INFERRED)
	{
		return;
	}

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if (joint0State == NUI_SKELETON_POSITION_TRACKED && joint1State == NUI_SKELETON_POSITION_TRACKED)
	{
		jointScreenCo temp0 = m_Points[joint0];
		jointScreenCo temp1 = m_Points[joint1];

		//m_pRenderTarget->DrawLine(m_Points[joint0], m_Points[joint1], m_pBrushBoneTracked, g_TrackedBoneThickness);
		glPushMatrix();
		glTranslatef(sceletonCorrX, sceletonCorrY, 0);
		glScalef(sceletonScaleX, sceletonScaleY, sceletonCorrZ);
			glBegin(GL_LINES);
				glVertex3f(temp0.positionX, temp0.positionY, 0);
				glVertex3f(temp1.positionX, temp1.positionY, 0);
			glEnd();
		glPopMatrix();
		
	}
	else
	{
		//m_pRenderTarget->DrawLine(m_Points[joint0], m_Points[joint1], m_pBrushBoneInferred, g_InferredBoneThickness);
		jointScreenCo temp0 = m_Points[joint0];
		jointScreenCo temp1 = m_Points[joint1];
		glPushMatrix();
		glTranslatef(sceletonCorrX, sceletonCorrY, 0);
		glScalef(sceletonScaleX, sceletonScaleY, sceletonCorrZ);
			glBegin(GL_LINES);
				glVertex3f(temp0.positionX, temp0.positionY, 0);
				glVertex3f(temp1.positionX, temp1.positionY, 0);
			glEnd();
		glPopMatrix();
	}
}

jointScreenCo SkeletonToScreen(Vector4 skeletonPoint, int w, int h)
{
	LONG x, y;
	USHORT depth;

	// Calculate the skeleton's position on the screen
	// NuiTransformSkeletonToDepthImage returns coordinates in NUI_IMAGE_RESOLUTION_320x240 space
	NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth);

	float screenPointX = static_cast<float>(x * w) / glutGet(GLUT_WINDOW_WIDTH);
	float screenPointY = static_cast<float>(y * h) / glutGet(GLUT_WINDOW_HEIGHT);

	jointScreenCo tempResult;

	tempResult.positionX = screenPointX;
	tempResult.positionY = screenPointY;

	return tempResult;
}

void DrawSkeleton(const NUI_SKELETON_DATA & skel, int windowWidth, int windowHeight){

	int i = 0;

	for (i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i)
	{
		m_Points[i] = SkeletonToScreen(skel.SkeletonPositions[i], windowWidth, windowHeight);
	}

	// Render Torso
	DrawBone(skel, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
	DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
	DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
	DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
	DrawBone(skel, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
	DrawBone(skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
	DrawBone(skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);

	// Left Arm
	DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
	DrawBone(skel, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
	DrawBone(skel, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

	// Right Arm
	DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
	DrawBone(skel, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
	DrawBone(skel, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

	// Left Leg
	DrawBone(skel, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
	DrawBone(skel, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
	DrawBone(skel, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

	// Right Leg
	DrawBone(skel, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
	DrawBone(skel, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
	DrawBone(skel, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);

}

void ProcessSkeleton(){
	NUI_SKELETON_FRAME skeletonFrame = { 0 };

	HRESULT hr = sensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);
	if (FAILED(hr))
	{
		return;
	}

	// smooth out the skeleton data
	sensor->NuiTransformSmooth(&skeletonFrame, NULL);

	for (int i = 0; i < NUI_SKELETON_COUNT; ++i)
	{
		NUI_SKELETON_TRACKING_STATE trackingState = skeletonFrame.SkeletonData[i].eTrackingState;

		if (NUI_SKELETON_TRACKED == trackingState)
		{
			// We're tracking the skeleton, draw it
		    DrawSkeleton(skeletonFrame.SkeletonData[i], width, height);
		}
		else if (NUI_SKELETON_POSITION_ONLY == trackingState)
		{
			// we've only received the center point of the skeleton, draw that
			//	D2D1_ELLIPSE ellipse = D2D1::Ellipse(
			//	SkeletonToScreen(skeletonFrame.SkeletonData[i].Position, width, height),
			//	g_JointThickness,
			//	g_JointThickness
			//	);

			//m_pRenderTarget->DrawEllipse(ellipse, m_pBrushJointTracked);
		}
	}

}

void draw() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 1, -1);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT);

	glClear(GL_DEPTH_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	ProcessSkeleton();

	testWall->Draw();

	glRasterPos2i(15, 20);
	char text[10];
	
	sprintf_s(text, "%d", 5);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text);
	// kolor krawêdzi szeœcianu

	// skierowanie poleceñ do wykonania
	glFlush();

	// zamiana buforów koloru
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	// obszar renderingu - całe okno
	glViewport(0, 0, w, h);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bryły obcinania
	if (Aspect == ASPECT_1_1)
	{
		// wysokość okna większa od wysokości okna
		if (w < h && w > 0)
			glOrtho(-2.0, 2.0, -2.0 * h / w, 2.0 * h / w, -2.0, 2.0);
		else

			// szerokość okna większa lub równa wysokości okna
			if (w >= h && h > 0)
				glOrtho(-2.0 * w / h , 2.0 * w / h, -2.0, 2.0, -2.0, 2.0);

	}
	else
		glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);

	// generowanie sceny 3D
	draw();
}

void timerFunc(int value) {
	testWall->Rescale(1);
	draw();
	glutTimerFunc(10, timerFunc, value);
}

int main(int argc, char* argv[])
{
	if (!initKinect()) return 1;

	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	testWall = new Wall(200, 200, 0, 300, 200, 1, 0, 0);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// rozmiary g³ównego okna programu
	glutInitWindowSize(width, height);

	glutCreateWindow("Kinect");

	glutTimerFunc(2000, timerFunc, 0);

	glutDisplayFunc(draw);

	glutReshapeFunc(Reshape);

	glutIdleFunc(draw);

	// Main loop
	glutMainLoop();
	return 0;
}

