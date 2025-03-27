#pragma once

#include "Point.hpp"
#include "Vect.hpp"

class PMat {
public:
    PMat(double m, Point pos, Vect vit, int type);
    void update(double h);

    void addForce(Vect f);

    Point getPos() const { return pos; }
    Vect getVit() const { return vit; }

    Point* getPosPtr() { return &pos; }

    void draw() const;

    void setType(int t) { type = t; }

private:
    double m;
    Point pos;
    Vect vit;
    Vect frc;
    int type;

    void updateLeapFrog(double h);
    void updateEulerExp(double h);
    void updateFixe(double h);
};
