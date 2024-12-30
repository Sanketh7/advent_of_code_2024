#include <iostream>
#include <regex>
#include <cassert>
#include <complex>
#include <vector>
#include <set>
#include <map>

// constexpr int NUM_ROWS = 7;
// constexpr int NUM_COLS = 11;
constexpr int NUM_ROWS = 103;
constexpr int NUM_COLS = 101;

int bound_dim(int val, int dim) {
    if (val >= 0) {
        return val % dim;
    } else {
        int tmp = (-val) % dim;
        return tmp == 0 ? 0 : dim - tmp;
    }
}

struct Robot {
    std::complex<int> pos, vel;

    void do_step(int num_steps) {
        std::complex<int> new_pos = this->pos + num_steps * this->vel;
        new_pos = std::complex<int>{bound_dim(new_pos.real(), NUM_ROWS), bound_dim(new_pos.imag(), NUM_COLS)};
        this->pos = new_pos;
    }

    std::pair<int, int> pos_as_pair() const {
        return {pos.real(), pos.imag()};
    }
};

std::ostream& operator<<(std::ostream& os, const Robot& robot) {
    os << robot.pos << ' ' << robot.vel;
    return os;
}

void print_board(const std::vector<Robot>& robots) {
    std::map<std::pair<int, int>, int> pos_cnt;
    for (const auto& robot : robots) {
        pos_cnt[robot.pos_as_pair()]++;
    }
    for (int r = 0; r < NUM_ROWS; r++) {
        for (int c = 0; c < NUM_COLS; c++) {
            if (auto it = pos_cnt.find({r, c}); it != pos_cnt.end()) {
                std::cout << it->second;
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

int calc_safety_score(const std::vector<Robot>& robots) {
    std::multiset<std::pair<int, int>> pos_cnt;
    for (const auto& robot : robots) {
        pos_cnt.insert(robot.pos_as_pair());
    }
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for (int r = 0; r < NUM_ROWS / 2; r++) {
        for (int c = 0; c < NUM_COLS / 2; c++) q1 += pos_cnt.count({r, c});
        for (int c = NUM_COLS / 2 + 1; c < NUM_COLS; c++) q2 += pos_cnt.count({r, c});
    }
    for (int r = NUM_ROWS / 2 + 1; r < NUM_ROWS; r++) {
        for (int c = 0; c < NUM_COLS / 2; c++) q3 += pos_cnt.count({r, c});
        for (int c = NUM_COLS / 2 + 1; c < NUM_COLS; c++) q4 += pos_cnt.count({r, c});
    }
    return q1 * q2 * q3 * q4;
}

bool is_all_unique(const std::vector<Robot>& robots) {
    std::set<std::pair<int, int>> pos_cnt;
    for (const Robot& robot : robots) {
        if (pos_cnt.find(robot.pos_as_pair()) != pos_cnt.end()) {
            return false;
        }
        pos_cnt.insert(robot.pos_as_pair());
    }
    return true;
}

int part1(std::vector<Robot> robots) {
    for (Robot& robot : robots) {
        robot.do_step(100);
    }
    return calc_safety_score(robots);
}

int part2(std::vector<Robot> robots) {
    int step = 0;
    while (true) {
        ++step;
        for (Robot& robot : robots) {
            robot.do_step(1);
        }
        if (is_all_unique(robots)) {
            return step;
        }
    }
}

int main() {
    static const std::regex pattern{"p=(-?\\d+),(-?\\d+) v=(-?\\d+),(-?\\d+)"};
    std::smatch matches;
    std::string line;
    std::vector<Robot> robots;
    while (std::getline(std::cin, line)) {
        assert(std::regex_search(line, matches, pattern));
        robots.emplace_back(Robot{
            .pos = {std::stoi(matches[2]), std::stoi(matches[1])}, 
            .vel = {std::stoi(matches[4]), std::stoi(matches[3])}, 
        });
    }

    // int ans = part1(robots);
    int ans = part2(robots);
    std::cout << ans << std::endl;
}