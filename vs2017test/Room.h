#pragma once
#include <vector>
#include "Node.h"

using namespace std;

const int MSZ = 100;
const int WALL = 1;
const int SPACE = 2;
const int START = 3;
const int PATH = 5;
const int GRAY = 6;
const int BLACK_SPACE = 7;
const int TARGET = 8;
const int AMMO_STOCK = 9;
const int HEALT_POINTS = 10;
const int CYAN_SOLDIER = 11;
const int OLIVE_SOLDIER = 12;
const int SECURE_POINT = 13;
const int MOVEMENT = 14;

const int NUM_ROOMS = 12;


class Room
{
private:
	int cx, cy, width, height;
	vector<Node*> secure_points;
	vector<Node*> obstacle_nodes;
public:
	Room();
	Room(int row, int col, int w, int h);

	int GetCenterX();
	int GetCenterY();
	int GetWidth();
	int GetHeight();
	vector<Node*> GetSecurePoints();
	vector<Node*> GetObstacles();

	void SetCenterX(int col);
	void SetCenterY(int row);
	void SetWidth(int w);
	void SetHeight(int h);

	void AddObstacleNode(int x, int y);
	void Init(int maze[MSZ][MSZ]);
	bool Overlap(int w, int h, int cx, int cy);
};

