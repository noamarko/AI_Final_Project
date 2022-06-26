#pragma once

const int HEALTH_POINTS_STOCK = 50;
//const int NUM_OF_HEALTH_POINTS = 2;

class HealthPoints
{
private:
	int x;
	int y;
	int room_number;

public:
	HealthPoints();
	HealthPoints(int lx, int ly, int room_number_location);

	int GetX();
	int GetY();
	int GetRoomNumber();

	void SetX(int lx);
	void SetY(int ly);
	void SetRoomNumber(int room_number_location);

};