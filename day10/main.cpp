#include <vector>
#include <iostream>
#include <queue>

using Grid = std::vector<std::vector<int>>;

int score_head(const Grid& grid, std::pair<int, int> src) {
    std::vector<std::vector<bool>> vis(grid.size(), std::vector<bool>(grid[0].size(), false));
    std::queue<std::pair<int, int>> q;
    q.push(src);
    vis[src.first][src.second] = true;
    int score = 0;
    while (!q.empty()) {
        std::pair<int, int> node = q.front();
        q.pop();
        if (grid[node.first][node.second] == 9) {
            ++score;
        }
        for (int di = -1; di <= 1; di++) {
            for (int dj = -1; dj <= 1; dj++) {
                if (di != 0 && dj != 0) continue;
                std::pair<int, int> next{node.first + di, node.second + dj};
                if (next.first < 0 || next.first >= grid.size() || next.second < 0 || next.second >= grid[0].size()) {
                    continue;
                }
                if (!vis[next.first][next.second] && grid[next.first][next.second] == grid[node.first][node.second] + 1) {
                    q.push(next);
                    vis[next.first][next.second] = true;
                }
            }
        }
    }
    return score;
}

int part1(const Grid& grid) {
    int m = grid.size();
    int n = grid[0].size();
    int ans = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 0) {
                ans += score_head(grid, {i, j});
            }
        }
    }
    return ans;
}

int rate_head(const Grid& grid, std::pair<int, int> src) {
    int m = grid.size(), n = grid[0].size();
    std::vector<std::vector<int>> ratings(m, std::vector<int>(n, 0));
    ratings[src.first][src.second] = 1;
    for (int level = 1; level <= 9; level++) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != level) continue;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        if (di != 0 && dj != 0) continue;
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni < 0 || ni >= m || nj < 0 || nj >= n) continue;
                        if (grid[ni][nj] + 1 == grid[i][j]) {
                            ratings[i][j] += ratings[ni][nj];
                        }
                    }
                }
            }
        }
    }
    int ret = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 9) {
                ret += ratings[i][j];
            }
        }
    }
    return ret;
}

int part2(const Grid& grid) {
    int m = grid.size();
    int n = grid[0].size();
    int ans = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 0) {
                ans += rate_head(grid, {i, j});
            }
        }
    }
    return ans;
}

int main() {
    Grid grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::vector<int> line_nums(line.size());
        for (int i = 0; i < line.size(); i++) {
            line_nums[i] = line[i] - '0';
        }
        grid.push_back(line_nums);
    }

    // int ans = part1(grid);
    int ans = part2(grid);
    std::cout << ans << std::endl;
}