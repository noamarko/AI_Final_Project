#pragma once
#include "Room.h"
#include "glut.h"
#include <time.h>
#include "Grenade.h"
#include "CompareNodes.h"
#include <queue>
#include <vector>
#include <iostream>
#include "Ammo.h"
#include "HealthPoints.h"

//#include "Soldier.h"

using namespace std;

const int INFINITY_VALUE = 10000;
const double SPACE_COST = 0.05;
const double WALL_COST = 2;
enum DirectionType { TOP = 1, BOTTOM = 2, LEFT = 3, RIGHT = 4 };


class MazeHandler
{
public:
	int maze[MSZ][MSZ];
	double security_map[MSZ][MSZ] = { 0 };
	Room rooms[NUM_ROOMS];

	MazeHandler();

	void init();
	void UpdateSecurityMap();
	double Distance(int x1, int y1, int x2, int y2);
	void UpdatePQ(priority_queue <Node*, vector<Node*>, CompareNodes>& pq, Node* pn);
	void AddNeighbor(Node* pCurrent, int direction,
		priority_queue <Node*, vector<Node*>, CompareNodes>& pq, vector <Node>& gray, vector <Node>& black,
		int target_index);
	void RestorePath(Node* pCurrent);
	void DigPath(int index1, int index2);
	void DigPathes();
	void SetupMaze();
	void DrawMaze();
	void BuildPath(Node* pCurrent);
	void AddObstacles();
	void AddAmmoStock(Ammo* ammo1, Ammo* ammo2);
	void GenerateAmmoLocation(Ammo* ammo);
	void AddHealthPoints(HealthPoints* hp1, HealthPoints* hp2);
	void GenerateHealthPointsLocation(HealthPoints* hp);
	void RemoveAmmoPoint(Ammo* ammo1, Ammo* ammo2, int row, int col);
	void RemoveHealthPoint(HealthPoints* hp1, HealthPoints* hp2, int row, int col);
};
