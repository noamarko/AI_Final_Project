#include "Details.h"

Details::Details(double x, double y, Soldier cyans[NUM_SOLDIERS], Soldier olives[NUM_SOLDIERS]): x(x), y(y), cyans(cyans), olives(olives), text(""), t("")
{
}


void Details::Draw()
{
	glColor3f(0.15, 0.15, 0.15);
	glBegin(GL_POLYGON);
	glVertex2d(x, y);
	glVertex2d(x + 0.50, y);
	glVertex2d(x + 0.50, y - 0.4);
	glVertex2d(x, y - 0.4);
	glEnd();

	if (cyans[0].GetHealth() == 0)
	{
		glColor3f(0.0, 1.0, 1.0);
		glLoadIdentity();
		strcpy(text, "Cyan 1: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.05, 0);
		glEnd();
	}
	else
	{
		glColor3f(0.0, 1.0, 1.0);
		glLoadIdentity();
		strcpy(text, "Cyan 1: H:");
		sprintf(t, " %d B: %d, G: %d, R: %d", cyans[0].GetHealth(), cyans[0].GetBulletsAmmo(), cyans[0].GetGrenadeAmmo(), cyans[0].GetRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.05, 0);
		glEnd();
	}


	if (cyans[1].GetHealth() == 0)
	{
		glColor3f(0.0, 1.0, 1.0);
		glLoadIdentity();
		strcpy(text, "Cyan 2: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.10, 0);
		glEnd();
	}
	else
	{
		glColor3f(0.0, 1.0, 1.0);
		glLoadIdentity();
		strcpy(text, "Cyan 2: H:");
		sprintf(t, " %d B: %d, G: %d, R: %d", cyans[1].GetHealth(), cyans[1].GetBulletsAmmo(), cyans[1].GetGrenadeAmmo(), cyans[1].GetRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.10, 0);
		glEnd();
	}
	if (cyans[2].GetHealth() == 0)
	{
		glColor3f(0.0, 1.0, 1.0);
		glLoadIdentity();
		strcpy(text, "Cyan 3: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.15, 0);
		glEnd();
	}
	else
	{
		glColor3f(0.0, 1.0, 1.0);
		glLoadIdentity();
		strcpy(text, "Cyan 3: H:");
		sprintf(t, " %d B: %d, G: %d, R: %d", cyans[2].GetHealth(), cyans[2].GetBulletsAmmo(), cyans[2].GetGrenadeAmmo(), cyans[2].GetRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.15, 0);
		glEnd();
	}
	if (olives[0].GetHealth() == 0)
	{
		glColor3f(0.5, 0.5, 0.0);
		glLoadIdentity();
		strcpy(text, "Olive 1: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.22, 0);
		glEnd();
	}
	else
	{
		glColor3f(0.5, 0.5, 0.0);
		glLoadIdentity();
		strcpy(text, "Olive 1: H:");
		sprintf(t, " %d B: %d, G: %d, R: %d", olives[0].GetHealth(), olives[0].GetBulletsAmmo(), olives[0].GetGrenadeAmmo(), olives[0].GetRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.22, 0);
		glEnd();
	}

	if (olives[1].GetHealth() == 0)
	{
		glColor3f(0.5, 0.5, 0.0);
		glLoadIdentity();
		strcpy(text, "Olive 2: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.27, 0);
		glEnd();
	}
	else
	{
		glColor3f(0.5, 0.5, 0.0);
		glLoadIdentity();
		strcpy(text, "Olive 2: H:");
		sprintf(t, " %d B: %d, G: %d, R: %d", olives[1].GetHealth(), olives[1].GetBulletsAmmo(), olives[1].GetGrenadeAmmo(), olives[1].GetRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.27, 0);
		glEnd();
	}
	if (olives[2].GetHealth() == 0)
	{
		glColor3f(0.5, 0.5, 0.0);
		glLoadIdentity();
		strcpy(text, "Olive 3: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.32, 0);
		glEnd();
	}
	else
	{
		glColor3f(0.5, 0.5, 0.0);
		glLoadIdentity();
		strcpy(text, "Olive 3: H:");
		sprintf(t, " %d B: %d, G: %d, R: %d", olives[2].GetHealth(), olives[2].GetBulletsAmmo(), olives[2].GetGrenadeAmmo(), olives[2].GetRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.32, 0);
		glEnd();
	}



}

void Details::drawBitmapText(const char* string, float x, float y, float z)
{
	const char* c;
	glRasterPos3f(x, y, z);

	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}
