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

#define width 640
#define height 480

// OpenGL Variables

GLuint textureId;              // ID of the texture to contain Kinect RGB Data

GLubyte data[width*height * 4];  // BGRA array containing the texture data

// Kinect variables

HANDLE rgbStream;              // The identifier of the Kinect's RGB Camera

INuiSensor* sensor;            // The kinect sensor

struct jointScreenCo{
	float positionX;
	float positionY;
};

jointScreenCo   m_Points[NUI_SKELETON_POSITION_COUNT];

bool initKinect() {
	// Get a working kinect sensor
	int numSensors;
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
	if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

	// Initialize sensor
	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);
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
		glBegin(GL_LINES);
			glVertex3f(temp0.positionX, temp0.positionY, 0);
			glVertex3f(temp1.positionX, temp1.positionY, 0);
		glEnd();
	}
	else
	{
		//m_pRenderTarget->DrawLine(m_Points[joint0], m_Points[joint1], m_pBrushBoneInferred, g_InferredBoneThickness);
		jointScreenCo temp0 = m_Points[joint0];
		jointScreenCo temp1 = m_Points[joint1];

		glBegin(GL_LINES);
			glVertex3f(temp0.positionX, temp0.positionY, 0);
			glVertex3f(temp1.positionX, temp1.positionY, 0);
		glEnd();
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


void getKinectData(GLubyte* dest) {
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if (sensor->NuiImageStreamGetNextFrame(rgbStream, 0, &imageFrame) < 0) return;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);

	if (LockedRect.Pitch != 0)
	{
		const BYTE* curr = (const BYTE*)LockedRect.pBits;
		const BYTE* dataEnd = curr + (width*height) * 4;

		while (curr < dataEnd) {
			*dest++ = *curr++;
		}
	}

	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(rgbStream, &imageFrame);
}

void drawKinectData() {
	glBindTexture(GL_TEXTURE_2D, textureId);
	getKinectData(data);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, height, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, height, 0.0f);
	glEnd();
}

void draw() {
	//drawKinectData();
	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	ProcessSkeleton();
	//glFlush();
	glutSwapBuffers();

}

void execute() {
	glutMainLoop();
}

bool init(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Kinect SDK Tutorial");
	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	return true;
}

int main(int argc, char* argv[])
{
	if (!init(argc, argv)) return 1;
	if (!initKinect()) return 1;

	// Initialize textures
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// OpenGL setup
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glEnable(GL_TEXTURE_2D);

	// Camera setup
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Main loop
	execute();
	return 0;
}

