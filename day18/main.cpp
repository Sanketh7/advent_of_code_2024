#include <iostream>
#include <vector>
#include <queue>
#include <complex>
#include <climits>
#include <optional>

// constexpr int NUM_ROWS = 7;
// constexpr int NUM_COLS = 7;
// constexpr int MAX_LINES = 12;
constexpr int NUM_ROWS = 71;
constexpr int NUM_COLS = 71;
constexpr int MAX_LINES = 1024;
constexpr std::complex<int> complex_i{0, 1};

enum class Cell {
    Empty, Wall
};

bool valid_pos(int r, int c) {
    return 0 <= r && r < NUM_ROWS && 0 <= c && c < NUM_COLS;
}

bool valid_pos(std::complex<int> pos) {
    return valid_pos(pos.real(), pos.imag());
}

std::optional<int> calc_path(const std::vector<std::vector<Cell>>& grid) {
    std::vector<std::vector<int>> dist(NUM_ROWS, std::vector<int>(NUM_COLS, INT_MAX));
    dist[0][0] = 0;
    std::queue<std::complex<int>> q;
    q.emplace(0, 0);
    while (!q.empty()) {
        std::complex<int> curr = q.front();
        q.pop();
        int curr_dist = dist[curr.real()][curr.imag()];
        std::complex<int> dir = complex_i;
        for (int d = 0; d < 4; d++, dir *= complex_i) {
            std::complex<int> next = curr + dir;
            if (!valid_pos(next)) continue;
            if (grid[next.real()][next.imag()] == Cell::Wall) continue;
            if (curr_dist + 1 < dist[next.real()][next.imag()]) {
                dist[next.real()][next.imag()] = curr_dist + 1;
                q.push(next);
            }
        }
    }
    int ret = dist[NUM_ROWS - 1][NUM_COLS - 1];
    if (ret == INT_MAX) {
        return std::nullopt;
    }
    return ret;
}

int part1(const std::vector<std::complex<int>>& walls) {
    std::vector<std::vector<Cell>> grid(NUM_ROWS, std::vector<Cell>(NUM_COLS, Cell::Empty));
    for (int i = 0; i < MAX_LINES; i++) {
        if (valid_pos(walls[i])) {
            grid[walls[i].real()][walls[i].imag()] = Cell::Wall;
        }
    }

    return *calc_path(grid);
}

int part2(const std::vector<std::complex<int>>& walls) {
    std::vector<std::vector<Cell>> grid(NUM_ROWS, std::vector<Cell>(NUM_COLS, Cell::Empty));
    int l = 0, r = walls.size() - 1;
    int ans = -1;
    while (l <= r) {
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < NUM_COLS; j++) {
                grid[i][j] = Cell::Empty;
            }
        }
        int mid = l + (r - l) / 2;
        for (int i = 0; i <= mid; i++) {
            if (valid_pos(walls[i])) {
                grid[walls[i].real()][walls[i].imag()] = Cell::Wall;
            }
        }
        bool has_path = calc_path(grid).has_value();
        if (has_path) {
            l = mid + 1;
        } else {
            ans = mid;
            r = mid - 1;
        }
    }
    return ans;
}

int main() {
    std::vector<std::complex<int>> walls;
    std::string line;
    while (std::getline(std::cin, line)) {
        size_t comma_pos = line.find(',');
        int r = std::stoi(line.substr(0, comma_pos));
        int c = std::stoi(line.substr(comma_pos+1));
        walls.emplace_back(r, c);
    }

    // int ans = part1(walls);
    // std::cout << ans << std::endl;
    int ans = part2(walls);
    std::cout << walls[ans].real() << ',' << walls[ans].imag() << std::endl;
}