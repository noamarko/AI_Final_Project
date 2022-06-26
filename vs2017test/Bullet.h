#pragma once
#include "Room.h"

const double SPEED = 0.02;
const int BULLET_MOVEMENT = 10;

class Bullet
{
private:
	double x, y;
	double dirx, diry;
	bool isMoving;

public:
	Bullet();
	Bullet(double bx, double by, bool mv, double dx, double dy);

	bool GetIsMoving();
	double GetX();
	double GetY();

	void SetX(double x);
	void SetY(double y);
	void SetDirX(double dx);
	void SetDirY(double dy);
	void StopMoving();

	void Shoot();
	void Move(int maze[MSZ][MSZ]);
	void Draw();

};

