#include "../include/GridCollision.hpp"
#include <cmath>

GridCollision::GridCollision(double cellSize) : cellSize(cellSize) {}

void GridCollision::updateGrid(const std::vector<PMat>& particles) {
    grid.clear();
    for (const auto& particle : particles) {
        auto cellCoords = getCellCoords(particle.getPos());
        grid[cellCoords].push_back(const_cast<PMat*>(&particle));
    }
}

std::vector<std::pair<PMat*, PMat*>> GridCollision::detectCollisions() {
    std::vector<std::pair<PMat*, PMat*>> collisions;

    for (const auto& [cell, particles] : grid) {
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                double distance = particles[i]->getPos().dist(particles[j]->getPos());
                if (distance < cellSize) {
                    collisions.emplace_back(particles[i], particles[j]);

                    Vect direction = (particles[j]->getPos() - particles[i]->getPos()) / distance;
                    Vect repulsiveForce = direction * (cellSize - distance) * 10.0; // Arbitrary strength
                    particles[i]->addForce(-repulsiveForce);
                    particles[j]->addForce(repulsiveForce);
                }
            }
        }

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dz = -1; dz <= 1; ++dz) {
                    if (dx == 0 && dy == 0 && dz == 0) continue;

                    auto neighborCell = std::make_tuple(
                        std::get<0>(cell) + dx,
                        std::get<1>(cell) + dy,
                        std::get<2>(cell) + dz
                    );

                    if (grid.find(neighborCell) != grid.end()) {
                        for (auto* p1 : particles) {
                            for (auto* p2 : grid[neighborCell]) {
                                double distance = p1->getPos().dist(p2->getPos());
                                if (distance < cellSize) {
                                    collisions.emplace_back(p1, p2);

                                    Vect direction = (p2->getPos() - p1->getPos()) / distance;
                                    Vect repulsiveForce = direction * (cellSize - distance) * 10.0; // Arbitrary strength
                                    p1->addForce(-repulsiveForce);
                                    p2->addForce(repulsiveForce);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return collisions;
}

std::tuple<int, int, int> GridCollision::getCellCoords(const Point& pos) const {
    return {
        static_cast<int>(std::floor(pos.getX() / cellSize)),
        static_cast<int>(std::floor(pos.getY() / cellSize)),
        static_cast<int>(std::floor(pos.getZ() / cellSize))
    };
}