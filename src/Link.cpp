#include "../include/Link.hpp"

#include <gfl.h>

Link::Link(PMat *M1, PMat *M2, double k, double z) : M1(M1), M2(M2), k(k), z(z)
{
    l0 = M1->getPos().dist(M2->getPos());
}

void Link::update()
{
    double d = M1->getPos().dist(M2->getPos());
    Vect u = (M2->getPos() - M1->getPos()) / d;
    // Vect f = u * (-k * (d - l0));
    Vect f = u * (-k * (d - l0)) - (M2->getVit() - M1->getVit()) * z;
    M1->addForce(-f);
    M2->addForce(f);
}

void Link::draw() const
{
    auto p1 = M1->getPos();
    auto p2 = M2->getPos();
    GFLpoint A = {p1.getX(), p1.getY()};
    GFLpoint B = {p2.getX(), p2.getY()};
    GFLcolor c = {0, 0, 1};
    gfl_DrawLine(A, B, c, 2);
}

void Link::update_gravity(double gravity)
{
    M1->addForce(Vect(0, -gravity, 0));
    M2->addForce(Vect(0, -gravity, 0));
}

void Link::update_wind(double wind)
{
    M1->addForce(Vect(wind, 0, 0));
    M2->addForce(Vect(wind, 0, 0));
}
