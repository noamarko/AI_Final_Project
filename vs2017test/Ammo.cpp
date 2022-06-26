#include "Ammo.h"
#include <stdlib.h>

/* ---------- Constructors ---------- */

Ammo::Ammo()
{
	x = 0;
	y = 0;
	room_number = 0;
}

Ammo::Ammo(int lx, int ly, int room_number_location)
{
	x = lx;
	y = ly;
	room_number = room_number_location;
}

/* ---------- Getters ---------- */

int Ammo::GetX()
{
	return x;
}

int Ammo::GetY()
{
	return y;
}

int Ammo::GetRoomNumber()
{
	return room_number;
}

/* ---------- Setters ---------- */

void Ammo::SetX(int lx)
{
	x = lx;
}

void Ammo::SetY(int ly)
{
	y = ly;
}

void Ammo::SetRoomNumber(int room_number_location)
{
	room_number = room_number_location;
}
