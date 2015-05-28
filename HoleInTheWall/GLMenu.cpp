#include "stdafx.h"

#include <Windows.h>
#include <Ole2.h>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include <iostream>

#define width 640
#define height 480

class Menu{
	public: Menu(void){

	}

	private:
		int numberOfEntries;

};

class MenuElement{
	private:
		GLuint positionX, positionY;
		unsigned char* displatText;
		GLuint backgroundColorR, backgroundColoG, backgroundColorB;
		GLuint textColorR, textColoG, textColorB;
		GLuint fontSize;
		char* font;

	public:
		void draw(){
			glRasterPos2i(100, 120);
			glColor4f(backgroundColorR, backgroundColoG, backgroundColorB, 1.0f);
			glutBitmapString(font, displatText);
		}
};