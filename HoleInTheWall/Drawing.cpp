#include "stdafx.h"
#include "Drawing.h"

void Wall::Draw()
{
	glPushMatrix();
	
		glColor3f(colorR, colorG,colorB);
		glTranslatef(positionX, positionY,positionZ);
		glScalef(scaleFactorX, scaleFactorY, 0);
		glBegin(GL_QUADS);
			
			glVertex3f(sizeX, 0, 0);
			glVertex3f(sizeX, sizeY, 0);
			glVertex3f(0, sizeY, 0);
			glVertex3f(0, 0, 0);
		
		glEnd();
	
	glPopMatrix();
}

void Wall::Rescale(GLfloat percent)
{
	scaleFactorX = scaleFactorX * percent;
	scaleFactorY = scaleFactorY * percent;
	//scaleFactorZ = (scaleFactorZ * 100) / percent;
}

// TODO write it !

bool Wall::checkPosition(jointScreenCo inputCoordinates[], float accuracy)
{
	int inputSize = sizeof inputCoordinates / sizeof(*inputCoordinates);

	for (int i = 0; i<16; i++)
	{
		if (((skeletonJoints[i].positionX + accuracy) > inputCoordinates[i].positionX) && ((skeletonJoints[i].positionX - accuracy) < inputCoordinates[i].positionX) &&
			((skeletonJoints[i].positionY + accuracy) > inputCoordinates[i].positionY) && ((skeletonJoints[i].positionY - accuracy) < inputCoordinates[i].positionY))
		{

		}
		else
		{
			return false;
		}
	}
	return true;
}

int Wall::countCorrectJointsPosition(jointScreenCo inputCoordinates[], float accuracy)
{
	int inputSize = sizeof inputCoordinates / sizeof(*inputCoordinates);

	int numberOfCorrectJoints = 0;

	for (int i = 0; i<16; i++)
	{
		if (((skeletonJoints[i].positionX + accuracy) > inputCoordinates[i].positionX) && ((skeletonJoints[i].positionX - accuracy) < inputCoordinates[i].positionX) &&
			((skeletonJoints[i].positionY + accuracy) > inputCoordinates[i].positionY) && ((skeletonJoints[i].positionY - accuracy) < inputCoordinates[i].positionY))
		{
			numberOfCorrectJoints++;
		}
	}
	return numberOfCorrectJoints;
}

Wall::Wall(GLfloat inX, GLfloat inY, GLfloat inZ, GLfloat sX, GLfloat sY, GLfloat inR, GLfloat inG, GLfloat inB)
{
	positionX = inX;
	positionY = inY;
	positionZ = inZ;

	sizeX = sX;
	sizeY = sY;

	colorR = inR;
	colorG = inG;
	colorB = inB;

	scaleFactorX = 1;
	scaleFactorY = 1;
	scaleFactorZ = 1;
}