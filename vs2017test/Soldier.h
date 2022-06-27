#pragma once 

#include "Ammo.h"
#include "HealthPoints.h"
#include "Grenade.h"
#include "Bullet.h"
#include "Node.h"
#include <vector>
#include "MazeHandler.h"

using namespace std;

const int START_STACKS_AMMO = 5;
const int BULLETS_PER_STACK = 5;
const int START_GRENADES_AMMO = 5;
const int START_HEALTH = 1000;
const int NUM_SOLDIERS = 2;
const int PATH_INDEX = NUM_ROOMS + 10;


class Soldier
{	
private:
	bool isInRoom;
	int locationX;
	int locationY;
	int health;
	int total_bullets_ammo;
	int bullets_stack_ammo;
	int stack_grenade;
	int grenade_ammo;
	int room_index;
	int color;

	Node *start, *target;
	vector<Node*> path;

	Bullet* pb;
	Grenade* pg;

public:
	Soldier();
	Soldier(int x, int y, int room, int team_color);

	int GetLocationX();
	int GetLocationY();
	int GetHealth();
	int GetBulletsAmmo();
	int GetBulletsStack();
	int GetGrenadeAmmo();
	int GetGrenadeStack();
	int GetColor();
	int GetRoomIndex();
	bool isSecure(MazeHandler* mazeHandler, Soldier opponent);

	void SetColor(int team_color);
	void SetRoomIndex(int index);
	void SetLocationX(int lx);
	void SetLocationY(int ly);
	void SetHealth(int health);
	void UpdateHealth(int health_points);
	void UpdateBulletsAmmo(int num_of_bullets);
	void UpdateGrenadeAmmo(int num_of_grenades);

	void Reload();
	void FindTargetPoint(int x1, int y1, int x2, int y2);
	void SetSecurePoint(MazeHandler* mazeHandler, Soldier opponent, Room room);
	void SetPath(Node* current_node);
	void EmptyPath();
	void Move(MazeHandler* mazeHandler, int type_of_points);
	void WalkInPath(MazeHandler* mazeHandler, int minX, int minY, int maxX, int maxY);
	void TargetAchieved(int type_of_points);
	int CalculateRoomIndex(MazeHandler* mazeHandler);
	void Shoot(Soldier opponent_team[NUM_SOLDIERS], MazeHandler* mazeHandler);
	void SetBulletDetails(Soldier opponent_team[NUM_SOLDIERS]);
	void ShotMovement(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS]);
	void ThrowGrenade(Soldier opponent_team[NUM_SOLDIERS], MazeHandler* mazeHandler);
	void SetGrenadeDetails(Soldier opponent_team[NUM_SOLDIERS], MazeHandler* mazeHandler);
	void GrenadeMovement(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS]);
	void GrenadeHit(Soldier opponent_team[NUM_SOLDIERS], int row, int col, Bullet* pBullet);
	void AddNextStep(Node* pCurrent, DirectionType directionType, priority_queue <Node*, vector<Node*>, CompareNodes>& pq,
		vector <Node>& gray, vector <Node>& black, MazeHandler* mazeHandler);
	void FindPath(MazeHandler* mazeHandler);
	void SearchForAmmo(MazeHandler* mazeHandler, Ammo* ammo1, Ammo* ammo2);
	void SearchForHealth(MazeHandler* mazeHandler, HealthPoints* hp1, HealthPoints* hp2);
	void Attack(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS], Soldier opponent);
	void SearchForCover(MazeHandler* mazeHandler, Soldier opponent);
	void MoveToRandomRoom(MazeHandler* mazeHandler);
	void SearchForEnemy(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS], bool* withEnemy);
	void HideFromEnemy(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS], bool* withEnemy);
};