#pragma once
#include "glut.h"
#include <time.h>
#include <vector>
#include <iostream>
#include <windows.h>
#include "Node.h"
#include "Room.h"
#include <queue>
#include "CompareNodes.h"
#include "Bullet.h"
#include <math.h>
#include "Grenade.h"
#include "Ammo.h"
#include "HealthPoints.h"
#include "Soldier.h"
#include "Details.h"
#include "MazeHandler.h"


const int W = 600; // window width
const int H = 600; // window height
const int BULLET_SHORTAGE = 5;
const int GRENADE_SHORTAGE = 2;
const int HEALTH_SHORTAGE = 50;

void init();
void GenerateTeams();
void CalaculateTeamPosition(Soldier team[NUM_SOLDIERS], int roomNumber, int team_color);
void RemoveDeadSoldiers();
void SearchForEnemy(Soldier* pSoldier, bool* withEnemy);
void PlayTurn(Soldier* pSoldier);
void CheckIfFinished(bool cyan_alive, bool olive_alive);
void Play();
void display();
void idle();
void menu(int choice);
void mouse(int button, int state, int x, int y);