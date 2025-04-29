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
        // Check collisions within the same cell
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                if (particles[i]->getPos().dist(particles[j]->getPos()) < cellSize) {
                    collisions.emplace_back(particles[i], particles[j]);
                }
            }
        }

        // Check collisions with neighboring cells
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
                                if (p1->getPos().dist(p2->getPos()) < cellSize) {
                                    collisions.emplace_back(p1, p2);
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