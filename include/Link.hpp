#pragma once

#include "PMat.hpp"

class Link {
public:
    Link(PMat *M1, PMat *M2, double k, double z);
    void update();
    void update_gravity(double gravity);
    void update_wind(double wind);

    void draw() const;

private:
    PMat *M1;
    PMat *M2;
    double k;
    double z;
    double l0;
};
