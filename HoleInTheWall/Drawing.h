#include "stdafx.h"

#include <Windows.h>
#include <Ole2.h>

#include "GL\glew.h"
#include "GL\freeglut.h"

class Wall
{
private:
	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;

	GLfloat scaleFactorX;
	GLfloat scaleFactorY;
	GLfloat scaleFactorZ;

	GLfloat colorR;
	GLfloat colorG;
	GLfloat colorB;

	GLfloat sizeX;
	GLfloat sizeY;


	Wall() { } // private default constructor

public:
	Wall(GLfloat inX, GLfloat inY, GLfloat inZ, GLfloat sX, GLfloat sY, GLfloat inR, GLfloat inG, GLfloat inB);

	void Draw();
	void Rescale(int factor);
	void Init();
};