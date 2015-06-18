#include "stdafx.h"

#include <Windows.h>
#include <Ole2.h>

#include "GL\glew.h"
#include "GL\freeglut.h"

struct jointScreenCo{
	float positionX;
	float positionY;
};

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
	
	jointScreenCo skeletonJoints[16];

	jointScreenCo ankle_right;
	jointScreenCo knee_right;
	jointScreenCo hip_right;

	jointScreenCo ankle_left;
	jointScreenCo knee_left;
	jointScreenCo hip_left;

	jointScreenCo hip_center;
	jointScreenCo spine;
	jointScreenCo shoulder_center;
	jointScreenCo head;

	jointScreenCo wrist_right;
	jointScreenCo elbow_right;
	jointScreenCo shoulder_right;

	jointScreenCo wrist_left;
	jointScreenCo elbow_left;
	jointScreenCo shoulder_left;



	Wall() { } // private default constructor

public:
	Wall(GLfloat inX, GLfloat inY, GLfloat inZ, GLfloat sX, GLfloat sY, GLfloat inR, GLfloat inG, GLfloat inB);

	bool checkPosition(jointScreenCo inputCoordinates[], float accuracy);
	int countCorrectJointsPosition(jointScreenCo inputCoordinates[], float accuracy);
	void Draw();
	void Rescale(GLfloat factor);
	void Init();
};