#include "Supplier.h"
#include <stdlib.h>
#include "Room.h"
#include "glut.h"

/* ---------- Constructors ---------- */

Supplier::Supplier()
{
    locationX = 0;
    locationY = 0;
    grenade_ammo = 0;
    bullets_ammo = 0;
    health_ammo = 0;
    isInRoom = true;
    onMission = false;
    room_index = 0;
    color = SPACE;
    start = nullptr;
    target = nullptr;
    pb = nullptr;
    pg = nullptr;
}

Supplier::Supplier(int x, int y, int room, int team_color)
{
    locationX = x;
    locationY = y;
    grenade_ammo = 0;
    bullets_ammo= 0;
    health_ammo = 0;
    isInRoom = true;
    onMission = false;
    room_index = room;
    color = team_color;
    start = nullptr;
    target = nullptr;
    pb = nullptr;
    pg = nullptr;
}

/* ---------- Getters ---------- */

int Supplier::GetLocationX()
{
    return locationX;
}

int Supplier::GetLocationY()
{
    return locationY;
}


int Supplier::GetBulletsAmmo()
{
    return bullets_ammo;
}

int Supplier::GetGrenadeAmmo()
{
    return grenade_ammo;
}

int Supplier::GetHealthAmmo()
{
    return health_ammo;
}

int Supplier::GetColor()
{
    return color;
}

int Supplier::GetRoomIndex()
{
    return room_index;
}

/* Check if soldier is behind a shelter */
bool Soldier::isSecure(MazeController* mazeController, Soldier opponent)
{
    int minX, minY, maxX, maxY;
    minX = mazeController->rooms[opponent.GetRoomIndex()].GetCenterX() - mazeController->rooms[opponent.GetRoomIndex()].GetWidth() / 2;
    minY = mazeController->rooms[opponent.GetRoomIndex()].GetCenterY() - mazeController->rooms[opponent.GetRoomIndex()].GetHeight() / 2;
    maxX = minX + mazeController->rooms[opponent.GetRoomIndex()].GetWidth();
    maxY = minY + mazeController->rooms[opponent.GetRoomIndex()].GetHeight();

    // Check if a soldier is behind a wall and the wall is not an external point from the room
    if ((mazeController->maze[locationY + 1][locationX] == WALL && locationY + 1 < maxY) ||
        (mazeController->maze[locationY - 1][locationX] == WALL && locationY - 1 > minY) ||
        (mazeController->maze[locationY][locationX + 1] == WALL && locationX + 1 < maxX) ||
        (mazeController->maze[locationY][locationX - 1] == WALL && locationX - 1 > minX))
        return true;
    return false;

}

/* ---------- Setters ---------- */

void Supplier::SetColor(int team_color)
{
    color = team_color;
}

void Supplier::SetRoomIndex(int index)
{
    room_index = index;
}

void Supplier::SetLocationX(int lx)
{
    locationX = lx;
}

void Supplier::SetLocationY(int ly)
{
    locationY = ly;
}


void Supplier::UpdateBulletsAmmo(int num_of_bullets)
{
    bullets_ammo += num_of_bullets;
}

void Supplier::UpdateGrenadeAmmo(int num_of_grenades)
{
    grenade_ammo += num_of_grenades;
}

void Supplier::UpdateHealthAmmo(int num_of_health)
{
    health_ammo += num_of_health;
}

/* ---------- Game Functions ---------- */

/* Find the closer ammo or health point to Supplier */
void Supplier::FindTargetPoint(int x1, int y1, int x2, int y2)
{
    double h;
    /* Calculate the heuristic of the two options */
    double h1 = sqrt(pow(((double)locationX - (double)x1), 2) + pow((double)locationY - (double)y1, 2));
    double h2 = sqrt(pow(((double)locationX - (double)x2), 2) + pow((double)locationY - (double)y2, 2));
    if (h1 < h2)
    {
        h = h1;
        target = new Node(y1, x1, nullptr, 0, 0);
    }
    else
    {
        h = h2;
        target = new Node(y2, x2, nullptr, 0, 0);
    }
    start = new Node(locationY, locationX, nullptr, 0, h);
}

/* Find the secure point in the current room */
void Soldier::SetSecurePoint(MazeController* mazeController, Soldier opponent, Room room)
{
    double h = 200, temp;
    int r = 0, c = 0;
    vector<Node*> nodes = room.GetObstacles();
    Node* pCurrent;
    while (!nodes.empty())
    {
        pCurrent = nodes.back(); // Get the last node in list
        nodes.pop_back(); // and remove it

        /* Find the secure point based on opponent location */

        // opponent up
        if (opponent.GetLocationY() > locationY)
        {
            temp = sqrt(pow(((double)locationX - (double)pCurrent->GetCol()), 2) +
                pow((double)locationY - (double)pCurrent->GetRow() - 1, 2));
            if (mazeController->maze[pCurrent->GetRow() - 1][pCurrent->GetCol()] == SPACE && temp < h)
            {
                c = pCurrent->GetCol();
                r = pCurrent->GetRow() - 1;
                h = temp;
            }
        }

        // opponent down
        if (opponent.GetLocationY() < locationY)
        {
            temp = sqrt(pow(((double)locationX - (double)pCurrent->GetCol()), 2) +
                pow((double)locationY - (double)pCurrent->GetRow() + 1, 2));
            if (mazeController->maze[pCurrent->GetRow() + 1][pCurrent->GetCol()] == SPACE && temp < h)
            {
                c = pCurrent->GetCol();
                r = pCurrent->GetRow() + 1;
                h = temp;
            }
        }

        // opponent left
        if (opponent.GetLocationX() < locationX)
        {
            temp = sqrt(pow(((double)locationX - (double)pCurrent->GetCol()) + 1, 2) +
                pow((double)locationY - (double)pCurrent->GetRow(), 2));
            if (mazeController->maze[pCurrent->GetRow()][pCurrent->GetCol() + 1] == SPACE && temp < h)
            {
                c = pCurrent->GetCol() + 1;
                r = pCurrent->GetRow();
                h = temp;
            }
        }

        // opponent right
        if (opponent.GetLocationX() > locationX)
        {
            temp = sqrt(pow(((double)locationX - (double)pCurrent->GetCol() - 1), 2) +
                pow((double)locationY - (double)pCurrent->GetRow(), 2));
            if (mazeController->maze[pCurrent->GetRow()][pCurrent->GetCol() - 1] == SPACE && temp < h)
            {
                c = pCurrent->GetCol() - 1;
                r = pCurrent->GetRow();
                h = temp;
            }
        }

    }

    mazeController->maze[r][c] = SECURE_POINT;
    target = new Node(r, c, nullptr, 0, 0);
    start = new Node(locationY, locationX, nullptr, 0, h);
}

void Soldier::SetPath(Node* current_node)
{
    /* Bulid the path of the soldier based on the A* algorithm */
    while (!current_node->operator==(*start))
    {
        path.push_back(current_node);
        current_node = current_node->GetParent();
    }
}

void Soldier::EmptyPath()
{
    /* Empty the path if we need to go to new target point */
    while (!path.empty())
        path.pop_back();
}

void Soldier::Move(MazeController* mazeController, int type_of_points)
{
    int minX = 0, minY = 0, maxX = 0, maxY = 0;
    /* Calculate minimum and maximum indexes in the room */
    if (room_index != PATH_INDEX)
    {
        minX = mazeController->rooms[room_index].GetCenterX() - mazeController->rooms[room_index].GetWidth() / 2;
        maxX = minX + mazeController->rooms[room_index].GetWidth();
        minY = mazeController->rooms[room_index].GetCenterY() - mazeController->rooms[room_index].GetHeight() / 2;
        maxY = minY + mazeController->rooms[room_index].GetHeight();
    }

    if (!path.empty()) // Walk in the path
        WalkInPath(mazeController, minX, minY, maxX, maxY);

    else // Arrived to target
        TargetAchieved(type_of_points);

}

void Soldier::WalkInPath(MazeController* mazeController, int minX, int minY, int maxX, int maxY)
{
    mazeController->maze[locationY][locationX] = SPACE; // Delete soldier from previous position
    Node* pcurrent = path.back(); // The last node in the path is the next step of the soldier
    path.pop_back(); // Remove it

    if (mazeController->maze[pcurrent->GetRow()][pcurrent->GetCol()] == SPACE && !isInRoom) // If soldier is in room
    {
        room_index = CalculateRoomIndex(mazeController);
        if (room_index != PATH_INDEX)
            isInRoom = true;
    }
    else if (room_index == PATH_INDEX) // Soldier in path
        isInRoom = false;
    else if (pcurrent->GetRow() < minY || pcurrent->GetRow() > maxY ||
        pcurrent->GetCol() < minX || pcurrent->GetCol() > maxX)
        isInRoom = false;

    locationX = pcurrent->GetCol();
    locationY = pcurrent->GetRow();
    mazeController->maze[locationY][locationX] = color;
}

void Soldier::TargetAchieved(int type_of_points)
{
    if (type_of_points == HEALT_POINTS)
    {
        health += HEALTH_POINTS_STOCK;
        if (color == SPETSNAZ)
            printf("Spetsnaz Soldier took Health Points!, Health: %d\n", health);
        else
            printf("Navy Seal took Health Points!, Health: %d\n", health);
    }
    else if (type_of_points == AMMO_STOCK)
    {
        bullets_ammo += BULLETS_STOCK;
        grenade_ammo += GRENADE_STOCK;
        if (color == SPETSNAZ)
            printf("Spetsnaz Soldier took Ammo!, Bullets: %d, Grenades: %d\n", bullets_ammo, grenade_ammo);
        else
            printf("Navy Seal took Ammo!, Bullets: %d, Grenades: %d\n", bullets_ammo, grenade_ammo);
    }

    target = nullptr; // Zero the target in order to search for new one
}

/* Calculate the soldier's room index while he is moving */
int Supplier::CalculateRoomIndex(MazeController* mazeController)
{
    int i, minX, minY, maxX, maxY;
    for (i = 0; i < NUM_ROOMS; i++)
    {
        /* Calculate minimum and maximum indexes in the current room */
        minX = mazeController->rooms[i].GetCenterX() - mazeController->rooms[i].GetWidth() / 2;
        maxX = minX + mazeController->rooms[i].GetWidth();
        minY = mazeController->rooms[i].GetCenterY() - mazeController->rooms[i].GetHeight() / 2;
        maxY = minY + mazeController->rooms[i].GetHeight();
        if (locationY >= minY && locationY <= maxY && locationX >= minX && locationX <= maxX) // The Supplier is in this room
        {
            //room_index = i;
            return i;
        }
    }

    //room_index = PATH_INDEX; // If not in any room, then in path
    return PATH_INDEX;
}


/* Adding neighbors to calculate the next step of A* */
void Supplier::AddNextStep(Node* pCurrent, DirectionType directionType, priority_queue <Node*, vector<Node*>, CompareNodes>& pq,
    vector <Node>& gray, vector <Node>& black, MazeController* mazeController)
{
    int i = 0, j = 0;
    Node* pn;
    double g, h;
    vector<Node>::iterator it_gray;
    vector<Node>::iterator it_black;

    switch (directionType)
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
    h = mazeController->Distance(j, i, target->GetCol(), target->GetRow());
    g = pCurrent->GetG() + mazeController->security_map[i][j];

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
                mazeController->UpdatePQ(pq, pn);
                gray.erase(it_gray);
                gray.push_back(*pn);
            }
        }
    }
}

/* Find the shortest path to target using A* algorithm */
void Supplier::FindPath(MazeController* mazeController)
{
    vector <Node> gray;
    vector <Node> black;
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;
    vector<Node>::iterator it_gray;
    vector<Node>::iterator it_black;
    Node* pn;
    Node* pCurrent;
    int startY, startX, targetY, targetX;
    startY = locationY;
    startX = locationX;
    targetY = target->GetRow();
    targetX = target->GetCol();
    pn = new Node(startY, startX, nullptr, 0, mazeController->Distance(startX, startY, targetX, targetY));
    // add start to PQ and to Grays
    pq.push(pn);
    gray.push_back(*pn);
    // while PQ is not empty
    while (!pq.empty())
    {
        // pick the "BEST"( F = G+H ) Node from PQ
        pCurrent = pq.top();
        pq.pop(); // and remove it from pq
        // if it is TARGET, the best path has been found
        if (pCurrent->operator==(*target))
        {
            // Set path to soldier
            SetPath(pCurrent);
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
            // Add neighbors of pCurrent to PQ.
                // check top neighbor
            if (mazeController->maze[pCurrent->GetRow() + 1][pCurrent->GetCol()] == SPACE ||
                mazeController->maze[pCurrent->GetRow() + 1][pCurrent->GetCol()] ==
                mazeController->maze[target->GetRow()][target->GetCol()])
                AddNextStep(pCurrent, TOP, pq, gray, black, mazeController);
            // check bottom neighbor
            if (mazeController->maze[pCurrent->GetRow() - 1][pCurrent->GetCol()] == SPACE ||
                mazeController->maze[pCurrent->GetRow() - 1][pCurrent->GetCol()] ==
                mazeController->maze[target->GetRow()][target->GetCol()])
                AddNextStep(pCurrent, BOTTOM, pq, gray, black, mazeController);
            // check left neighbor
            if (mazeController->maze[pCurrent->GetRow()][pCurrent->GetCol() - 1] == SPACE ||
                mazeController->maze[pCurrent->GetRow()][pCurrent->GetCol() - 1] ==
                mazeController->maze[target->GetRow()][target->GetCol()])
                AddNextStep(pCurrent, LEFT, pq, gray, black, mazeController);
            // check right neighbor
            if (mazeController->maze[pCurrent->GetRow()][pCurrent->GetCol() + 1] == SPACE ||
                mazeController->maze[pCurrent->GetRow()][pCurrent->GetCol() + 1] ==
                mazeController->maze[target->GetRow()][target->GetCol()])
                AddNextStep(pCurrent, RIGHT, pq, gray, black, mazeController);
        }// else
    }// while
}

/* Supplier searching for ammo */
void Supplier::SearchForAmmo(Soldier* pSoldier, MazeController* mazeController, Ammo* ammo1, Ammo* ammo2)
{
    
    /* Calculate the target node and find the path to this point in case when the current soldier do not have one */
    if (target == nullptr)
    {
        FindTargetPoint(ammo1->GetX(), ammo1->GetY(), ammo2->GetX(), ammo2->GetY());
        FindPath(mazeController);
    }
    /* the target point is already took by other soldier */
    else if (mazeController->maze[target->GetRow()][target->GetCol()] != AMMO_STOCK)
    {
        /* if there is still at least one ammo point on the map */
        if (ammo1->GetX() != INFINITY_VALUE || ammo2->GetX() != INFINITY_VALUE)
        {
            FindTargetPoint(ammo1->GetX(), ammo1->GetY(), ammo2->GetX(), ammo2->GetY());
            EmptyPath(); // Delete previous path
            FindPath(mazeController);
        }
    }

    /* When the soldier reached to the ammo point, remove it from the map */
    if (locationX == target->GetCol() && locationY == target->GetRow())
        mazeController->RemoveAmmoPoint(ammo1, ammo2, locationY, locationX);

    Move(mazeController, AMMO_STOCK); // Soldier step
}

/* Soldier searching for health */
void Supplier::SearchForHealth(Soldier* pSoldier,MazeController* mazeController, HealthPoints* hp1, HealthPoints* hp2)
{
    /* Calculate the target node and find the path to this point in case when the current soldier do not have one */
    if (target == nullptr)
    {
        FindTargetPoint(hp1->GetX(), hp1->GetY(), hp2->GetX(), hp2->GetY());
        FindPath(mazeController);
    }
    /* the target point is already took by other soldier */
    else if (mazeController->maze[target->GetRow()][target->GetCol()] != HEALT_POINTS)
    {
        /* if there is still at least one health point on the map */
        if (hp1->GetX() != INFINITY_VALUE || hp2->GetX() != INFINITY_VALUE)
        {
            FindTargetPoint(hp1->GetX(), hp1->GetY(), hp2->GetX(), hp2->GetY());
            EmptyPath(); // Delete previous path
            FindPath(mazeController);
        }
    }

    /* When the soldier reached to the health point, remove it from the map */
    if (locationX == target->GetCol() && locationY == target->GetRow())
        mazeController->RemoveHealthPoint(hp1, hp2, locationY, locationX);

    Move(mazeController, HEALT_POINTS); // Soldier step	
}



void Soldier::Attack(MazeController* mazeController, Soldier opponent_team[NUM_SOLDIERS], Soldier opponent)
{
    /* If the opponent behind an obstacle, throw grenade */
    if ((isSecure(mazeController, opponent) || opponent.isSecure(mazeController, *this)) && grenade_ammo > 0)
        ThrowGrenade(opponent_team, mazeController);

    else if (bullets_ammo > 0)
        Shoot(opponent_team, mazeController);
}

void Soldier::SearchForCover(MazeController* mazeController, Soldier opponent)
{
    double x, y;
    x = 2 * (double)opponent.GetLocationX() / (double)MSZ - 1;
    y = 2 * (double)opponent.GetLocationY() / (double)MSZ - 1;
    Grenade* pg = new Grenade(x, y);
    pg->UpdateSecurityMap(mazeController->maze, mazeController->security_map);
    /* Calculate the target node and find the path to this point in case when the current soldier do not have one */
    if (target == nullptr)
    {
        SetSecurePoint(mazeController, opponent, mazeController->rooms[room_index]);
        FindPath(mazeController);
    }
    /* the secure point is already took by other soldier */
    else if (mazeController->maze[target->GetRow()][target->GetCol()] != SECURE_POINT)
    {
        SetSecurePoint(mazeController, opponent, mazeController->rooms[room_index]);
        EmptyPath(); // Delete previous path
        FindPath(mazeController);
    }

    /* When arrived to secure point, paint it SPACE again */
    if (locationX == target->GetCol() && locationY == target->GetRow())
        mazeController->maze[locationY][locationX] = SPACE;

    Move(mazeController, SECURE_POINT); // Soldier step
}

void Soldier::MoveToRandomRoom(MazeController* mazeController)
{
    int room_index, minX = 0, minY = 0, x = 0, y = 0;
    double h = 0;
    Node* pn = nullptr;
    if (target == nullptr)
    {
        room_index = rand() % NUM_ROOMS;
        /* If the center of the room is SPACE, go there*/
        if (mazeController->maze[mazeController->rooms[room_index].GetCenterY()][mazeController->rooms[room_index].GetCenterX()] == SPACE)
        {
            pn = new Node(mazeController->rooms[room_index].GetCenterY(), mazeController->rooms[room_index].GetCenterX(), nullptr, 0, 0);
            h = mazeController->Distance(locationX, locationY, mazeController->rooms[room_index].GetCenterX(), mazeController->rooms[room_index].GetCenterY());
        }
        else // Search for random point in the room
        {
            minX = mazeController->rooms[room_index].GetCenterX() - mazeController->rooms[room_index].GetWidth() / 2;
            minY = mazeController->rooms[room_index].GetCenterY() - mazeController->rooms[room_index].GetHeight() / 2;

            while (mazeController->maze[y][x] != SPACE)
            {
                /* Generate random x and y in the room*/
                x = rand() % mazeController->rooms[room_index].GetWidth() + minX;
                y = rand() % mazeController->rooms[room_index].GetHeight() + minY;
            }
            h = mazeController->Distance(locationX, locationY, x, y);
            pn = new Node(y, x, nullptr, 0, 0);
        }
        target = pn;
        start = new Node(locationY, locationX, nullptr, 0, h);
        FindPath(mazeController);
    }

    Move(mazeController, MOVEMENT); // Soldier Step
}

void Soldier::SearchForEnemy(MazeController* mazeController, Soldier opponent_team[NUM_SOLDIERS], bool* withEnemy)
{
    int i;
    for (i = 0; i < NUM_SOLDIERS; i++)
    {
        if (room_index == opponent_team[i].GetRoomIndex() && room_index != PATH_INDEX)
        {
            Attack(mazeController, opponent_team, opponent_team[i]);
            SearchForCover(mazeController, opponent_team[i]);
            *withEnemy = true;
        }
    }

}




