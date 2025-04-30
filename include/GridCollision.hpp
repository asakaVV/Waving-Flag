#pragma once

#include "PMat.hpp"
#include <vector>
#include <unordered_map>
#include <tuple>
#include <functional>

namespace std {
    template <>
    struct hash<std::tuple<int, int, int>> {
        std::size_t operator()(const std::tuple<int, int, int>& key) const noexcept {
            auto [x, y, z] = key;
            std::size_t h1 = std::hash<int>{}(x);
            std::size_t h2 = std::hash<int>{}(y);
            std::size_t h3 = std::hash<int>{}(z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

class GridCollision {
public:
    GridCollision(double cellSize);

    void updateGrid(const std::vector<PMat>& particles);
    std::vector<std::pair<PMat*, PMat*>> detectCollisions();

private:
    double cellSize;
    std::unordered_map<std::tuple<int, int, int>, std::vector<PMat*>> grid;

    std::tuple<int, int, int> getCellCoords(const Point& pos) const;
};