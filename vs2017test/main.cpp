#include "main.h"

using namespace std;

Ammo* ammo1, * ammo2;
HealthPoints* hp1, * hp2;
Soldier cyan_team[NUM_SOLDIERS];
Soldier olive_team[NUM_SOLDIERS];

Details* pd = nullptr;
MazeHandler* mazeHandler = nullptr;

bool isPlaying = false;

/* ---------- Build The Map ---------- */

void init()
{
	mazeHandler = new MazeHandler();
	ammo1 = new Ammo();
	ammo2 = new Ammo();
	mazeHandler->AddAmmoStock(ammo1, ammo2);
	hp1 = new HealthPoints();
	hp2 = new HealthPoints();
	mazeHandler->AddHealthPoints(hp1, hp2);
	GenerateTeams();
}

/* ---------- Game Methods ---------- */

/* Generating team locations to game map */

void GenerateTeams()
{
	int i, team1_room_index, team2_room_index;
	team1_room_index = rand() % NUM_ROOMS;
	CalaculateTeamPosition(cyan_team, team1_room_index, CYAN_SOLDIER);

	//Initialize teams in different rooms
	do
	{
		team2_room_index = rand() % NUM_ROOMS;
	} while (team2_room_index == team1_room_index);

	CalaculateTeamPosition(olive_team, team2_room_index, OLIVE_SOLDIER);
}

void CalaculateTeamPosition(Soldier team[NUM_SOLDIERS], int roomNumber, int team_color)
{
	Soldier* pSoldier;
	int randomX, randomY;
	for (int i = 0; i < NUM_SOLDIERS; i++)
	{
		pSoldier = &team[i];
		/* Generate random location in the room until it is space */
		do
		{
			randomX = rand() % mazeHandler->rooms[roomNumber].GetWidth() +
				(mazeHandler->rooms[roomNumber].GetCenterX() - mazeHandler->rooms[roomNumber].GetWidth() / 2);
			randomY = rand() % mazeHandler->rooms[roomNumber].GetHeight() +
				(mazeHandler->rooms[roomNumber].GetCenterY() - mazeHandler->rooms[roomNumber].GetHeight() / 2);
			pSoldier->SetLocationX(randomX);
			pSoldier->SetLocationY(randomY);
		} while (mazeHandler->maze[pSoldier->GetLocationY()][pSoldier->GetLocationX()] != SPACE);
		pSoldier->SetColor(team_color);
		pSoldier->SetRoomIndex(roomNumber);
		mazeHandler->maze[pSoldier->GetLocationY()][pSoldier->GetLocationX()] = pSoldier->GetColor();
	}
}

/* Paint Dead soldiers as SPACE and remove them from room */
void RemoveDeadSoldiers()
{
	int i;
	for (i = 0; i < NUM_SOLDIERS; i++)
	{
		if (cyan_team[i].GetHealth() <= 0)
		{
			mazeHandler->maze[cyan_team[i].GetLocationY()][cyan_team[i].GetLocationX()] = SPACE;
			cyan_team[i].SetRoomIndex(PATH_INDEX);
		}

		if (olive_team[i].GetHealth() <= 0)
		{
			mazeHandler->maze[olive_team[i].GetLocationY()][olive_team[i].GetLocationX()] = SPACE;
			olive_team[i].SetRoomIndex(PATH_INDEX);
		}
	}
}

void PlayTurn(Soldier* pSoldier)
{
	int i;
	bool withEnemy = false;
	bool supplierOnMission = false;

	if (pSoldier->GetHealth() <= 0)
		return;

	if (pSoldier->GetBulletsStack() == 0) {
		if (pSoldier->GetColor() == CYAN_SOLDIER)
			pSoldier->HideFromEnemy(mazeHandler, olive_team, &withEnemy);
		else
			pSoldier->HideFromEnemy(mazeHandler, cyan_team, &withEnemy);
		if (pSoldier->GetBulletsAmmo() > 0)
			pSoldier->Reload();
			
		if (0 < pSoldier->GetBulletsAmmo() < 3 ) { // && !supplier->OnMission()
			//supplierGiveAmmo(pSoldier.GetLocationX(), pSoldier.GetLocationY(), &supplierOnMission)
		}
	}

	//if ((pSoldier->GetBulletsAmmo() < BULLET_SHORTAGE || pSoldier->GetGrenadeAmmo() < GRENADE_SHORTAGE)
		//&& (ammo1->GetX() != INFINITY_VALUE || ammo2->GetX() != INFINITY_VALUE))
		//pSoldier->SearchForAmmo(mazeHandler, ammo1, ammo2);
		
	else if (pSoldier->GetHealth() < HEALTH_SHORTAGE && (hp1->GetX() != INFINITY_VALUE || hp2->GetX() != INFINITY_VALUE))
		pSoldier->SearchForHealth(mazeHandler, hp1, hp2);
	else
	{
		if (pSoldier->GetColor() == CYAN_SOLDIER)
			pSoldier->SearchForEnemy(mazeHandler, olive_team, &withEnemy);
		else
			pSoldier->SearchForEnemy(mazeHandler, cyan_team, &withEnemy);
		
		if (!withEnemy)
			pSoldier->MoveToRandomRoom(mazeHandler);
	}
}

void CheckIfFinished(bool cyan_alive, bool olive_alive)
{
	if (!cyan_alive && olive_alive)
	{
		cout << "Olive Team Won!\n";
		isPlaying = false;
	}
	else if (cyan_alive && !olive_alive)
	{
		cout << "Cyan Team Won!\n";
		isPlaying = false;
	}
	else if (!cyan_alive && !olive_alive)
	{
		cout << "It's a Tie!";
		isPlaying = false;
	}
}

void Play()
{
	Sleep(200);
	int i;
	bool cyan_alive = false, olive_alive = false;
	for (i = 0; i < NUM_SOLDIERS; i++)
	{
		PlayTurn(&cyan_team[i]);
		PlayTurn(&olive_team[i]);
	}

	RemoveDeadSoldiers();

	if (isPlaying)
	{
		for (i = 0; i < NUM_SOLDIERS; i++)
		{
			if (cyan_team[i].GetHealth() > 0)
				cyan_alive = true;
			if (olive_team[i].GetHealth() > 0)
				olive_alive = true;
		}
		CheckIfFinished(cyan_alive, olive_alive);		
	}

	if (!isPlaying)
		glutPostRedisplay(); // call to display again
}

/* ---------- General Functions ---------- */

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	mazeHandler->DrawMaze();

	if (isPlaying)
		Play();

	if (pd != nullptr)
		pd->Draw();

	glutSwapBuffers(); // show all
}

void idle()
{
	if (pd != nullptr)
		pd->Draw();

	glutPostRedisplay(); // call to display again
}

void menu(int choice)
{
	if (choice == 1) // Start Game
		isPlaying = true;
}

void mouse(int button, int state, int x, int y)
{
	double bx, by;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		bx = (double)2 * x / (double)W - 1; // now bx is between -1 and 1
		by = (double)2 * ((double)H - y) / (double)H - 1;

		pd = new Details(bx, by, cyan_team, olive_team);
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("AI Final Project");
	glutDisplayFunc(display); // refresh window function
	glutIdleFunc(idle); // kind of timer function
	glutMouseFunc(mouse);
	glutCreateMenu(menu);
	glutAddMenuEntry("Start Game", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	init();
	glutMainLoop();
}