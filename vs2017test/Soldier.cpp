#include "Soldier.h"
#include <stdlib.h>
#include "Room.h"
#include "glut.h"

/* ---------- Constructors ---------- */

Soldier::Soldier()
{
    locationX = 0;
    locationY = 0;
    health = START_HEALTH;
    total_bullets_ammo = START_BULLETS_AMMO;
    bullets_stack_ammo = BULLETS_PER_STACK;
    grenade_ammo = START_GRENADES_AMMO;
    isInRoom = true;
    room_index = 0;
    color = SPACE;
    start = nullptr;
    target = nullptr;
    pb = nullptr;
    pg = nullptr;
}

Soldier::Soldier(int x, int y, int room, int team_color)
{
    locationX = x;
    locationY = y;
    health = START_HEALTH;
    total_bullets_ammo = START_BULLETS_AMMO; // Total amount of bullets in the vest
    bullets_stack_ammo = BULLETS_PER_STACK; // Total amount of bullets in weapon
    grenade_ammo = START_GRENADES_AMMO;
    isInRoom = true;
    room_index = room;
    color = team_color;
    start = nullptr;
    target = nullptr;
    pb = nullptr;
    pg = nullptr;
}

/* ---------- Getters ---------- */

int Soldier::GetLocationX()
{
    return locationX;
}

int Soldier::GetLocationY()
{
    return locationY;
}

int Soldier::GetHealth()
{
    return health;
}

int Soldier::GetBulletsAmmo() // returns the amount of bullets in vest
{
    return total_bullets_ammo;
}

int Soldier::GetBulletsStack() // returns the amount of bullets in weapon
{
    return bullets_stack_ammo;
}

int Soldier::GetGrenadeAmmo()
{
    return grenade_ammo;
}

int Soldier::GetColor()
{
    return color;
}

int Soldier::GetRoomIndex()
{
    return room_index;
}

/* Check if soldier is behind a shelter */
bool Soldier::isSecure(MazeHandler* mazeHandler, Soldier opponent)
{
    int minX, minY, maxX, maxY;
    minX = mazeHandler->rooms[opponent.GetRoomIndex()].GetCenterX() - mazeHandler->rooms[opponent.GetRoomIndex()].GetWidth() / 2;
    minY = mazeHandler->rooms[opponent.GetRoomIndex()].GetCenterY() - mazeHandler->rooms[opponent.GetRoomIndex()].GetHeight() / 2;
    maxX = minX + mazeHandler->rooms[opponent.GetRoomIndex()].GetWidth();
    maxY = minY + mazeHandler->rooms[opponent.GetRoomIndex()].GetHeight();

    // Check if a soldier is behind a wall and the wall is not an external point from the room
    if ((mazeHandler->maze[locationY + 1][locationX] == WALL && locationY + 1 < maxY) ||
        (mazeHandler->maze[locationY - 1][locationX] == WALL && locationY - 1 > minY) ||
        (mazeHandler->maze[locationY][locationX + 1] == WALL && locationX + 1 < maxX) ||
        (mazeHandler->maze[locationY][locationX - 1] == WALL && locationX - 1 > minX))
        return true;
    return false;

}

/* ---------- Setters ---------- */

void Soldier::SetColor(int team_color)
{
    color = team_color;
}

void Soldier::SetRoomIndex(int index)
{
    room_index = index;
}

void Soldier::SetLocationX(int lx)
{
    locationX = lx;
}

void Soldier::SetLocationY(int ly)
{
    locationY = ly;
}

void Soldier::SetHealth(int health)
{
    this->health = health;
}

void Soldier::UpdateHealth(int health_points)
{
    health += health_points;
}

void Soldier::UpdateBulletsAmmo(int num_of_bullets)
{
    bullets_stack_ammo += num_of_bullets;
}

void Soldier::UpdateGrenadeAmmo(int num_of_grenades)
{
    grenade_ammo += num_of_grenades;
}

/* ---------- Game Functions ---------- */

/* Find the closer ammo or health point to soldier */


void Soldier::Reload() {
    bullets_stack_ammo += 5;
    total_bullets_ammo -= 1;
}

void Soldier::FindTargetPoint(int x1, int y1, int x2, int y2)
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
void Soldier::SetSecurePoint(MazeHandler* mazeHandler, Soldier opponent, Room room)
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
            if (mazeHandler->maze[pCurrent->GetRow() - 1][pCurrent->GetCol()] == SPACE && temp < h)
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
            if (mazeHandler->maze[pCurrent->GetRow() + 1][pCurrent->GetCol()] == SPACE && temp < h)
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
            if (mazeHandler->maze[pCurrent->GetRow()][pCurrent->GetCol() + 1] == SPACE && temp < h)
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
            if (mazeHandler->maze[pCurrent->GetRow()][pCurrent->GetCol() - 1] == SPACE && temp < h)
            {
                c = pCurrent->GetCol() - 1;
                r = pCurrent->GetRow();
                h = temp;
            }
        }

    }

    mazeHandler->maze[r][c] = SECURE_POINT;
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

void Soldier::Move(MazeHandler* mazeHandler, int type_of_points)
{
    int minX = 0, minY = 0, maxX = 0, maxY = 0;
    /* Calculate minimum and maximum indexes in the room */
    if (room_index != PATH_INDEX)
    {
        minX = mazeHandler->rooms[room_index].GetCenterX() - mazeHandler->rooms[room_index].GetWidth() / 2;
        maxX = minX + mazeHandler->rooms[room_index].GetWidth();
        minY = mazeHandler->rooms[room_index].GetCenterY() - mazeHandler->rooms[room_index].GetHeight() / 2;
        maxY = minY + mazeHandler->rooms[room_index].GetHeight();
    }

    if (!path.empty()) // Walk in the path
        WalkInPath(mazeHandler, minX, minY, maxX, maxY);

   else // Arrived to target
        TargetAchieved(type_of_points);    

}

void Soldier::WalkInPath(MazeHandler* mazeHandler, int minX, int minY, int maxX, int maxY)
{
    mazeHandler->maze[locationY][locationX] = SPACE; // Delete soldier from previous position
    Node* pcurrent = path.back(); // The last node in the path is the next step of the soldier
    path.pop_back(); // Remove it

    if (mazeHandler->maze[pcurrent->GetRow()][pcurrent->GetCol()] == SPACE && !isInRoom) // If soldier is in room
    {
        room_index = CalculateRoomIndex(mazeHandler);
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
    mazeHandler->maze[locationY][locationX] = color;
}

void Soldier::TargetAchieved(int type_of_points)
{
    if (type_of_points == HEALT_POINTS)
    {
        health += HEALTH_POINTS_STOCK;
        if (color == CYAN_SOLDIER)
            printf("Cyan Soldier took Health Points!, Health: %d\n", health);
        else
            printf("Olive Soldier took Health Points!, Health: %d\n", health);
    }
    else if (type_of_points == AMMO_STOCK)
    {
        bullets_stack_ammo += BULLETS_STOCK;
        grenade_ammo += GRENADE_STOCK;
        if (color == CYAN_SOLDIER)
            printf("Cyan Soldier took Ammo!, Bullets: %d, Grenades: %d\n", bullets_stack_ammo, grenade_ammo);
        else
            printf("Olive Soldier took Ammo!, Bullets: %d, Grenades: %d\n", bullets_stack_ammo, grenade_ammo);
    }

    target = nullptr; // Zero the target in order to search for new one
}

/* Calculate the soldier's room index while he is moving */
int Soldier::CalculateRoomIndex(MazeHandler* mazeHandler)
{
    int i, minX, minY, maxX, maxY;
    for (i = 0; i < NUM_ROOMS; i++)
    {
        /* Calculate minimum and maximum indexes in the current room */
        minX = mazeHandler->rooms[i].GetCenterX() - mazeHandler->rooms[i].GetWidth() / 2;
        maxX = minX + mazeHandler->rooms[i].GetWidth();
        minY = mazeHandler->rooms[i].GetCenterY() - mazeHandler->rooms[i].GetHeight() / 2;
        maxY = minY + mazeHandler->rooms[i].GetHeight();
        if (locationY >= minY && locationY <= maxY && locationX >= minX && locationX <= maxX) // The soldier is in this room
        {
            //room_index = i;
            return i;
        }
    }

    //room_index = PATH_INDEX; // If not in any room, then in path
    return PATH_INDEX;
}

/* Shoot Bullet */
void Soldier::Shoot(Soldier opponent_team[NUM_SOLDIERS], MazeHandler* mazeHandler)
{

    if (health <= 0)
        return;

    if (pb == nullptr) // Initialize bullet
        SetBulletDetails(opponent_team);

    else if (pb->GetIsMoving())
        ShotMovement(mazeHandler, opponent_team);

    else // Bullet hit wall or enemy
        pb = nullptr;

}

void Soldier::SetBulletDetails(Soldier opponent_team[NUM_SOLDIERS])
{
    double bx, by, dirx = 0, diry = 0, xdistance, ydistance, size;
    for (int i = 0; i < NUM_SOLDIERS; i++)
    {
        if (room_index == opponent_team[i].GetRoomIndex())
        {
            if (bullets_stack_ammo > 0)
            {
                /* Calculate bullet location and dierection */
                bx = 2 * (double)locationX / (double)MSZ - 1;
                by = 2 * (double)locationY / (double)MSZ - 1;

                xdistance = (double)opponent_team[i].GetLocationX() - (double)locationX;
                ydistance = (double)opponent_team[i].GetLocationY() - (double)locationY;
                size = sqrt(pow(xdistance, 2) + pow(ydistance, 2));
                dirx = xdistance / size;
                diry = ydistance / size;
                pb = new Bullet(bx, by, false, dirx, diry); // Create new bullet
                pb->Shoot(); //Start Bullet Movement
                bullets_stack_ammo--;
                break;
            }
        }

    }
}

void Soldier::ShotMovement(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS])
{
    double distance;
    int damage, row, col, i;
    i = 0;
    Soldier* pOpponent;
    while (i < BULLET_MOVEMENT && pb->GetIsMoving()) // Move the bullet x (as defined) times before soldiers move
    {
        row = MSZ * (pb->GetY() + 1) / 2;
        col = MSZ * (pb->GetX() + 1) / 2;

        /* Bullet Hit */
        if ((mazeHandler->maze[row][col] == CYAN_SOLDIER && color != CYAN_SOLDIER) ||
            (mazeHandler->maze[row][col] == OLIVE_SOLDIER && color != OLIVE_SOLDIER))
        {
            for (int j = 0; j < NUM_SOLDIERS; j++) // Find the injured soldier
            {
                pOpponent = &opponent_team[j];
                if (row == pOpponent->GetLocationY() && col == pOpponent->GetLocationX())
                {
                    distance = sqrt(pow(((double)locationX - pOpponent->GetLocationX()), 2) +
                        pow((double)locationY - pOpponent->GetLocationY(), 2));
                    damage = 2.0 * 100 / distance;  // calculate damage while considering distance between soldier
                    pOpponent->UpdateHealth(-damage);

                    if (pOpponent->GetHealth() < 0)
                        pOpponent->SetHealth(0);

                    if (pOpponent->GetColor() == CYAN_SOLDIER)
                        printf("Cyan Soldier Was Wounded!, Health: %d\n", pOpponent->GetHealth());
                    else
                        printf("Olive Soldier Was Wounded!, Health: %d\n", pOpponent->GetHealth());

                    pb->StopMoving();

                    if (pOpponent->GetHealth() <= 0) // Opponent Died
                    {
                        if (pOpponent->GetColor() == CYAN_SOLDIER)
                            printf("Cyan Soldier Died!\n");
                        else
                            printf("Olive Soldier Died!\n");
                    }
                    break;
                }
            }
        }
        pb->Move(mazeHandler->maze);
        i++;
    }
    pb->Draw();
}

/* Throw Grenade */
void Soldier::ThrowGrenade(Soldier opponent_team[NUM_SOLDIERS], MazeHandler* mazeHandler)
{
    double distance;
    int damage, row, col, i;
    Soldier* popponent;

    if (health <= 0)
        return;

    if (pg == nullptr) // Initialize grenade
        SetGrenadeDetails(opponent_team, mazeHandler);

    else if (pg->GetIsMoving())
        GrenadeMovement(mazeHandler, opponent_team);

    else
        pg = nullptr;  
}

void Soldier::SetGrenadeDetails(Soldier opponent_team[NUM_SOLDIERS], MazeHandler* mazeHandler)
{
    Soldier* pOpponent;
    double bx, by, dirx = 0, diry = 0, xdistance, ydistance, size;
    for (int i = 0; i < NUM_SOLDIERS; i++)
    {
        if (room_index == opponent_team[i].GetRoomIndex())
        {
            pOpponent = &opponent_team[i];
            if (grenade_ammo > 0)
            {
                /* Calculate grenade location and dierection */
                bx = 2 * (double)locationX / (double)MSZ - 1;
                by = 2 * (double)locationY / (double)MSZ - 1;

                pg = new Grenade(bx, by); // Create new grenade

                xdistance = (double)pOpponent->GetLocationX() - (double)locationX;
                ydistance = (double)pOpponent->GetLocationY() - (double)locationY;
                size = sqrt(pow(xdistance, 2) + pow(ydistance, 2));
                dirx = xdistance / size;
                diry = ydistance / size;

                pg->SetDirX(dirx);
                pg->SetDirY(diry);

                // Start grenade movement 
                pg->Throw(mazeHandler->maze, locationX, locationY, pOpponent->GetLocationX(), pOpponent->GetLocationY());
                grenade_ammo--;
                pg->Explode();
                break;
            }
        }

    }
}

void Soldier::GrenadeMovement(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS])
{
    int row, col, i;
    bool isAlive = true;
    Bullet* pBullet;

    row = pg->GetY();
    col = pg->GetX();
    if (row != pg->GetTargetY() || col != pg->GetTargetX()) // Grenade doesn't arrived to target yet
        pg->Throw(mazeHandler->maze, 0, 0, 0, 0); // continue movement

    else
    {
        i = 0;
        while (i < BULLET_MOVEMENT && pg->GetIsMoving()) // Move the bullet x (as defined) times before soldiers move
        {
            for (int j = 0; j < NUM_BULLETS; j++)
            {
                pBullet = pg->GetBulletFromIndex(j);
                row = MSZ * (pBullet->GetY() + 1) / 2;
                col = MSZ * (pBullet->GetX() + 1) / 2;
                if ((mazeHandler->maze[row][col] == CYAN_SOLDIER && color != CYAN_SOLDIER) || 
                    (mazeHandler->maze[row][col] == OLIVE_SOLDIER && color != OLIVE_SOLDIER))
                {
                    GrenadeHit(opponent_team, row, col, pBullet);
                }
            }
            i++;
            pg->Move(mazeHandler->maze);
        }
    }
    pg->Draw();
}

void Soldier::GrenadeHit(Soldier opponent_team[NUM_SOLDIERS], int row, int col, Bullet* pBullet) 
{
    int damage;
    double distance;
    bool isAlive = true;
    Soldier* pOpponent;

    for (int k = 0; k < NUM_SOLDIERS; k++)
    {
        pOpponent = &opponent_team[k];
        if (row == pOpponent->GetLocationY() && col == pOpponent->GetLocationX() && pBullet->GetIsMoving())
        {
            distance = sqrt(pow(((double)pg->GetX() - pOpponent->GetLocationX()), 2) +
                pow((double)pg->GetY() - pOpponent->GetLocationY(), 2));
            damage = 100 / distance;
            pOpponent->UpdateHealth(-damage);
            if (pOpponent->GetHealth() < 0)
            {
                pOpponent->SetHealth(0);
                if (pOpponent->GetColor() == CYAN_SOLDIER && isAlive)
                    printf("Cyan Soldier Died By Grenade!\n", pOpponent->GetHealth());
                else if (isAlive)
                    printf("Olive Soldier Died By Grenade!\n", pOpponent->GetHealth());
                isAlive = false;
            }
            else
            {
                if (pOpponent->GetColor() == CYAN_SOLDIER)
                    printf("Cyan Soldier Was Wounded By Grenade!, Health: %d\n", pOpponent->GetHealth());
                else
                    printf("Olive Soldier Was Wounded By Grenade!, Health: %d\n", pOpponent->GetHealth());
            }
            pBullet->StopMoving();
            break;
        }
    }
}

/* Adding neighbors to calculate the next step of A* */
void Soldier::AddNextStep(Node* pCurrent, DirectionType directionType, priority_queue <Node*, vector<Node*>, CompareNodes>& pq,
    vector <Node>& gray, vector <Node>& black, MazeHandler* mazeHandler)
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
    h = mazeHandler->Distance(j, i, target->GetCol(), target->GetRow());
    g = pCurrent->GetG() + mazeHandler->security_map[i][j];

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
                mazeHandler->UpdatePQ(pq, pn);
                gray.erase(it_gray);
                gray.push_back(*pn);
            }
        }
    }
}

/* Find the shortest path to target using A* algorithm */
void Soldier::FindPath(MazeHandler* mazeHandler)
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
    pn = new Node(startY, startX, nullptr, 0, mazeHandler->Distance(startX, startY, targetX, targetY));
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
            if (mazeHandler->maze[pCurrent->GetRow() + 1][pCurrent->GetCol()] == SPACE ||
                mazeHandler->maze[pCurrent->GetRow() + 1][pCurrent->GetCol()] ==
                mazeHandler->maze[target->GetRow()][target->GetCol()])
                AddNextStep(pCurrent, TOP, pq, gray, black, mazeHandler);
            // check bottom neighbor
            if (mazeHandler->maze[pCurrent->GetRow() - 1][pCurrent->GetCol()] == SPACE ||
                mazeHandler->maze[pCurrent->GetRow() - 1][pCurrent->GetCol()] ==
                mazeHandler->maze[target->GetRow()][target->GetCol()])
                AddNextStep(pCurrent, BOTTOM, pq, gray, black, mazeHandler);
            // check left neighbor
            if (mazeHandler->maze[pCurrent->GetRow()][pCurrent->GetCol() - 1] == SPACE ||
                mazeHandler->maze[pCurrent->GetRow()][pCurrent->GetCol() - 1] ==
                mazeHandler->maze[target->GetRow()][target->GetCol()])
                AddNextStep(pCurrent, LEFT, pq, gray, black, mazeHandler);
            // check right neighbor
            if (mazeHandler->maze[pCurrent->GetRow()][pCurrent->GetCol() + 1] == SPACE ||
                mazeHandler->maze[pCurrent->GetRow()][pCurrent->GetCol() + 1] ==
                mazeHandler->maze[target->GetRow()][target->GetCol()])
                AddNextStep(pCurrent, RIGHT, pq, gray, black, mazeHandler);
        }// else
    }// while
}

/* Soldier searching for ammo */
void Soldier::SearchForAmmo(MazeHandler* mazeHandler, Ammo* ammo1, Ammo* ammo2)
{
    /* Calculate the target node and find the path to this point in case when the current soldier do not have one */
    if (target == nullptr)
    {
        FindTargetPoint(ammo1->GetX(), ammo1->GetY(), ammo2->GetX(), ammo2->GetY());
        FindPath(mazeHandler);
    }
    /* the target point is already took by other soldier */
    else if (mazeHandler->maze[target->GetRow()][target->GetCol()] != AMMO_STOCK)
    {
        /* if there is still at least one ammo point on the map */
        if (ammo1->GetX() != INFINITY_VALUE || ammo2->GetX() != INFINITY_VALUE)
        {
            FindTargetPoint(ammo1->GetX(), ammo1->GetY(), ammo2->GetX(), ammo2->GetY());
            EmptyPath(); // Delete previous path
            FindPath(mazeHandler);
        }
    }

    /* When the soldier reached to the ammo point, remove it from the map */
    if (locationX == target->GetCol() && locationY == target->GetRow())
        mazeHandler->RemoveAmmoPoint(ammo1, ammo2, locationY, locationX);

    Move(mazeHandler, AMMO_STOCK); // Soldier step
}

/* Soldier searching for health */
void Soldier::SearchForHealth(MazeHandler* mazeHandler, HealthPoints* hp1, HealthPoints* hp2)
{
    /* Calculate the target node and find the path to this point in case when the current soldier do not have one */
    if (target == nullptr)
    {
        FindTargetPoint(hp1->GetX(), hp1->GetY(), hp2->GetX(), hp2->GetY());
        FindPath(mazeHandler);
    }
    /* the target point is already took by other soldier */
    else if (mazeHandler->maze[target->GetRow()][target->GetCol()] != HEALT_POINTS)
    {
        /* if there is still at least one health point on the map */
        if (hp1->GetX() != INFINITY_VALUE || hp2->GetX() != INFINITY_VALUE)
        {
            FindTargetPoint(hp1->GetX(), hp1->GetY(), hp2->GetX(), hp2->GetY());
            EmptyPath(); // Delete previous path
            FindPath(mazeHandler);
        }
    }

    /* When the soldier reached to the health point, remove it from the map */
    if (locationX == target->GetCol() && locationY == target->GetRow())
        mazeHandler->RemoveHealthPoint(hp1, hp2, locationY, locationX);

    Move(mazeHandler, HEALT_POINTS); // Soldier step	
}

void Soldier::Attack(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS], Soldier opponent)
{
    /* If the opponent behind an obstacle, throw grenade */
    if ((isSecure(mazeHandler, opponent) || opponent.isSecure(mazeHandler, *this)) && grenade_ammo > 0)
        ThrowGrenade(opponent_team, mazeHandler);

    else if (bullets_stack_ammo > 0)
        Shoot(opponent_team, mazeHandler);
}

void Soldier::SearchForCover(MazeHandler* mazeHandler, Soldier opponent)
{
    double x, y;
    x = 2 * (double)opponent.GetLocationX() / (double)MSZ - 1;
    y = 2 * (double)opponent.GetLocationY() / (double)MSZ - 1;
    Grenade* pg = new Grenade(x, y);
    pg->UpdateSecurityMap(mazeHandler->maze, mazeHandler->security_map);
    /* Calculate the target node and find the path to this point in case when the current soldier do not have one */
    if (target == nullptr)
    {
        SetSecurePoint(mazeHandler, opponent, mazeHandler->rooms[room_index]);
        FindPath(mazeHandler);
    }
    /* the secure point is already took by other soldier */
    else if (mazeHandler->maze[target->GetRow()][target->GetCol()] != SECURE_POINT)
    {
        SetSecurePoint(mazeHandler, opponent, mazeHandler->rooms[room_index]);
        EmptyPath(); // Delete previous path
        FindPath(mazeHandler);
    }

    /* When arrived to secure point, paint it SPACE again */
    if (locationX == target->GetCol() && locationY == target->GetRow())
        mazeHandler->maze[locationY][locationX] = SPACE;

    Move(mazeHandler, SECURE_POINT); // Soldier step
}

void Soldier::MoveToRandomRoom(MazeHandler* mazeHandler)
{
    int room_index, minX = 0, minY = 0, x = 0, y = 0;
    double h = 0;
    Node* pn = nullptr;
    if (target == nullptr)
    {
        room_index = rand() % NUM_ROOMS;
        /* If the center of the room is SPACE, go there*/
        if (mazeHandler->maze[mazeHandler->rooms[room_index].GetCenterY()][mazeHandler->rooms[room_index].GetCenterX()] == SPACE)
        {
            pn = new Node(mazeHandler->rooms[room_index].GetCenterY(), mazeHandler->rooms[room_index].GetCenterX(), nullptr, 0, 0);
            h = mazeHandler->Distance(locationX, locationY, mazeHandler->rooms[room_index].GetCenterX(), mazeHandler->rooms[room_index].GetCenterY());
        }
        else // Search for random point in the room
        {
            minX = mazeHandler->rooms[room_index].GetCenterX() - mazeHandler->rooms[room_index].GetWidth() / 2;
            minY = mazeHandler->rooms[room_index].GetCenterY() - mazeHandler->rooms[room_index].GetHeight() / 2;

            while (mazeHandler->maze[y][x] != SPACE)
            {
                /* Generate random x and y in the room*/
                x = rand() % mazeHandler->rooms[room_index].GetWidth() + minX;
                y = rand() % mazeHandler->rooms[room_index].GetHeight() + minY;
            }
            h = mazeHandler->Distance(locationX, locationY, x, y);
            pn = new Node(y, x, nullptr, 0, 0);
        }
        target = pn;
        start = new Node(locationY, locationX, nullptr, 0, h);
        FindPath(mazeHandler);
    }

    Move(mazeHandler, MOVEMENT); // Soldier Step
}

void Soldier::SearchForEnemy(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS], bool* withEnemy)
{
    int i;
    for (i = 0; i < NUM_SOLDIERS; i++)
    {
        if (room_index == opponent_team[i].GetRoomIndex() && room_index != PATH_INDEX)
        {
            Attack(mazeHandler, opponent_team, opponent_team[i]);
            SearchForCover(mazeHandler, opponent_team[i]);
            *withEnemy = true;
        }
    }

}

void Soldier::HideFromEnemy(MazeHandler* mazeHandler, Soldier opponent_team[NUM_SOLDIERS], bool* withEnemy)
{
    int i;
    for (i = 0; i < NUM_SOLDIERS; i++)
    {
        if (room_index == opponent_team[i].GetRoomIndex() && room_index != PATH_INDEX)
        {
            SearchForCover(mazeHandler, opponent_team[i]);
            *withEnemy = true;
        }
    }

}




