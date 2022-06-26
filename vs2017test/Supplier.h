#pragma once 

#include "Ammo.h"
#include "HealthPoints.h"
#include "Grenade.h"
#include "Bullet.h"
#include "Node.h"
#include "Soldier.h"
#include <vector>
#include "MazeController.h"

using namespace std;

const int MAX_BULLETS = 40;
const int START_GRENADES = 10;
const int START_HEALTH = 200;
const int PATH_INDEX = NUM_ROOMS + 10;


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


	bool isSecure(MazeController* mazeController, Soldier opponent);

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
	void SetSecurePoint(MazeController* mazeController, Soldier opponent, Room room);
	void SetPath(Node* current_node);
	void EmptyPath();
	void Move(MazeController* mazeController, int type_of_points);
	void WalkInPath(MazeController* mazeController, int minX, int minY, int maxX, int maxY);
	void TargetAchieved(int type_of_points);
	int CalculateRoomIndex(MazeController* mazeController);
	void SetBulletDetails(Soldier opponent_team[NUM_SOLDIERS]);
	void SetGrenadeDetails(Soldier opponent_team[NUM_SOLDIERS], MazeController* mazeController);

	void AddNextStep(Node* pCurrent, DirectionType directionType, priority_queue <Node*, vector<Node*>, CompareNodes>& pq,
		vector <Node>& gray, vector <Node>& black, MazeController* mazeController);
	void FindPath(MazeController* mazeController);
	void SearchForAmmo(Soldier* pSoldier, MazeController* mazeController, Ammo* ammo1, Ammo* ammo2);
	void SearchForHealth(Soldier* pSoldier,MazeController* mazeController, HealthPoints* hp1, HealthPoints* hp2);

	void SearchForCover(MazeController* mazeController, Soldier opponent);
	void MoveToRandomRoom(MazeController* mazeController);
	void SearchForEnemy(MazeController* mazeController, Soldier opponent_team[NUM_SOLDIERS], bool* withEnemy);
};
