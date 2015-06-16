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