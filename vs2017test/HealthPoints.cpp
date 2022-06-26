#include "HealthPoints.h"
#include <stdlib.h>

/* ---------- Constructors ---------- */

HealthPoints::HealthPoints()
{
	x = 0;
	y = 0;
	room_number = 0;
}

HealthPoints::HealthPoints(int lx, int ly, int room_number_location)
{
	x = lx;
	y = ly;
	room_number = room_number_location;
}

/* ---------- Getters ---------- */

int HealthPoints::GetX()
{
	return x;
}

int HealthPoints::GetY()
{
	return y;
}

int HealthPoints::GetRoomNumber()
{
	return room_number;
}

/* ---------- Setters ---------- */

void HealthPoints::SetX(int lx)
{
	x = lx;
}

void HealthPoints::SetY(int ly)
{
	y = ly;
}

void HealthPoints::SetRoomNumber(int room_number_location)
{
	room_number = room_number_location;
}