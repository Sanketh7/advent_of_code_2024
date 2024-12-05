#include <iostream>
#include <string_view>
#include <vector>

struct Grid {
    std::vector<std::string> rows;

    int count_word(const std::string_view word) const {
        int cnt = 0;
        for (int row = 0; row < rows.size(); ++row) {
            for (int col = 0; col < rows[row].size(); ++col) {
                for (int drow = -1; drow <= 1; ++drow) {
                    for (int dcol = -1; dcol <= 1; ++dcol) {
                        cnt += count_word_helper(row, col, drow, dcol, word) ? 1 : 0;
                    }
                }
            }
        }
        return cnt;
    }

    int count_xmas() const {
        int cnt = 0;
        for (int row = 0; row < rows.size(); ++row) {
            for (int col = 0; col < rows[row].size(); ++col) {
                bool diag1 = count_word_helper(row-1, col-1, 1, 1, "MAS") || count_word_helper(row+1, col+1, -1, -1, "MAS");
                bool diag2 = count_word_helper(row-1, col+1, 1, -1, "MAS") || count_word_helper(row+1, col-1, -1, 1, "MAS");
                cnt += (diag1 && diag2) ? 1 : 0;
            }
        }
        return cnt;
    }

private:
    bool check_cell(int row, int col, char target) const {
        if (row < 0 || row >= rows.size() || col < 0 || col >= rows[row].size()) {
            return false;
        }
        return rows[row][col] == target;
    }

    int count_word_helper(int row, int col, int drow, int dcol, const std::string_view word) const {
        for (int i = 0; i < word.size(); i++) {
            if (!check_cell(row + drow * i, col + dcol * i, word[i])) {
                return false;
            }
        }
        return true;
    }
};

int part1(const Grid& grid) {
    return grid.count_word("XMAS");
}

int part2(const Grid& grid) {
    return grid.count_xmas();
}

int main() {
    std::vector<std::string> rows;
    std::string line;
    while (std::getline(std::cin, line)) {
        rows.push_back(line);
    }

    Grid grid{.rows = rows};
    // int ans = part1(grid);
    int ans = part2(grid);
    std::cout << ans << std::endl;
}