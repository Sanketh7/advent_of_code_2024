#include <iostream>
#include <vector>
#include <cassert>
#include <complex>

struct RegionStats {
    char plant;
    int area;
    int perimeter;
    int corners; // actually number of corners * 3

    RegionStats& operator+=(const RegionStats& other) {
        assert(plant == other.plant);
        area += other.area;
        perimeter += other.perimeter;
        corners += other.corners;
        return *this;
    }
};

bool is_inside_grid(int i, int j, const std::vector<std::string>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    return i >= 0 && i < m && j >= 0 && j < n;
}

RegionStats calc_stats(int i, int j, const std::vector<std::string>& grid, std::vector<std::vector<bool>>& vis) {
    vis[i][j] = true;
    RegionStats stats{.plant = grid[i][j], .area = 0, .perimeter = 0};
    int diff_neighbors = 0;
    for (int ni = i-1; ni <= i+1; ni++) {
        for (int nj = j-1; nj <= j+1; nj++) {
            if (ni != i && nj != j) continue;
            if (!is_inside_grid(ni, nj, grid) || grid[ni][nj] != grid[i][j]) {
                ++diff_neighbors;
            }
            if (is_inside_grid(ni, nj, grid) && !vis[ni][nj] && grid[i][j] == grid[ni][nj]) {
                stats += calc_stats(ni, nj, grid, vis);
            }
        }
    }
    stats.area += 1;
    stats.perimeter += diff_neighbors;
    std::complex<int> dir{0, -1};
    int corners = 0;
    auto is_diff = [&grid, i, j](const std::complex<int>& dir) -> bool {
        return !is_inside_grid(i+dir.real(), j+dir.imag(), grid) || grid[i+dir.real()][j+dir.imag()] != grid[i][j];
    };
    auto is_valid = [&grid, i, j](const std::complex<int>& dir) -> bool {
        return is_inside_grid(i+dir.real(), j+dir.imag(), grid);
    };
    for (int d = 0; d < 4; d++, dir *= std::complex<int>{0, 1}) {
        std::complex<int> dir90 = dir * std::complex<int>{0, 1};
        std::complex<int> dir45 = dir + dir90;
        bool diff = is_diff(dir);
        bool diff45 = is_diff(dir45);
        bool diff90 = is_diff(dir90);
        if (diff && diff45 && diff90) corners += 3;
        if (diff && !diff45 && diff90) corners += 3;
        if (!diff && !diff45 && diff90) corners += 3 - is_valid(dir) - is_valid(dir45);
        if (!diff && diff45 && !diff90) corners += 3 - is_valid(dir) - is_valid(dir90);
        if (diff && !diff45 && !diff90) corners += 3 - is_valid(dir45) - is_valid(dir90);
    }
    stats.corners += corners;
    return stats;
}

int part1(const std::vector<std::string>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    std::vector<std::vector<bool>> vis(m, std::vector<bool>(n, false));
    int ans = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (vis[i][j]) continue;
            RegionStats stats = calc_stats(i, j, grid, vis);
            ans += stats.perimeter * stats.area;
        }
    }
    return ans;
}

int part2(const std::vector<std::string>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    std::vector<std::vector<bool>> vis(m, std::vector<bool>(n, false));
    int ans = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (vis[i][j]) continue;
            RegionStats stats = calc_stats(i, j, grid, vis);
            ans += stats.corners / 3 * stats.area;
        }
    }
    return ans;
}

int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        grid.emplace_back(std::move(line));
    }

    // int ans = part1(grid);
    int ans = part2(grid);
    std::cout << ans << std::endl;
}