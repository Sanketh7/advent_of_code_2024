#include <iostream>
#include <ranges>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_map>

int part1(std::vector<int> locs1, std::vector<int> locs2) {
    assert(locs1.size() == locs2.size());
    std::sort(locs1.begin(), locs1.end());
    std::sort(locs2.begin(), locs2.end());
    int n = locs1.size();
    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans += abs(locs1[i] - locs2[i]);
    }
    return ans;
}

int part2(std::vector<int> locs1, std::vector<int> locs2) {
    std::unordered_map<int, int> cnt;
    for (auto& x : locs2) {
        ++cnt[x];
    }
    int ans = 0;
    for (auto& x : locs1) {
        ans += x * cnt[x];
    }
    return ans;
}

int main() {
    std::vector<int> locs1, locs2;
    std::string line;
    while (std::getline(std::cin, line)) {
        auto sv = line 
            | std::views::split(std::string_view{"  "}) 
            | std::views::transform([](auto&& s) { 
                return std::stoi(std::string(s.begin(), s.end())); 
            });
        std::vector<int> tokens(sv.begin(), sv.end());
        locs1.push_back(tokens[0]);
        locs2.push_back(tokens[1]);
    }

    // int ans = part1(locs1, locs2);
    int ans = part2(locs1, locs2);
    std::cout << ans << std::endl;
}