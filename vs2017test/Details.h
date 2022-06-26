#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Soldier.h"
#include "glut.h"


const int S = 30;

class Details
{
private:
	double x, y;
	Soldier* cyans;
	Soldier* olives;
	char text[S];
	char t[S];

public:
	Details(double x, double y, Soldier cyans[NUM_SOLDIERS], Soldier olives[NUM_SOLDIERS]);
	void Draw();
	void drawBitmapText(const char* string, float x, float y, float z);

};