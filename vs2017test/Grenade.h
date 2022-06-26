#pragma once
#include <vector>
#include "Bullet.h"

const int NUM_BULLETS = 15;
const double PI = 3.14;
const int GRENADE_MOVEMENT = 2;

class Grenade
{
private:
	Bullet bullets[NUM_BULLETS];
	double  x, y;
	double dirx, diry;
	bool isMoving;
	int targetx, targety;

public:
	Grenade();
	Grenade(double x, double y);
	double GetX();
	double GetY();
	bool GetIsMoving();
	Bullet* GetBulletFromIndex(int index);
	int GetTargetX();
	int GetTargetY();

	void SetX(double x);
	void SetY(double y);
	void SetDirX(double dx);
	void SetDirY(double dy);
	void SetTargetX(int tx);
	void SetTargetY(int ty);

	void Throw(int maze[MSZ][MSZ], int soldierX, int soldierY, int x, int y);
	void MoveGrenade();
	void Explode();
	void Move(int maze[MSZ][MSZ]);
	void Draw();
	void UpdateSecurityMap(int maze[MSZ][MSZ], double map[MSZ][MSZ]);


};

