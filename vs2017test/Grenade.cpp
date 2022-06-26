#include "Grenade.h"
#include "glut.h"
#include <math.h>

/* ---------- Constructors ---------- */

Grenade::Grenade()
{
	x = 0;
	y = 0;
	targetx = 0;
	targety = 0;
	dirx = 1;
	diry = 0;
	isMoving = false;
	int i;
	double alpha, teta = 2 * PI / NUM_BULLETS;
	// init all bullets
	for (i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += teta)
	{
		bullets[i].SetX(x);
		bullets[i].SetY(y);
		bullets[i].SetDirX(cos(alpha));
		bullets[i].SetDirY(sin(alpha));
	}
}

Grenade::Grenade(double x, double y)
{
	double alpha, teta = 2*PI/NUM_BULLETS;
	int i;
	this->x = x;
	this->y = y;
	targetx = 0;
	targety = 0;
	dirx = 1;
	diry = 0;
	isMoving = false;
	// init all bullets
	for ( i = 0,alpha = 0; i < NUM_BULLETS; i++,alpha+=teta)
	{
		bullets[i].SetX(x);
		bullets[i].SetY(y);
		bullets[i].SetDirX(cos(alpha));
		bullets[i].SetDirY(sin(alpha));
	}

}

/* ---------- Getters ---------- */

double Grenade::GetX()
{
	return  MSZ * (x + 1) / 2;
}

double Grenade::GetY()
{
	return  MSZ * (y + 1) / 2;
}

bool Grenade::GetIsMoving()
{
	int i;
	/* If one of the grenade's bullets is still moving, the grenade is still exist */
	for (i = 0; i < NUM_BULLETS; i++)
	{
		if (bullets[i].GetIsMoving())
			return true;
	}

	/* To zero the grenade's target */
	targetx = 0;
	targety = 0;
	return false;
}

Bullet* Grenade::GetBulletFromIndex(int index)
{
	return &bullets[index];
}

int Grenade::GetTargetX()
{
	return targetx;
}

int Grenade::GetTargetY()
{
	return targety;
}

/* ---------- Setters ---------- */

void Grenade::SetX(double lx)
{

	x = lx;
	double alpha, teta = 2 * PI / NUM_BULLETS;
	int i;
	// update all bullets x location
	for (i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += teta)
	{
		bullets[i].SetX(x);
		bullets[i].SetDirX(cos(alpha));
	}

}

void Grenade::SetY(double ly)
{
	y = ly;
	double alpha, teta = 2 * PI / NUM_BULLETS;
	int i;
	// update all bullets y location
	for (i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += teta)
	{
		bullets[i].SetY(x);
		bullets[i].SetDirY(sin(alpha));
	}
}

void Grenade::SetDirX(double dx)
{
	dirx = dx;
}

void Grenade::SetDirY(double dy)
{
	diry = dy;
}

void Grenade::SetTargetX(int tx)
{
	targetx = tx;
}

void Grenade::SetTargetY(int ty)
{
	targety = ty;
}

/* ---------- Game Functions ---------- */

/* Throw grenade near the opponent */
void Grenade::Throw(int maze[MSZ][MSZ], int soldierX, int soldierY, int opponentX, int opponentY)
{
	double xdistance, ydistance, size, distance;
	int damage, row, col, i = 0;
	int randx = 0, randy = 0;

	if (targetx == 0 && targety == 0) // initialize target point
	{
		targetx = opponentX;
		targety = opponentY;
		// Check Up
		if (maze[opponentY + 1][opponentX] == SPACE)
			targety += 1;
		// Check Right
		else if (maze[opponentY][opponentX + 1] == SPACE)
			targetx += 1;
		// Check Left
		else if (maze[opponentY][opponentX - 1] == SPACE)
			targetx -= 1;
		// Check Down
		else if (maze[opponentY - 1][opponentX] == SPACE)
			targety -= 1;
		// Check Up - Right
		else if (maze[opponentY + 1][opponentX + 1] == SPACE)
		{
			targetx += 1;
			targety += 1;
		}
		// Check Up - Left
		else if (maze[opponentY + 1][opponentX - 1] == SPACE)
		{
			targetx -= 1;
			targety += 1;
		}
		// Check Down - Left
		else if (maze[opponentY - 1][opponentX - 1] == SPACE)
		{
			targetx -= 1;
			targety -= 1;
		}
		// Check Down - Right
		else if (maze[opponentY - 1][opponentX + 1] == SPACE)
		{
			targetx += 1;
			targety += 1;
		}
		isMoving = true;

		/* Calculate grenade direction */
		xdistance = (double)targetx - (double)soldierX;
		ydistance = (double)targety - (double)soldierY;
		size = sqrt(pow(xdistance, 2) + pow(ydistance, 2));
		dirx = xdistance / size;
		diry = ydistance / size;
	}

	while (i < GRENADE_MOVEMENT && isMoving) // Move The grenade x (as defined) times before the soldiers move
	{
		MoveGrenade();
		i++;
	}
	Draw();

}


void Grenade::MoveGrenade()
{
	int row, col;

	if (isMoving)
	{
		row = MSZ * (y + 1) / 2;
		col = MSZ * (x + 1) / 2;

		if (row == targety && col == targetx) // end of move
		{
			isMoving = false;
		}
		else // go on moving
		{
			if (row == targety)
				x += dirx * SPEED;
			else if (col == targetx)
				y += diry * SPEED;
			else
			{
				x += dirx * SPEED;
				y += diry * SPEED;
			}

			/* Move all the grenade's bullets */
			for (int i = 0; i < NUM_BULLETS; i++)
			{
				bullets[i].SetX(x);
				bullets[i].SetY(y);
			}
		}
	}
}

void Grenade::Explode()
{
	double alpha, teta = 2 * PI / NUM_BULLETS;
	int i;
	for ( i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += teta)
	{
		bullets[i].SetDirX(cos(alpha));
		bullets[i].SetDirY(sin(alpha));
		bullets[i].Shoot();
	}
}

void Grenade::Move(int maze[MSZ][MSZ])
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets[i].Move(maze);

}

void Grenade::Draw()
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets[i].Draw();
}

void Grenade::UpdateSecurityMap(int maze[MSZ][MSZ], double map[MSZ][MSZ])
{
	bool stop = false;
	int row, col,i;
	double delta = 0.00025;
	for (i = 0; i < NUM_BULLETS; i++)
		bullets[i].Shoot();

	while (!stop)
	{
		stop = true;		
		for ( i = 0; i < NUM_BULLETS; i++)
		{
			if (bullets[i].GetIsMoving())
			{
				stop = false;
				row = MSZ * (bullets[i].GetY() + 1) / 2;
				col = MSZ * (bullets[i].GetX() + 1) / 2;
				if (row >= 0 && row < MSZ && col >= 0 && col < MSZ && 
					(maze[row][col] == CYAN_SOLDIER || maze[row][col] == OLIVE_SOLDIER || maze[row][col] == SPACE))
				{
					map[row][col] += delta;
					bullets[i].Move(maze);
				}
				else
					bullets[i].StopMoving();
			}
		}
	}
}
