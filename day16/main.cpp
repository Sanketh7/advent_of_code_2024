#include <iostream>
#include <vector>
#include <cassert>
#include <complex>
#include <optional>
#include <queue>
#include <map>
#include <climits>
#include <set>

constexpr std::complex<int> complex_i{0, 1};

class Grid {
public:
    void add_row(std::string&& row) {
        if (!_rows.empty()) {
            assert(static_cast<int>(row.size()) == _n);
        } else {
            _n = row.size();
        }
        _m++;
        _rows.emplace_back(std::move(row));
    }

    int m() const { return _m; }
    int n() const { return _n; }
    bool validate_pos(int r, int c) const { 
        return 0 <= r && r < _m && 0 <= c && c < _n;
    }
    bool validate_pos(std::complex<int> pos) const {
        return validate_pos(pos.real(), pos.imag());
    }
    char at(int r, int c) const {
        assert(validate_pos(r, c));
        return _rows[r][c];
    }
    char at(std::complex<int> pos) const {
        return at(pos.real(), pos.imag());
    }

    std::optional<std::complex<int>> find_first(char target) const {
        for (int r = 0; r < _m; r++) {
            for (int c = 0; c < _n; c++) {
                if (at(r, c) == target) {
                    return std::complex<int>{r, c};
                }
            }
        }
        return {};
    }

private:
    int _m{0}, _n{0};
    std::vector<std::string> _rows;
};

struct State {
    std::complex<int> pos;
    std::complex<int> dir;
};

auto operator<=>(const State& a, const State& b) {
    if (auto c = a.pos.real() <=> b.pos.real(); c != 0) return c;
    if (auto c = a.pos.imag() <=> b.pos.imag(); c != 0) return c;
    if (auto c = a.dir.real() <=> b.dir.real(); c != 0) return c;
    return a.dir.imag() <=> b.dir.imag();
}

std::vector<std::pair<State, int>> generate_transitions(State curr_state, const Grid& grid, bool reverse) {
    std::vector<std::pair<State, int>> ret;
    {
        std::complex<int> next_dir = curr_state.dir * complex_i;
        for (int d = 0; d < 3; d++, next_dir *= complex_i) {
            int cost = d == 1 ? 2000 : 1000;
            ret.emplace_back(State{.pos = curr_state.pos, .dir = next_dir}, cost);
        }
    }
    {
        std::complex<int> next_pos = reverse ? curr_state.pos - curr_state.dir : curr_state.pos + curr_state.dir;
        if (grid.validate_pos(next_pos) && grid.at(next_pos) != '#') {
            ret.emplace_back(State{.pos = next_pos, .dir = curr_state.dir}, 1);
        }
    }
    return ret;
}

std::map<State, int> calc_dist(const Grid& grid, std::complex<int> src, std::complex<int> dest) {
    std::map<State, int> dist;
    std::priority_queue<std::pair<int, State>> pq;
    State src_state{.pos = src, .dir = {0, 1}};
    dist[src_state] = 0;
    pq.emplace(0, src_state);

    while (!pq.empty()) {
        if (-pq.top().first > dist[pq.top().second]) {
            pq.pop();
            continue;
        }
        State curr_state = pq.top().second;
        int curr_dist = -pq.top().first;
        pq.pop();
        if (curr_state.pos == dest) break;

        for (const auto& [next_state, cost] : generate_transitions(curr_state, grid, false)) {
            if (dist.find(next_state) == dist.end() || curr_dist + cost < dist[next_state]) {
                int new_dist = curr_dist + cost;
                dist[next_state] = new_dist;
                pq.emplace(-new_dist, next_state);
            }
        }
    }

    return dist;
}

int part1(const Grid& grid) {
    std::complex<int> src = *grid.find_first('S');
    std::complex<int> dest = *grid.find_first('E');
    std::map<State, int> dist = calc_dist(grid, src, dest);
    std::complex<int> dir = complex_i;
    int ans = INT_MAX;
    for (int d = 0; d < 4; d++, dir *= complex_i) {
        State state{.pos = dest, .dir = dir};
        if (auto it = dist.find(state); it != dist.end()) {
            ans = std::min(ans, it->second);
        }
    }
    return ans;
}

void backtrack(State curr_state, const Grid& grid, const std::map<State, int>& dist, std::set<std::pair<int, int>>& on_path, std::set<State>& vis) {
    vis.insert(curr_state);
    on_path.emplace(curr_state.pos.real(), curr_state.pos.imag());
    int curr_dist = dist.at(curr_state);
    for (const auto& [next_state, cost] : generate_transitions(curr_state, grid, true)) {
        if (vis.find(next_state) != vis.end()) continue;
        if (auto it = dist.find(next_state); it != dist.end() && it->second == curr_dist - cost) {
            backtrack(next_state, grid, dist, on_path, vis);
        }
    }
}

int part2(const Grid& grid) {
    std::complex<int> src = *grid.find_first('S');
    std::complex<int> dest = *grid.find_first('E');
    std::map<State, int> dist = calc_dist(grid, src, dest);
    std::complex<int> dir = complex_i;
    int best = INT_MAX;
    for (int d = 0; d < 4; d++, dir *= complex_i) {
        State state{.pos = dest, .dir = dir};
        if (auto it = dist.find(state); it != dist.end()) {
            best = std::min(best, it->second);
        }
    }
    assert(best != INT_MAX);
    std::set<std::pair<int, int>> on_path;
    std::set<State> vis;
    dir = complex_i;
    for (int d = 0; d < 4; d++, dir *= complex_i) {
        State state{.pos = dest, .dir = dir};
        if (auto it = dist.find(state); it != dist.end() && it->second == best) {
            backtrack(state, grid, dist, on_path, vis);
        }
    }
    return on_path.size();
}

int main() {
    Grid grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        grid.add_row(std::move(line));
    }

    // int ans = part1(grid);
    int ans = part2(grid);
    std::cout << ans << std::endl;
}