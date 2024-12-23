#include <iostream>
#include <cstdint>
#include <vector>
#include <ranges>
#include <cassert>
#include <cmath>
#include <unordered_map>

int count_digits(int64_t x) {
    return log10(x) + 1;
}

std::pair<int64_t, int64_t> split_number(int64_t x) {
    int num_digits = count_digits(x);
    int64_t divisor = pow(10ll, num_digits / 2);
    assert(divisor > 0);
    int64_t left = x / divisor;
    int64_t right = x % divisor;
    assert(left >= 0);
    assert(right >= 0);
    return {left, right};
}

std::unordered_map<int64_t, int64_t> simulate(const std::unordered_map<int64_t, int64_t>& before) {
    std::unordered_map<int64_t, int64_t> after;
    for (const auto& [stone, cnt] : before) {
        if (stone == 0) {
            after[1] += cnt;
            continue;
        }
        if (count_digits(stone) % 2 == 0) {
            const auto [left, right] = split_number(stone);
            after[left] += cnt;
            after[right] += cnt;
            continue;
        }
        after[stone * 2024ll] += cnt;
    }
    return after;
}

int64_t part1and2(std::unordered_map<int64_t, int64_t> stones, int num_steps) {
    for (int i = 0; i < num_steps; i++) {
        stones = simulate(stones);
    }
    int64_t ans = 0;
    for (const auto& [stone, cnt] : stones) {
        ans += cnt;
    }
    return ans;
}

int main() {
    std::string line;
    std::getline(std::cin, line);
    auto sv = line 
        | std::views::split(' ') 
        | std::views::transform([](auto&& s) { 
            return std::stoll(std::string(s.begin(), s.end())); 
        });
    std::unordered_map<int64_t, int64_t> stones;
    for (auto it = sv.begin(); it != sv.end(); it++) {
        ++stones[*it];
    }

    // int ans = part1and2(stones, 25);
    int64_t ans = part1and2(stones, 75);
    std::cout << ans << std::endl;
}