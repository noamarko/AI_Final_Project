#include "MazeHandler.h"

/* ---------- Constructor ---------- */

MazeHandler::MazeHandler()
{
	init();
}

/* ---------- Build Map Functions ---------- */

/* Create Game Map */
void MazeHandler::init()
{
	//    RED, GREEN, BLUE
	glClearColor(0.8, 0, 0, 0);// color of window background

	srand(time(0));
	// define main axes
	glOrtho(-1, 1, -1, 1, -1, 1);

	SetupMaze();
}

void MazeHandler::UpdateSecurityMap()
{
	int num_grenades = 30000;
	int i;
	double x, y;

	for (i = 0; i < num_grenades; i++)
	{
		x = 2 * (double)(rand() % MSZ) / (double)MSZ - 1;
		y = 2 * (double)(rand() % MSZ) / (double)MSZ - 1;

		Grenade* p = new Grenade(x, y);

		p->UpdateSecurityMap(maze, security_map);
	}
}

/* Calculate heuristic distance */
double MazeHandler::Distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow((x1 - x2), 2) + pow(y1 - y2, 2));
}

/* Updating priority queue to A* algorithm */
void MazeHandler::UpdatePQ(priority_queue<Node*, vector<Node*>, CompareNodes>& pq, Node* pn)
{
	bool found = false;
	Node* ptmp;
	vector <Node*> tmp_v;

	while (!pq.empty() && !found)
	{
		ptmp = pq.top();
		pq.pop();
		if ((*ptmp).operator==(*pn))
		{
			found = true;
			pq.push(pn);
		}
		else
			tmp_v.push_back(ptmp);
	}

	while (!tmp_v.empty())
	{
		ptmp = tmp_v.back();
		tmp_v.pop_back();
		pq.push(ptmp);
	}
}

/* Adding neighbors to the next step of A* algorithm */
void MazeHandler::AddNeighbor(Node* pCurrent, int direction, priority_queue<Node*, vector<Node*>, CompareNodes>& pq, vector<Node>& gray, vector<Node>& black, int target_index)
{
	int i = 0, j = 0;
	Node* pn;
	double g, h;
	vector<Node>::iterator it_gray;
	vector<Node>::iterator it_black;

	switch (direction)
	{
	case TOP:
		i = pCurrent->GetRow() + 1;
		j = pCurrent->GetCol();
		break;
	case BOTTOM:
		i = pCurrent->GetRow() - 1;
		j = pCurrent->GetCol();
		break;
	case LEFT:
		i = pCurrent->GetRow();
		j = pCurrent->GetCol() - 1;
		break;
	case RIGHT:
		i = pCurrent->GetRow();
		j = pCurrent->GetCol() + 1;
		break;
	}
	h = Distance(j, i, rooms[target_index].GetCenterX(), rooms[target_index].GetCenterY());
	if (maze[i][j] == WALL)
		g = pCurrent->GetG() + WALL_COST;
	else // SPACE or TARGET
		g = pCurrent->GetG() + SPACE_COST;

	pn = new Node(i, j, pCurrent, g, h);
	// Let's check the color of neighbor
	it_black = find(black.begin(), black.end(), *pn); // Now we see that we really need operator ==
	if (it_black != black.end()) // it is black => we don't add it to pq 
	{
		delete pn;
		return;
	}
	else // it is not black 
	{
		it_gray = find(gray.begin(), gray.end(), *pn);
		if (it_gray == gray.end()) // it is not gray => it is white so add it to pq and paint it gray
		{
			pq.push(pn);
			gray.push_back(*pn);
		}
		else // it is gray => check if its F is better (lower) then of its copy from pq
		{
			if (it_gray->GetF() < pn->GetF()) // pn is not better then its copy from pq
			{
				delete pn;
				return;
			}
			else // pn is BETTER then its copy from pq
			{
				UpdatePQ(pq, pn);
				gray.erase(it_gray);
				gray.push_back(*pn);
			}
		}
	}
}

/* Restores the path to target node */
void MazeHandler::RestorePath(Node* pCurrent)
{
	while (maze[pCurrent->GetRow()][pCurrent->GetCol()] != START)
	{
		maze[pCurrent->GetRow()][pCurrent->GetCol()] = SPACE;
		pCurrent = pCurrent->GetParent();
	}
	maze[pCurrent->GetRow()][pCurrent->GetCol()] = SPACE;
}

// runs A* for building a path from rooms[index1] to rooms[index2]
void MazeHandler::DigPath(int index1, int index2)
{
	vector <Node> gray;
	vector <Node> black;
	priority_queue <Node*, vector<Node*>, CompareNodes> pq;
	vector<Node>::iterator it_gray;
	vector<Node>::iterator it_black;

	int is, js, it, jt;
	Node* pn;
	Node* pCurrent;
	is = rooms[index1].GetCenterY();
	js = rooms[index1].GetCenterX();
	it = rooms[index2].GetCenterY();
	jt = rooms[index2].GetCenterX();
	// 1. set center of rooms[index1] as START
	maze[is][js] = START;
	// 2. set center of rooms[index2] as TARGET
	maze[it][jt] = TARGET;
	// 3. add start to PQ and to Grays 
	pn = new Node(is, js, nullptr, 0, Distance(js, is, jt, it));
	pq.push(pn);
	gray.push_back(*pn);
	// 4. while PQ is not empty
	while (!pq.empty())
	{
		// 4.1 pick the "BEST"( F = G+H ) Node from PQ
		pCurrent = pq.top();
		pq.pop(); // and remove it from pq
		// 4.2 if it is TARGET, the best path has been found
		if (maze[pCurrent->GetRow()][pCurrent->GetCol()] == TARGET)
		{
			//			maze[current.GetRow()][current.GetCol()] = SPACE;
						// 4.2.1 Restore path by combining digging (through walls) and moving (through spaces)
			RestorePath(pCurrent);
			return; // 
		}
		else // pCurrent is not TARGET
		{
			// paint pCurrent black
			black.push_back(*pCurrent);
			// remove pCurrent from gray
			it_gray = find(gray.begin(), gray.end(), *pCurrent); // based on operator == if we compare objects
			if (it_gray != gray.end()) // if it was found
				gray.erase(it_gray);
			// 4.3 Add neighbors of pCurrent to PQ and return to 4.
				// check top neighbor
			if (pCurrent->GetRow() + 1 < MSZ)
				AddNeighbor(pCurrent, TOP, pq, gray, black, index2);
			// check bottom neighbor
			if (pCurrent->GetRow() - 1 >= 0)
				AddNeighbor(pCurrent, BOTTOM, pq, gray, black, index2);
			// check left neighbor
			if (pCurrent->GetCol() - 1 >= 0)
				AddNeighbor(pCurrent, LEFT, pq, gray, black, index2);
			// check right neighbor
			if (pCurrent->GetCol() + 1 < MSZ)
				AddNeighbor(pCurrent, RIGHT, pq, gray, black, index2);
		}// else
	}// while
	cout << "ERROR: Priority queue is empty. The target was missed\n";

}

void MazeHandler::DigPathes()
{
	int i, j;

	for (i = 0; i < NUM_ROOMS; i++)
		for (j = i + 1; j < NUM_ROOMS; j++)
			DigPath(i, j);
}

void MazeHandler::SetupMaze()
{
	int i, j, k;
	int countRoom;
	int w, h, cx, cy;
	// set borders

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
			maze[i][j] = WALL;

	for (countRoom = 0; countRoom < NUM_ROOMS; countRoom++)
	{
		bool hasOverlap;
		do
		{
			hasOverlap = false;

			// pick random room parameters
			w = 7 + rand() % (MSZ / 5);
			h = 7 + rand() % (MSZ / 5);
			cx = 1 + w / 2 + rand() % (MSZ - w - 2);
			cy = 1 + h / 2 + rand() % (MSZ - h - 2);

			// check overlapping
			for (k = 0; k < countRoom && !hasOverlap; k++)
				if (rooms[k].Overlap(w, h, cx, cy))
					hasOverlap = true;
		} while (hasOverlap);
		// setup room
		rooms[countRoom].SetWidth(w);
		rooms[countRoom].SetHeight(h);
		rooms[countRoom].SetCenterX(cx);
		rooms[countRoom].SetCenterY(cy);
		rooms[countRoom].Init(maze);
	}

	// now dig the pathes between each room
	DigPathes();

	//Add Obstacles to the map
	AddObstacles();

	//Update Security Map
	UpdateSecurityMap();
}

void MazeHandler::DrawMaze()
{
	int i, j;
	double dx, dy, x, y;

	dx = dy = 2.0 / MSZ; // 2.0 is the logical width and height of the window

	y = -1;
	for (i = 0; i < MSZ; i++, y += dy)
	{
		x = -1;
		for (j = 0; j < MSZ; j++, x += dx)
		{
			switch (maze[i][j]) {
			case WALL:
				glColor3d(0.2, 0.2, 0.2);
				break;
			case SPACE:
				glColor3d(1 - security_map[i][j], 1 - security_map[i][j], 1 - security_map[i][j]);
				break;
			case SECURE_POINT:
				glColor3d(1 - security_map[i][j], 1 - security_map[i][j], 1 - security_map[i][j]);
				break;
			case START:
				glColor3d(0.2, 0.4, 1);
				break;
			case TARGET:
				glColor3d(1, 0, 0);
				break;
			case BLACK_SPACE:
				glColor3d(1 - security_map[i][j], 1 - security_map[i][j], 1 - security_map[i][j]);;
				break;
			case GRAY:
				glColor3d(0, 0.8, 0);
				break;
			case PATH:
				glColor3d(1, 0.5, 1);
				break;
			case AMMO_STOCK:
				glColor3d(0.63, 0.32, 0.18);
				break;
			case HEALT_POINTS:
				glColor3d(0.2, 1, 0.2);
				break;
			case CYAN_SOLDIER:
				glColor3d(0, 1, 1);
				break;
			case OLIVE_SOLDIER:
				glColor3d(0.5, 0.5, 0);
				break;

			}// switch
			// draw one square in maze
			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x, y + dy);
			glVertex2d(x + dx, y + dy);
			glVertex2d(x + dx, y);
			glEnd();
		}
	}
}

void MazeHandler::BuildPath(Node* pCurrent)
{
	while (maze[pCurrent->GetRow()][pCurrent->GetCol()] != START)
	{
		maze[pCurrent->GetRow()][pCurrent->GetCol()] = PATH;
		pCurrent = pCurrent->GetParent();
	}
}

/* Adding obstacles to the game map */
void MazeHandler::AddObstacles()
{
	const int up = 0, down = 1, right = 2, left = 3;
	int obstacle_size, num_obstacles, currentX, currentY, minX, minY;
	int side, i, j, k;
	for (i = 0; i < NUM_ROOMS; i++)
	{
		num_obstacles = rand() % 2 + 1;
		for (k = 0; k < num_obstacles; k++)
		{
			j = 0;
			obstacle_size = rand() % 8 + 2; // Obstacle size between 2 - 8
			minX = rooms[i].GetCenterX() - rooms[i].GetWidth() / 2;
			minY = rooms[i].GetCenterY() - rooms[i].GetHeight() / 2;
			/* Generate random locations of obstacle, we don't want it to block the paths*/
			currentX = rand() % (rooms[i].GetWidth() - 4) + (minX + 1);
			currentY = rand() % (rooms[i].GetHeight() - 4) + (minY + 1);
			while (j < obstacle_size)
			{
				maze[currentY][currentX] = WALL;
				rooms[i].AddObstacleNode(currentX, currentY);
				side = rand() % 4;
				if (side == up && currentY + 1 < minY + rooms[i].GetHeight())
					currentY += 1;
				else if (side == down && currentY - 1 > minY)
					currentY -= 1;
				else if (side == right && currentX + 1 < minX + rooms[i].GetWidth())
					currentX += 1;
				else if (side == left && currentX - 1 < minX)
					currentX -= 1;
				j++;
			}
		}
	}
}

/* Adding ammo points locations to game map */
void MazeHandler::AddAmmoStock(Ammo* ammo1, Ammo* ammo2)
{
	ammo1->SetRoomNumber(rand() % NUM_ROOMS);
	GenerateAmmoLocation(ammo1);
	// to create ammo in different rooms
	do
	{
		ammo2->SetRoomNumber(rand() % NUM_ROOMS);
	} while (ammo1->GetRoomNumber() == ammo2->GetRoomNumber());

	GenerateAmmoLocation(ammo2);

	maze[ammo1->GetY()][ammo1->GetX()] = AMMO_STOCK;
	maze[ammo2->GetY()][ammo2->GetX()] = AMMO_STOCK;
}

/* Generating ammo locations in the rooms */
void MazeHandler::GenerateAmmoLocation(Ammo* ammo)
{
	do {
		ammo->SetX(rand() % rooms[ammo->GetRoomNumber()].GetWidth() +
			(rooms[ammo->GetRoomNumber()].GetCenterX() - rooms[ammo->GetRoomNumber()].GetWidth() / 2));
		ammo->SetY(rand() % rooms[ammo->GetRoomNumber()].GetHeight() +
			(rooms[ammo->GetRoomNumber()].GetCenterY() - rooms[ammo->GetRoomNumber()].GetHeight() / 2));
	} while (maze[ammo->GetY()][ammo->GetX()] != SPACE);
}

/* Adding health points locations to game map */
void MazeHandler::AddHealthPoints(HealthPoints* hp1, HealthPoints* hp2)
{
	hp1->SetRoomNumber(rand() % NUM_ROOMS);
	GenerateHealthPointsLocation(hp1);
	// to create health points in different rooms
	do
	{
		hp2->SetRoomNumber(rand() % NUM_ROOMS);
	} while (hp1->GetRoomNumber() == hp2->GetRoomNumber());

	GenerateHealthPointsLocation(hp2);

	maze[hp1->GetY()][hp1->GetX()] = HEALT_POINTS;
	maze[hp2->GetY()][hp2->GetX()] = HEALT_POINTS;
}

/* Generating health points locations in the rooms */
void MazeHandler::GenerateHealthPointsLocation(HealthPoints* hp)
{
	do {
		hp->SetX(rand() % rooms[hp->GetRoomNumber()].GetWidth() +
			(rooms[hp->GetRoomNumber()].GetCenterX() - rooms[hp->GetRoomNumber()].GetWidth() / 2));
		hp->SetY(rand() % rooms[hp->GetRoomNumber()].GetHeight() +
			(rooms[hp->GetRoomNumber()].GetCenterY() - rooms[hp->GetRoomNumber()].GetHeight() / 2));
	} while (maze[hp->GetY()][hp->GetX()] != SPACE);
}

/* Remove ammo point from map in case one of the soldier took it */
void MazeHandler::RemoveAmmoPoint(Ammo* ammo1, Ammo* ammo2, int row, int col)
{
	if (ammo1->GetY() == row && ammo1->GetX() == col)
	{
		ammo1->SetX(INFINITY_VALUE);
		ammo1->SetY(INFINITY_VALUE);
	}
	else
	{
		ammo2->SetX(INFINITY_VALUE);
		ammo2->SetY(INFINITY_VALUE);
	}
}

/* Remove health point from map in case one of the soldier took it */
void MazeHandler::RemoveHealthPoint(HealthPoints* hp1, HealthPoints* hp2, int row, int col)
{
	if (hp1->GetY() == row && hp1->GetX() == col)
	{
		hp1->SetX(INFINITY_VALUE);
		hp1->SetY(INFINITY_VALUE);
	}
	else
	{
		hp2->SetX(INFINITY_VALUE);
		hp2->SetY(INFINITY_VALUE);
	}
}
