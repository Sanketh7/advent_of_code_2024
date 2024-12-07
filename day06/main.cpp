#include <iostream>
#include <vector>
#include <cassert>
#include <map>
#include <set>

enum class Dir {
    UP, DOWN, LEFT, RIGHT
};

constexpr std::pair<int, int> dirToDeltas(Dir dir) {
    switch (dir) {
        case Dir::UP:
            return {-1, 0};
        case Dir::DOWN:
            return {1, 0};
        case Dir::LEFT:
            return {0, -1};
        case Dir::RIGHT:
            return {0, 1};
        default:
            throw std::invalid_argument{"Bad direction"};
    }
}

constexpr Dir turn90Deg(Dir dir)  {
    switch (dir) {
        case Dir::UP:
            return Dir::RIGHT;
        case Dir::DOWN:
            return Dir::LEFT;
        case Dir::LEFT:
            return Dir::UP;
        case Dir::RIGHT:
            return Dir::DOWN;
        default:
            throw std::invalid_argument{"Bad direction"};
    }
}

int part1(const std::vector<std::string>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    int row = -1, col = -1;
    Dir dir = Dir::UP;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < m; c++) {
            if (grid[r][c] == '^') {
                row = r;
                col = c;
            }
        }
    }
    assert(row != -1 && col != -1);
    std::vector<std::vector<bool>> vis(n, std::vector<bool>(m, false));
    while (row >= 0 && row < n && col >= 0 && col < m) {
        vis[row][col] = true;
        std::pair<int, int> deltas = dirToDeltas(dir);
        int nrow = row + deltas.first, ncol = col + deltas.second;
        if (nrow < 0 || nrow >= n || ncol < 0 || ncol >= m) break;
        if (grid[nrow][ncol] == '#') {
            dir = turn90Deg(dir);
            continue;
        }
        row = nrow;
        col = ncol;
    }

    int ans = 0;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < m; c++) {
            ans += vis[r][c] ? 1 : 0;
        }
    }
    return ans;
}

bool has_cycle(const std::vector<std::string>& grid, std::pair<int, int> pos, std::pair<int, int> obstacle) {
    int n = grid.size();
    int m = grid[0].size();
    Dir dir = Dir::UP;
    int iter = 0;
    while (iter < 5 * n * m && pos.first >= 0 && pos.first < n && pos.second >= 0 && pos.second < m) {
        iter++;
        std::pair<int, int> delta = dirToDeltas(dir);
        std::pair<int, int> next{pos.first + delta.first, pos.second + delta.second};
        if (next.first < 0 || next.first >= n || next.second < 0 || next.second >= m) return false;
        if (grid[next.first][next.second] == '#' || next == obstacle) {
            dir = turn90Deg(dir);
            continue;
        }
        pos = next;
    }
    return pos.first >= 0 && pos.first < n && pos.second >= 0 && pos.second < m;
}

int part2(const std::vector<std::string>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    std::pair<int, int> start{-1, -1};
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < m; c++) {
            if (grid[r][c] == '^') {
                start = {r, c};
            }
        }
    }
    assert(start.first != -1 && start.second != -1);

    int ans = 0;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < m; c++) {
            if (grid[r][c] == '.') {
                ans += has_cycle(grid, start, {r, c}) ? 1 : 0;
            }
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