#pragma once

//const int NUM_OF_AMMO_POINTS = 2;
const int BULLETS_STOCK = 5;
const int GRENADE_STOCK = 2;

class Ammo
{
private:
	int x;
	int y;
	int room_number;

public:
	Ammo();
	Ammo(int lx, int ly, int lroom_number);

	int GetX();
	int GetY();
	int GetRoomNumber();

	void SetX(int lx);
	void SetY(int ly);
	void SetRoomNumber(int room_number_location);
};