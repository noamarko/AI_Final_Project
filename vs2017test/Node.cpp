#include "Node.h"

/* ---------- Constructors ---------- */

Node::Node()
{
    row = 0;
    col = 0;
    parent = nullptr;
    g = 0;
    h = 0;
    f = g + h;
}

Node::Node(int r, int c, Node* pr, double g, double h)
{
    row = r;
    col = c;
    parent = pr;
    this->g = g;
    this->h = h;
    f = g + h;
}

/* ---------- Getters ---------- */

int Node::GetRow()
{
    return row;
}

int Node::GetCol()
{
    return col;
}

Node* Node::GetParent()
{
    return parent;
}

double Node::GetG()
{
    return g;
}

double Node::GetF()
{
    return f;
}

double Node::GetH()
{
    return h;
}

/* ---------- Setters ---------- */

void Node::SetRow(int r)
{
    row = r;
}

void Node::SetCol(int c)
{
    col = c;
}

void Node::SetG(double g)
{
    this->g = g;
}

void Node::SetH(double h)
{
    this->h = h;
}

void Node::ComputeF()
{
    f = g + h;
}

void Node::SetParent(Node* p)
{
    parent = p;
}
