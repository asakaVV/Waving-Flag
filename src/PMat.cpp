#include "../include/PMat.hpp"

#include <gfl.h>

PMat::PMat(double m, Point pos, Vect vit, int type) : m(m), pos(pos), vit(vit), type(type)
{
    frc = Vect(0, 0, 0);
}

void PMat::update(double h)
{
    switch (type)
    {
    case 0:
        updateFixe(h);
        break;
    case 1:
        updateEulerExp(h);
        break;
    case 2:
        updateLeapFrog(h);
        break;
    default:
        break;
    }
}

void PMat::addForce(Vect f)
{
    frc += f;
}

void PMat::updateLeapFrog(double h)
{
    vit += frc * h / m;
    pos += vit * h;
    frc = Vect(0, 0, 0);
}

void PMat::updateEulerExp(double h)
{
    pos += vit * h;
    vit += frc * h / m;
    frc = Vect(0, 0, 0);
}

void PMat::updateFixe(double h)
{
    frc = Vect(0, 0, 0);
}

void PMat::draw() const
{
    GFLpoint p = {pos.getX(), pos.getY()};
    GFLcolor c;
    switch (type)
    {
    case 0:
        c = {0, 0, 0};
        break;
    case 1:
        c = {1, 0, 0};
        break;
    default:
        c = {0, 1, 0};
        break;
    }
    gfl_DrawFillCircle(p, 0.05, c);
}
