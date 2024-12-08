#include <iostream>
#include <vector>
#include <unordered_map>
#include <numeric>

int part1(const std::vector<std::string>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    std::unordered_map<char, std::vector<std::pair<int, int>>> ants;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char c = grid[i][j];
            if (c != '.') {
                ants[c].emplace_back(i, j);
            }
        }
    }

    std::vector<std::vector<bool>> antinodes(n, std::vector<bool>(m, false));
    for (const auto& [ant, locs] : ants) {
        for (auto [x1, y1] : locs) {
            for (auto [x2, y2] : locs) {
                if (x1 == x2 && y1 == y2) continue;
                int x3 = (x2 - x1) + x2;
                int y3 = (y2 - y1) + y2;
                if (x3 >= 0 && x3 < n && y3 >= 0 && y3 < m) {
                    antinodes[x3][y3] = true;
                }
            }
        }
    }
    int ans = 0;
    for (const auto& row : antinodes) {
        for (auto b : row) {
            if (b) ++ans;
        }
    }
    return ans;
}

void fill_slope(std::vector<std::vector<bool>>& antinodes, int start_x, int start_y, int dx, int dy) {
    int x = start_x;
    int y = start_y;
    while (x >= 0 && x < antinodes.size() && y >= 0 && y < antinodes[x].size()) {
        antinodes[x][y] = true;
        x += dx;
        y += dy;
    }
    x = start_x;
    y = start_y;
    while (x >= 0 && x < antinodes.size() && y >= 0 && y < antinodes[x].size()) {
        antinodes[x][y] = true;
        x -= dx;
        y -= dy;
    }
}

int part2(const std::vector<std::string>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    std::unordered_map<char, std::vector<std::pair<int, int>>> ants;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char c = grid[i][j];
            if (c != '.') {
                ants[c].emplace_back(i, j);
            }
        }
    }

    std::vector<std::vector<bool>> antinodes(n, std::vector<bool>(m, false));
    for (const auto& [ant, locs] : ants) {
        for (auto [x1, y1] : locs) {
            for (auto [x2, y2] : locs) {
                if (x1 == x2 && y1 == y2) continue;
                int dx = x2 - x1;
                int dy = y2 - y1;
                int gcd = std::gcd(dx, dy);
                fill_slope(antinodes, x1, y1, dx / gcd, dy / gcd);
            }
        }
    }
    int ans = 0;
    for (const auto& row : antinodes) {
        for (auto b : row) {
            if (b) ++ans;
        }
    }
    return ans;
}

int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        grid.push_back(line);
    }

    // int ans = part1(grid);
    int ans = part2(grid);
    std::cout << ans << std::endl;
}