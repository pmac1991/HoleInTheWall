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

bool initKinect() {
	// Get a working kinect sensor
	int numSensors;
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
	if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

	// Initialize sensor
	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
	sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_COLOR,            // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,    // Image resolution
		0,      // Image stream flags, e.g. near mode
		2,      // Number of frames to buffer
		NULL,   // Event handle
		&rgbStream);
	return sensor;
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


int main(int argc, char argv[])
{
	initKinect();
	return 0;
}

