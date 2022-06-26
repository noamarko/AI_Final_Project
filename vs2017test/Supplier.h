#pragma once 

#include "Ammo.h"
#include "HealthPoints.h"
#include "Grenade.h"
#include "Bullet.h"
#include "Node.h"
#include "Soldier.h"
#include <vector>
#include "MazeHandler.h"

using namespace std;

const int MAX_BULLETS = 40;
const int START_GRENADES = 10;
const int SUPPLIER_START_HEALTH = 200;
const int SUPPLIER_PATH_INDEX = NUM_ROOMS + 10;


class Supplier
{
private:
	bool isInRoom;
	bool onMission;
	int locationX;
	int locationY;
	int grenade_ammo;
	int bullets_ammo;
	int health_ammo;
	int room_index;
	int color;

	Soldier* pTeam;
	Node* start, * target;
	vector<Node*> path;

	Bullet* pb;
	Grenade* pg;

public:
	Supplier();
	Supplier(int x, int y, int room, int team_color);

	int GetLocationX();
	int GetLocationY();
	int GetBulletsAmmo();
	int GetGrenadeAmmo();
	int GetColor();
	int GetRoomIndex();
	int GetHealthAmmo();


	bool isSecure(MazeHandler* mazeHandler, Soldier opponent);

	void SetColor(int team_color);
	void SetRoomIndex(int index);
	void SetLocationX(int lx);
	void SetLocationY(int ly);
	void SetHealth(int health);
	void UpdateHealth(int health_points);
	void UpdateBulletsAmmo(int num_of_bullets);
	void UpdateGrenadeAmmo(int num_of_grenades);
	void UpdateHealthAmmo(int num_of_health);
	
	void FindTargetPoint(int x1, int y1, int x2, int y2);
	void SetSecurePoint(MazeHandler* mazeHandler, Soldier opponent, Room room);
	void SetPath(Node* current_node);
	void EmptyPath();
	void Move(MazeHandler* mazeHandler, int type_of_points);
	void WalkInPath(MazeHandler* mazeHandler, int minX, int minY, int maxX, int maxY);
	void TargetAchieved(int type_of_points);
	int CalculateRoomIndex(MazeHandler* mazeHandler);
	void SetBulletDetails(Soldier opponent_team[NUM_SOLDIERS]);
	void SetGrenadeDetails(Soldier opponent_team[NUM_SOLDIERS], MazeHandler* mazeHandler);

	void AddNextStep(Node* pCurrent, DirectionType directionType, priority_queue <Node*, vector<Node*>, CompareNodes>& pq,
		vector <Node>& gray, vector <Node>& black, MazeHandler* mazeHandler);
	void FindPath(MazeHandler* mazeHandler);
	void SearchForAmmo(Soldier* pSoldier, MazeHandler* mazeHandler, Ammo* ammo1, Ammo* ammo2);
	void SearchForHealth(Soldier* pSoldier,MazeHandler* mazeHandler, HealthPoints* hp1, HealthPoints* hp2);

	void SearchForCover(MazeHandler* mazeHandler, Soldier opponent);
	void MoveToRandomRoom(MazeHandler* mazeHandler);
	void SearchForEnemy(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS], bool* withEnemy);
};
