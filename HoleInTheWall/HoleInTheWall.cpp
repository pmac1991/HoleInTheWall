// HoleInTheWall.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>
#include <Ole2.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include "GL\glew.h"
#include "GL\freeglut.h"



// Kinect variables

HANDLE rgbStream;              // The identifier of the Kinect's RGB Camera

INuiSensor* sensor;            // The kinect sensor

bool initKinect();

int main(int argc, char argv[])
{
	initKinect();
	return 0;
}

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
